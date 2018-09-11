#include "noise_gen/noise_module.h"
#include "detail/gen/source_counter_visitor.h"
#include "detail/gen/validation_visitor.h"
#include "detail/gen/set_param_visitor.h"

#include <noise/module/module.h>

#include <algorithm>
#include <iostream>
#include <stdexcept>

/**
    Dummy noise type used for initialization
*/
class DummyNoise : public noise::module::Module
{
public:
    DummyNoise() : Module(0)
    {

    }

    static DummyNoise& instance()
    {
        static DummyNoise d;
        return d;
    }
    
    virtual int GetSourceModuleCount() const override
    {
        return 0;
    }

    virtual double GetValue(double x, double y, double z) const override
    {
        return 0.0;
    }
};

static DummyNoise dummy;

struct ModuleRefVistor : public boost::static_visitor<noise::module::Module&>
{
public:
    template<typename T>
    noise::module::Module& operator()(T& module) const
    {
        return static_cast<noise::module::Module&>(module);
    }
};

struct InvalidateVistor : public boost::static_visitor<>
{
public:
    template<typename T>
    void operator()(T& operand) const
    {
        if (std::is_same<T, NoiseModule*>::value)
        {
            operand = T{};
        }
    }
};

/**
    Create libnoise modules and allocation parameters
*/
class ModuleFactory
{
public:
    static NoiseModule::ModuleVariant createModule(NoiseModule::Type type)
    {
        switch (type)
        {
        case NoiseModule::Type::Billow:
            return { noise::module::Billow() };
        case NoiseModule::Type::Blend:
            return { noise::module::Blend() };
        case NoiseModule::Type::Perlin:
            return { noise::module::Perlin() };
        case NoiseModule::Type::RidgedMulti:
            return { noise::module::RidgedMulti() };
        case NoiseModule::Type::ScaleBias:
            return { noise::module::ScaleBias() };
        case NoiseModule::Type::Select:
            return { noise::module::Select() };
        case NoiseModule::Type::Spheres:
            return { noise::module::Spheres() };
        case NoiseModule::Type::Turbulence:
            return { noise::module::Turbulence() };
        case NoiseModule::Type::Voronoi:
            return { noise::module::Voronoi() };
        default:
            throw std::runtime_error("Invalid noise type");
            break;
        }
    }

    static NoiseModule::ParameterMap createParams(NoiseModule::Type type)
    {
        switch (type)
        {
        case NoiseModule::Type::Billow:
            return {
                { "seed", 1337 },
                { "frequency", (float)noise::module::DEFAULT_BILLOW_FREQUENCY },
                { "octaves", RangedInt(1, 25, noise::module::DEFAULT_BILLOW_OCTAVE_COUNT) },
                { "persistence", RangedFloat(0.f, 1.f, noise::module::DEFAULT_BILLOW_PERSISTENCE) },
                { "lacunarity", RangedFloat(1.f, 4.f, noise::module::DEFAULT_BILLOW_LACUNARITY) },
            };
        case NoiseModule::Type::Blend:
            return {
            };
        case NoiseModule::Type::Perlin:
            return {
                {"seed", 1337},
                {"frequency", (float)noise::module::DEFAULT_PERLIN_FREQUENCY},
                {"octaves", RangedInt(1, 25, noise::module::DEFAULT_PERLIN_OCTAVE_COUNT)},
                {"persistence", RangedFloat(0.f, 1.f, noise::module::DEFAULT_PERLIN_PERSISTENCE)},
                {"lacunarity", RangedFloat(1.f, 4.f, noise::module::DEFAULT_PERLIN_LACUNARITY)},
            };
        case NoiseModule::Type::RidgedMulti:
            return {
                { "seed", 1337 },
                { "frequency", (float)noise::module::DEFAULT_RIDGED_FREQUENCY },
                { "octaves", RangedInt(1, 25, noise::module::DEFAULT_RIDGED_OCTAVE_COUNT) },
                { "lacunarity", RangedFloat(1.f, 4.f, noise::module::DEFAULT_RIDGED_LACUNARITY) },
            };
        case NoiseModule::Type::ScaleBias:
            return {
                {"bias", 0.0f},
                {"scale", 1.0f}
            };
        case NoiseModule::Type::Select:
            return {
                {"lower_bound", (float)noise::module::DEFAULT_SELECT_LOWER_BOUND},
                {"upper_bound", (float)noise::module::DEFAULT_SELECT_UPPER_BOUND},
                {"fall_off", (float)noise::module::DEFAULT_SELECT_EDGE_FALLOFF}
            };
        case NoiseModule::Type::Spheres:
            return {
                {"frequency", (float)noise::module::DEFAULT_SPHERES_FREQUENCY}
            };
        case NoiseModule::Type::Turbulence:
            return {
                {"seed", 1337},
                {"frequency", (float)noise::module::DEFAULT_TURBULENCE_FREQUENCY},
                {"power", (float)noise::module::DEFAULT_TURBULENCE_POWER},
                {"roughness", (float)noise::module::DEFAULT_TURBULENCE_ROUGHNESS}
            };
        case NoiseModule::Type::Voronoi:
            return {
                {"seed", 1337},
                {"displacement", (float)noise::module::DEFAULT_VORONOI_DISPLACEMENT},
                {"frequency", (float)noise::module::DEFAULT_VORONOI_FREQUENCY},
                {"enable_distance", false}
            };
        default:
            throw std::runtime_error("Invalid noise type");
            break;
        }
    }

    static NoiseModule::ParameterMapPtr initParams(NoiseModule::Type type)
    {
        auto params = createParams(type);
        return std::make_shared<NoiseModule::ParameterMap>(params);
    }
};

NoiseModule::NoiseModule(const std::string& name, NoiseModule::Type type) 
    : module_base_{ ModuleFactory::createModule(type) }
    , module_{ boost::apply_visitor(ModuleRefVistor{}, module_base_) }
    , source_refs_(module_.GetSourceModuleCount())
    , name_{ name }
    , type_{ type }
    , parameter_map_{ModuleFactory::initParams(type)}
    , is_valid_{false}
    , actual_source_count_{0}
{
    actual_source_count_ = module_.GetSourceModuleCount();

    const auto count = module_.GetSourceModuleCount();

    for (int i = 0; i < count; ++i)
    {
        module_.SetSourceModule(i, dummy);
    }
}

void NoiseModule::update()
{
    using namespace noise::module;

    is_valid_ = validate();

    if (!is_valid_)
    {
        return;
    }
    
    std::cout << name_ << " updating" << std::endl;
    boost::apply_visitor(SetParamsVisitor{ *parameter_map_ }, module_base_);
}

NoiseModule::ParameterMapPtr NoiseModule::getParams()
{
    return parameter_map_;
}

noise::module::Module& NoiseModule::getModule()
{
    return module_;
}

const std::string& NoiseModule::getName() const
{
    return name_;
}

void NoiseModule::setName(const std::string& name)
{
    this->name_ = name;
}

NoiseModule::Type NoiseModule::getType() const
{
    return type_;
}

bool NoiseModule::validate()
{
    auto valid_params = boost::apply_visitor(ValidationVisitor{ *parameter_map_ }, module_base_);
    auto valid_sources = std::all_of(source_refs_.begin(), source_refs_.begin() + actual_source_count_,
        [](NoiseModule::Ref ref) 
        {
            return !ref.expired();
        }
    );

    return valid_params && valid_sources;
}

bool NoiseModule::isValid() const
{
    return is_valid_;
}

void NoiseModule::setSourceModule(int index, NoiseModule::Ptr ptr)
{
    source_refs_[index] = NoiseModule::Ref{ ptr };
    if (ptr)
    {
        module_.SetSourceModule(index, ptr->getModule());
    }
}

int NoiseModule::getSourceModuleCount()
{
    return actual_source_count_;
}

NoiseModule::Ref NoiseModule::getSourceModule(int index)
{
    return source_refs_[index];
}

int NoiseModule::getActualSourceCount()
{
    return 0;
}