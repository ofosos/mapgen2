#include "mapgen_application.h"
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/GLContext.h>
#include <imgui.h>

using namespace Magnum;

MapGenApplication::MapGenApplication(const Arguments &arguments) : Platform::Application{ arguments }
{
    using namespace Math::Literals;
    /* Set clear color to dark blue */
    GL::Renderer::setClearColor(0xFFFFFF_rgbf);
}

void MapGenApplication::drawEvent()
{
    // Clear the window
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
    // new imgui frame
    imgui.newFrame(windowSize(), GL::defaultFramebuffer.viewport().size());

    if (ImGui::Begin("Hello"))
    {
        ImGui::Text("Hello World!!!");
        ImGui::End();
    }

    // draw imgui
    imgui.drawFrame();

    swapBuffers();
    redraw();
}

void MapGenApplication::viewportEvent(const Vector2i &size)
{
    GL::defaultFramebuffer.setViewport({ {}, size });
}

void MapGenApplication::keyPressEvent(KeyEvent &event)
{
    // exit if interacing with GUI
    if (imgui.keyPressEvent(event))
        return;
}

void MapGenApplication::keyReleaseEvent(KeyEvent &event)
{
    // exit if interacing with GUI
    if (imgui.keyReleaseEvent(event))
        return;
}

void MapGenApplication::mousePressEvent(MouseEvent &event)
{
    // exit if interacing with GUI
    if (imgui.mousePressEvent(event))
        return;
}

void MapGenApplication::mouseReleaseEvent(MouseEvent &event)
{
    // exit if interacing with GUI
    if (imgui.mouseReleaseEvent(event))
        return;
}

void MapGenApplication::mouseMoveEvent(MouseMoveEvent &event)
{
    // exit if interacing with GUI
    if (imgui.mouseMoveEvent(event))
        return;
}

void MapGenApplication::mouseScrollEvent(MouseScrollEvent &event)
{
    // exit if interacing with GUI
    if (imgui.mouseScrollEvent(event))
        return;
}

void MapGenApplication::textInputEvent(TextInputEvent &event)
{
    // exit if interacing with GUI
    if (imgui.textInputEvent(event))
        return;
}

/* main() function implementation */
MAGNUM_APPLICATION_MAIN(MapGenApplication)