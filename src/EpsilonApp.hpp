#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "components/HeaderPanel.hpp"
#include "components/LeftPanel.hpp"
#include "components/RightPanel.hpp"
#include "components/MainPanel.hpp"
#include "components/InputPanel.hpp"

namespace Epsilon {
    class EpsilonApp {
    private:
        std::string input_content;
        HeaderPanel header_panel;
        LeftPanel left_panel;
        RightPanel right_panel;
        MainPanel main_panel;
        InputPanel input_panel;
        ftxui::ScreenInteractive screen;
        
    public:
        EpsilonApp() 
            : input_panel(&input_content),
              screen(ftxui::ScreenInteractive::Fullscreen()) {}
        
        void Run() {
            auto header_component = header_panel.Create();
            auto left_component = left_panel.Create();
            auto right_component = right_panel.Create();
            auto main_component = main_panel.Create();
            auto input_component = input_panel.Create();
            
            int left_size = left_panel.GetWidth();
            int right_size = right_panel.GetWidth();
            int top_size = header_panel.GetHeight();
            int input_height = input_panel.GetHeight();
            
            auto main_with_input = ftxui::Container::Vertical({
                main_component | ftxui::flex,
                input_component | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, input_height)
            });
            
            auto container = main_with_input;
            container = ftxui::ResizableSplitLeft(left_component, container, &left_size);
            container = ftxui::ResizableSplitRight(right_component, container, &right_size);
            container = ftxui::ResizableSplitTop(header_component, container, &top_size);
            
            auto renderer = ftxui::Renderer(container, [&] { 
                return container->Render() | ftxui::border; 
            });
            
            screen.Loop(renderer);
        }
    };
}
