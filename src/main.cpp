#include <ftxui/component/component.hpp>       // для кнопок, меню и интерактивности
#include <ftxui/component/screen_interactive.hpp> // для запуска интерактивного экрана
#include <ftxui/dom/elements.hpp>              // для верстки (рамки, текст, цвета)
#include "task.hpp"

int main() {
    TaskManager tm;
    tm.tasks.push_back({"first", "this is first test task"});
    tm.tasks.push_back({"second", "this is second test task"});
    tm.tasks.push_back({"third", "this is third test task"});


    std::vector<std::string> menu_items;
    std::vector<std::string> contents;

    for (const Task &t : tm.tasks){
        menu_items.push_back(t.name);
        contents.push_back(t.description);
    }

    int selected_index = 0;

    auto menu = ftxui::Menu(&menu_items, &selected_index);

    auto renderer = ftxui::Renderer(menu, [&] {
        return ftxui::hbox({
            window(ftxui::text(" tasks "), menu->Render()) | ftxui::flex,

            ftxui::separator(),

            window(ftxui::text(" description "),
                   ftxui::vbox({
                       ftxui::paragraph(contents[selected_index])
                   })
            ) | ftxui::flex
        });
    });

    auto screen = ftxui::ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);

    return 0;
}
