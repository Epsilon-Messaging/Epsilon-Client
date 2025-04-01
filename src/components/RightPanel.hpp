#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <vector>
#include <string>

namespace Epsilon {
    class RightPanel {
    private:
        std::string title;
        int width;
        std::vector<std::string> logs;
        int selected_index = 0;

    public:
        RightPanel() : title("Direct Messages"), width(30), logs({
            "[INFO] Connecting to server...",
            "[INFO] Successfully connected to server.",
            "[INFO] Encrypting message 719047",
            "[INFO] Signing message 719047",
            "[INFO] Sending message 719047 to 23417098",
            "[INFO] Received message from 23417098",
        }) {}

        ftxui::Component Create() {
            std::vector<ftxui::Element> log_elements;
            for (const auto& log : logs) {
                log_elements.push_back(ftxui::paragraph(log));
            }

            auto log_list = ftxui::vbox(log_elements);

            return ftxui::Renderer([log_list] {
                return ftxui::vbox({
                    ftxui::text("Logs") | ftxui::center | ftxui::bold,
                    log_list
                }) | ftxui::border;
            });
        }

        int GetWidth() const { return width; }
    };
}

