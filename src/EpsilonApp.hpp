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
        HeaderPanel header_panel;
        LeftPanel left_panel;
        RightPanel right_panel;
        MainPanel main_panel;
        ftxui::ScreenInteractive screen;
        
    public:
        EpsilonApp() 
            : screen(ftxui::ScreenInteractive::Fullscreen()) {}
        
        void Run() {
            auto header_component = header_panel.Create();
            auto left_component = left_panel.Create();
            auto right_component = right_panel.Create();
            auto main_component = main_panel.Create();
            
            int left_size = left_panel.GetWidth();
            int right_size = right_panel.GetWidth();
            int top_size = header_panel.GetHeight();
            
            auto container = main_component;
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
