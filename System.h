#ifndef SYSTEM_H
#define SYSTEM_H
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <windows.h>
#include <dirent.h>
#include <array>
#include "Settings.h"
#include "Spectrum.h"



class System {
    public:
        int n_Specs;
        std::vector<std::string> file_names;
        std::vector<Spectrum> Spectrums;

        System();
        void print_menu();
        int write_ini();
        int read_ini();

        std::vector<std::string> get_orginal_names();
        int preprocess_cutoff(std::vector<std::string> file_names);
        int rm_two_line(std::string file_name);

        int read_spectrum();
        int read_csv();
        int show_info();
        int show_detail();
        int save_csv();
};

#endif // SYSTEM_H