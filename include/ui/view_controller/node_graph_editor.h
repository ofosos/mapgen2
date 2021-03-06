#ifndef UI_VIEW_CONTROLLER_NODE_GRAPH_EDITOR_H
#define UI_VIEW_CONTROLLER_NODE_GRAPH_EDITOR_H

#include "ui/view_controller/tab_renderer.h"
#include "ui/view_controller/module_manager_controller.h"

#include "addons/imguinodegrapheditor/imguinodegrapheditor.h"

class NodeGraphEditorTab : public TabRenderer
{
public:
    // TODO: scoped
    enum NodeTypes
    {
        ABS          = NoiseModule::Type::Abs,
        ADD          = NoiseModule::Type::Add,
        BILLOW       = NoiseModule::Type::Billow,
        BLEND        = NoiseModule::Type::Blend,
        CACHE        = NoiseModule::Type::Cache,
        CHECKERBOARD = NoiseModule::Type::Checkerboard,
        CLAMP        = NoiseModule::Type::Clamp,
        CONST        = NoiseModule::Type::Const,
        CYLINDERS    = NoiseModule::Type::Cylinders,
        DISPLACE     = NoiseModule::Type::Displace,
        EXPONENT     = NoiseModule::Type::Exponent,
        INVERT       = NoiseModule::Type::Invert,
        MAX          = NoiseModule::Type::Max,
        MIN          = NoiseModule::Type::Min,
        MULTIPLY     = NoiseModule::Type::Multiply,
        PERLIN       = NoiseModule::Type::Perlin,
        POWER        = NoiseModule::Type::Power,
        SELECT       = NoiseModule::Type::Select,
        SPHERES      = NoiseModule::Type::Spheres,
        ROTATE_POINT = NoiseModule::Type::RotatePoint,
        SCALE_POINT  = NoiseModule::Type::ScalePoint,
        TRANSATE     = NoiseModule::Type::TranslatePoint,
        TURBULENCE   = NoiseModule::Type::Turbulence,
        VORONOI      = NoiseModule::Type::Voronoi,
        OUTPUT,
        NODE_TYPE_COUNT
    };

    NodeGraphEditorTab(ModuleManagerController& manager);
    virtual ~NodeGraphEditorTab();

    virtual void renderTab() override;

private:

    static ImGui::Node* nodeFactory(int nt, const ImVec2& pos, const ImGui::NodeGraphEditor& nge);
    static void linkCallback(const ImGui::NodeLink& link, ImGui::NodeGraphEditor::LinkState state, ImGui::NodeGraphEditor& nge);
    static void nodeCallback(ImGui::Node*& node, ImGui::NodeGraphEditor::NodeState state, ImGui::NodeGraphEditor& nge);

    ModuleManagerController& manager_;
    ImGui::NodeGraphEditor nge;
};

#endif // UI_VIEW_CONTROLLER_NODE_GRAPH_EDITOR_H