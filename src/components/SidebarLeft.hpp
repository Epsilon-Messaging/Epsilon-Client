#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace Epsilon {
    ftxui::Component CreateLeftPanel() {
        return ftxui::Renderer([] { 
            return ftxui::text("Left") | ftxui::center; 
        });
    }
}

