#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace Epsilon {
    class LeftPanel {
    private:
        std::string title;
        int width;
        std::vector<std::string> usernames;
        int selected_index = 0;

    public:
        LeftPanel() : title("Direct Messages"), width(30), usernames({
            "User1",
            "User2",
            "User3",
            "User4",
            "User5",
            "User6",
            "User7",
            "User8"
        }) {}

        ftxui::Component Create() {
            auto user_list = ftxui::Menu(&usernames, &selected_index);

            return ftxui::Container::Vertical({
                ftxui::Renderer([] { return ftxui::text("Direct Messages") | ftxui::center | ftxui::bold; }),
                user_list
            }) | ftxui::yflex_shrink;
        }

        int GetWidth() const { return width; }
    };
}

