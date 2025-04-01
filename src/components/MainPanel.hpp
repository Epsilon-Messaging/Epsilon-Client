#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace Epsilon {
    class MainPanel {
    private:
        std::string title;
        int width;
        std::vector<std::string> logs;
        int selected_index = 0;

    public:
        MainPanel() : title("Messages"), width(30), logs({
            "[03/28/2025 17:24] Tiqur: Hello",
            "[03/28/2025 17:25] Peaches_MLG: Hi",
            "[03/28/2025 17:25] Tiqur: Check this out https://www.youtube.com/watch?v=dQw4w9WgXcQ",
            "[03/28/2025 17:26] Peaches_MLG: >:C",
        }) {}

        ftxui::Component Create() {
            std::vector<ftxui::Element> log_elements;
            for (const auto& log : logs) {
                log_elements.push_back(ftxui::paragraph(log));
            }

            auto log_list = ftxui::vbox(log_elements);

            return ftxui::Renderer([log_list] {
                return ftxui::vbox({
                    ftxui::text("Messages") | ftxui::center | ftxui::bold,
                    log_list
                }) | ftxui::border;
            });
        }

        int GetWidth() const { return width; }
    };
}
