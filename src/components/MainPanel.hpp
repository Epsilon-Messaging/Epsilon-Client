#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include "InputPanel.hpp"
#include "../Message.hpp"

auto timestamp_to_string(unsigned int timestamp) {
    std::chrono::seconds sec(timestamp);
    std::chrono::system_clock::time_point tp(sec);
    
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&time);
    
    std::stringstream ss;
    ss << "[" << std::put_time(&tm, "%m/%d/%y %H:%M") << "]";
    return ss.str();
}

namespace Epsilon {
    class MainPanel {
    private:
        std::string title;
        int width;
        std::vector<Message> messages;
        int selected_index = 0;
        std::string input_content;
        InputPanel input_panel;
        
        ftxui::Component message_component;
        ftxui::Component main_container;

    public:
        MainPanel() : title("Messages"), width(30), messages({
            { 1743480424, "Tiqur", "Hello"},
            { 1743480429, "Peaches_MLG", "Hi"},
            { 1743480434, "Tiqur", "Check this out https://www.youtube.com/watch?v=dQw4w9WgXcQ"},
            { 1743480437, "Peaches_MLG", "Ok"},
            { 1743480443, "Peaches_MLG", ">:C"},
        }), input_panel(&input_content) {}

        ftxui::Component Create() {
            auto input_component = input_panel.Create();
            int input_height = input_panel.GetHeight();

            std::vector<ftxui::Element> message_elements;
            for (const auto& message : messages) {
                auto date = ftxui::text(timestamp_to_string(message.timestamp)) | ftxui::color(ftxui::Color::GrayDark);
                auto sender_name = ftxui::text(message.sender_name) | 
                    (message.sender_name == messages[0].sender_name ? 
                     ftxui::color(ftxui::Color::Green) : ftxui::color(ftxui::Color::Blue));
                auto message_content = ftxui::text(message.content) | ftxui::color(ftxui::Color::White);

                auto combined_message = ftxui::hbox({
                    date,
                    ftxui::text(" "),
                    sender_name,
                    ftxui::text(": "),
                    message_content
                });
                message_elements.push_back(combined_message);
            }

            message_component = ftxui::Renderer([=] {
                return ftxui::vbox({
                    ftxui::text("Messages") | ftxui::center | ftxui::bold,
                    ftxui::vbox(message_elements) | 
                        ftxui::frame | 
                        ftxui::vscroll_indicator | 
                        ftxui::flex
                });
            });

            main_container = ftxui::Container::Vertical({
                message_component,
                input_component
            });

            return ftxui::Renderer(main_container, [=, this] {
                return ftxui::vbox({
                    message_component->Render() | ftxui::flex,
                    input_component->Render() | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 3)
                });
            });
        }

        int GetWidth() const { return width; }
    };
}
