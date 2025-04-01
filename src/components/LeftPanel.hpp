#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace Epsilon {
    class LeftPanel {
    private:
        std::string title;
        int width;
        
    public:
        LeftPanel() : title("Left"), width(30) {}
        
        ftxui::Component Create() {
            return ftxui::Renderer([] { 
                return ftxui::text("Left") | ftxui::center; 
            });
        }
        
        int GetWidth() const { return width; }
    };
}
