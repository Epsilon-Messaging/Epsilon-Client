#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace Epsilon {
    ftxui::Component CreateHeaderPanel() {
        return ftxui::Renderer([] { 
            return ftxui::text("Epsilon") | ftxui::center; 
        });
    }
}

