#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace Epsilon {
    class MainPanel {
    private:
        std::string title;
        
    public:
        MainPanel() : title("middle") {}
        
        ftxui::Component Create() {
            return ftxui::Renderer([] { 
                return ftxui::text("middle") | ftxui::center; 
            });
        }
    };
}
