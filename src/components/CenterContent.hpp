#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace Epsilon {
    ftxui::Component CreateMainPanel() {
        return ftxui::Renderer([] { 
            return ftxui::text("middle") | ftxui::center; 
        });
    }
}

