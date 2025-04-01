#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace Epsilon {
    class HeaderPanel {
    private:
        std::string title;
        int height;
        
    public:
        HeaderPanel() : title("Epsilon"), height(1) {}
        
        ftxui::Component Create() {
            return ftxui::Renderer([] { 
                return ftxui::text("Epsilon") | ftxui::center; 
            });
        }
        
        int GetHeight() const { return height; }
    };
}
