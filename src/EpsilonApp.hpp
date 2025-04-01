#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <vector>
#include "User.hpp"
#include "Message.hpp"
#include "components/HeaderPanel.hpp"
#include "components/LeftPanel.hpp"
#include "components/RightPanel.hpp"
#include "components/MainPanel.hpp"
#include "components/InputPanel.hpp"
#include "GroupMessage.hpp"
#include "DirectMessage.hpp"

namespace Epsilon {
    class EpsilonApp {
    private:
        HeaderPanel header_panel;
        LeftPanel left_panel;
        RightPanel right_panel;
        MainPanel main_panel;
        ftxui::ScreenInteractive screen;

        std::vector<GroupMessage> group_messages;
        std::vector<DirectMessage> direct_messages;

    public:
        EpsilonApp() : screen(ftxui::ScreenInteractive::Fullscreen()) {
            User tiqur = {"Tiqur", "publickey", 1};
            User peaches = {"Peaches_MLG", "publickey", 2};
            User alice = {"Alice", "publickey", 3};
            User bob = {"Bob", "publickey", 4};

            std::vector<Message> dm_messages = {
                {1743480424, "Tiqur", "Hello"},
                {1743480429, "Peaches_MLG", "Hi"},
                {1743480434, "Tiqur", "Check this out https://www.youtube.com/watch?v=dQw4w9WgXcQ"},
                {1743480437, "Peaches_MLG", "Ok"},
                {1743480443, "Peaches_MLG", ">:C"}
            };

            // Create DirectMessage
            direct_messages.push_back(DirectMessage(1, tiqur, peaches, dm_messages));
            direct_messages[0].printDirectMessageDetails();

            // Create a GroupMessage
            std::vector<User> group_users = {tiqur, peaches, alice, bob};
            group_messages.push_back(GroupMessage(2, "Group Chat: Dev Team", tiqur.id, group_users, {}));
            group_messages[0].printGroupMessageDetails();
        }

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

