#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace Epsilon {
    ftxui::Component CreateRightPanel() {
        return ftxui::Renderer([] { 
            return ftxui::text("Right") | ftxui::center; 
        });
    }
}

