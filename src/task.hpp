#pragma once

#include <fstream>
#include <string>
#include <vector>

struct Task{
    std::string name;
    std::string description;
};

class TaskManager{
    private:
        void cleanString(std::string &s){
            std::string to_find = "\n";
            std::string to_replace = " \\ ";

            size_t pos = 0;
            while ((pos = s.find(to_find, pos)) != std::string::npos) {
                s.replace(pos, to_find.length(), to_replace);

                // Move index past the replacement to avoid infinite loops
                pos += to_replace.length();
            }
        }
    public:
        std::vector<Task> tasks;
        bool saveTasks(std::string path){
            std::ofstream out_file(path);

            if (!out_file.is_open()) return false;

            out_file.clear();

            for (auto& task : tasks){
                cleanString(task.name);
                cleanString(task.description);
                out_file << task.name << "\n";
                out_file << task.description << "\n";
            }

            out_file.close();
            return true;
        }
        bool loadTasks(std::string path){
            std::ifstream in_file(path);

            if (!in_file.is_open()) return false;

            tasks.clear();

            std::string name_line;
            std::string description_line;

            while (
                std::getline(in_file, name_line) &&
                std::getline(in_file, description_line)
            ){
                Task tmp_task;
                tmp_task.name = name_line;
                tmp_task.description = description_line;

                tasks.push_back(tmp_task);
            }

            in_file.close();
            return true;
        }
};
