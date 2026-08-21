#include <ftxui/component/component.hpp>       // для кнопок, меню и интерактивности
#include <ftxui/component/screen_interactive.hpp> // для запуска интерактивного экрана
#include <ftxui/dom/elements.hpp>              // для верстки (рамки, текст, цвета)
#include "task.hpp"

int main() {
    TaskManager tm;
    tm.loadTasks("tasks.txt");

    std::vector<std::string> menu_items;
    std::vector<std::string> contents;

    for (const Task &t : tm.tasks){
        menu_items.push_back(t.name);
        contents.push_back(t.description);
    }

    int selected_index = 0;
    int current_scene = 0; // main, add tasks, ...

    std::string input_name = "";
    std::string input_description = "";

    auto screen = ftxui::ScreenInteractive::TerminalOutput();

    auto menu = ftxui::Menu(&menu_items, &selected_index);

    auto btn_go_to_add = ftxui::Button("add task", [&] {
        input_name = "";
        input_description = "";
        current_scene = 1;
    });

    auto btn_remove_task = ftxui::Button("remove task", [&] {
        if (tm.tasks.empty()) return;
        tm.tasks.erase(tm.tasks.begin() + selected_index);
        tm.saveTasks("tasks.txt");
        tm.loadTasks("tasks.txt");

        menu_items.clear();
        contents.clear();
        for (const Task &t : tm.tasks){
            menu_items.push_back(t.name);
            contents.push_back(t.description);
        }
    });

    auto left_panel = ftxui::Container::Vertical({
        ftxui::Container::Horizontal({btn_go_to_add | ftxui::flex, btn_remove_task | ftxui::flex}),
        menu
    });

    auto scene_main = ftxui::Renderer(left_panel, [&] {
        ftxui::Element content_view = ftxui::text("list is empty");
        if (!menu_items.empty() && static_cast<size_t>(selected_index) < menu_items.size()){
            content_view = ftxui::vbox({
                ftxui::paragraph(contents[selected_index])
            });
        }

        return ftxui::hbox({
            window(ftxui::text(" tasks "), left_panel->Render()) | ftxui::flex,

            ftxui::separator(),

            window(ftxui::text(" description "), content_view) | ftxui::flex
        });
    });

    // ==========================================
    // СЦЕНА 1: Экран добавления элемента
    // ==========================================
    auto input_title_comp = ftxui::Input(&input_name, "task name");
    auto input_content_comp = ftxui::Input(&input_description, "task description");

    auto btn_save = ftxui::Button("save and return", [&] {
        if (!input_name.empty()) {
            tm.tasks.push_back({input_name, input_description.empty() ? "<empty>" : input_description});

            selected_index = menu_items.size() - 1;
            tm.saveTasks("tasks.txt");
            tm.loadTasks("tasks.txt");

            menu_items.clear();
            contents.clear();
            for (const Task &t : tm.tasks){
                menu_items.push_back(t.name);
                contents.push_back(t.description);
            }
        }
        current_scene = 0;
    });

    auto btn_cancel = ftxui::Button("cancel", [&] {
        current_scene = 0;
    });

    auto add_form_container = ftxui::Container::Vertical({
        input_title_comp,
        input_content_comp,
        ftxui::Container::Horizontal({btn_save, btn_cancel})
    });

    auto scene_add = ftxui::CatchEvent(ftxui::Renderer(add_form_container, [&] {
        return ftxui::center(
            ftxui::window(ftxui::text(" adding new task "),
                ftxui::vbox({
                    ftxui::text("name:"),
                    input_title_comp->Render() | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 40),
                    ftxui::vbox(),
                    ftxui::text("description:"),
                    input_content_comp->Render() | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 60),
                    ftxui::vbox(),

                    add_form_container->ChildAt(2)->Render()
                })
            )
        );
    }), [&](ftxui::Event event) {
        return add_form_container->OnEvent(event);
    });


    // ==========================================
    // МЕНЕДЖЕР СЦЕН (Tab)
    // ==========================================
    auto main_tabs = ftxui::Container::Tab({scene_main, scene_add}, &current_scene);

    screen.Loop(main_tabs);

    return 0;
}
