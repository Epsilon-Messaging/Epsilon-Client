#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

namespace Epsilon {
    class InputPanel {
    private:
        std::string* content;
        int height;
        
    public:
        InputPanel(std::string* input_content) : content(input_content), height(5) {}
        
        ftxui::Component Create() {
            auto input = ftxui::Input(content);
            
            return ftxui::Renderer(input, [input] {
                return input->Render() | ftxui::flex | ftxui::border;
            });
        }
        
        int GetHeight() const { return height; }
    };
}
