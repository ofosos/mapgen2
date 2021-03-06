#include "noise_gen/module_manager.h"

#include <stdexcept>

ModuleManager::ModuleManager() :
    modules_{}
{
}

void ModuleManager::create(const std::string& name, NoiseModule::Type type)
{
    modules_[name] = NoiseModule::Ptr{ new NoiseModule(name, type) };
}

void ModuleManager::remove(const std::string& name)
{
    if (!has(name)) return;

    modules_.erase(name);

    for (auto& pair : modules_)
    {
        pair.second->update();
    }
}

void ModuleManager::rename(const std::string& current_name, const std::string& new_name)
{
    auto module = modules_[current_name];
    module->setName(new_name);

    modules_.erase(current_name);
    modules_.insert({ new_name, module });
}

NoiseModule::Ptr& ModuleManager::get(const std::string& name)
{
    if (modules_.find(name) != modules_.end())
    {
        return modules_[name];
    }
    else
    {
        throw std::runtime_error("No noise module with given name");
    }
}

bool ModuleManager::has(const std::string& name) const
{
    return modules_.find(name) != modules_.end();
}

void ModuleManager::forEach(std::function<void(const std::string&, NoiseModule&)> fn)
{
    for (auto& it : modules_)
    {
        fn(it.first, *it.second);
    }
}

std::size_t ModuleManager::size() const noexcept
{
    return modules_.size();
}