#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace Epsilon {
    class HeaderPanel {
    private:
        std::string title;
        int height;
        
    public:
        HeaderPanel() : title("Epsilon - User1"), height(1) {}
        
        ftxui::Component Create() {
            return ftxui::Renderer([] { 
                return ftxui::text("Epsilon - User1") | ftxui::center; 
            });
        }
        
        int GetHeight() const { return height; }
    };
}
