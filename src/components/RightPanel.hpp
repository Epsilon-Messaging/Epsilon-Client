#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace Epsilon {
    class RightPanel {
    private:
        std::string title;
        int width;
        
    public:
        RightPanel() : title("Right"), width(30) {}
        
        ftxui::Component Create() {
            return ftxui::Renderer([] { 
                return ftxui::text("Right") | ftxui::center; 
            });
        }
        
        int GetWidth() const { return width; }
    };
}
