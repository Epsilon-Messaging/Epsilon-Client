#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

namespace Epsilon {
    ftxui::Component CreateInputPanel(std::string* content) {
        auto input = ftxui::Input(content);
        
        return ftxui::Renderer(input, [input] {
            return input->Render() | ftxui::flex | ftxui::border;
        });
    }
}

