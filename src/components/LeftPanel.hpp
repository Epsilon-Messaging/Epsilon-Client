#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace Epsilon {
    class LeftPanel {
    private:
        std::string title;
        int width;
        std::vector<std::string> conversations;
        int selected_index = 0;

    public:
        LeftPanel() : title("Conversations"), width(30), conversations({
            "Peaches_MLG",
            "Cool People",
            "Secret Group",
            }) {
        }

        ftxui::Component Create() {
            auto conversation_list = ftxui::Menu(&conversations, &selected_index);

            return ftxui::Container::Vertical({
                ftxui::Renderer([] { return ftxui::text("Converations") | ftxui::center | ftxui::bold; }),
                conversation_list
            }) | ftxui::yflex_shrink;
        }

        int GetWidth() const { return width; }
    };
}

