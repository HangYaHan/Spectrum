#ifndef SYSTEM_H
#define SYSTEM_H
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <windows.h>
#include <dirent.h>
#include "Settings.h"

class System {
    public:
        const std::string original_file_path = ORGINAL_FILE_PATH;
        const std::string data_file_path = DATA_FILE_PATH;
        const std::string database_path = DATABASE_PATH;

        int n_Specs;
        std::vector<std::string> file_names;

        std::vector<std::string> names;

        System();
        void print_menu();
        int write_ini();
        int read_ini();

        std::vector<std::string> get_orginal_names();
        int preprocess_cutoff(std::vector<std::string> file_names);
        int rm_two_line(std::string file_name);
};

#endif