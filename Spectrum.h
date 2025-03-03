#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Settings.h"


class Spectrum {
    public:
        int id; //the name of file
        int first_peak_pos;
        int FWHM_first_peak; // real value
        std::vector<float> data;
        int step;

        Spectrum();
        int read_spectrum(const std::string& file_name);
        void judge_step();

        void show_data() const;
        void show_info() const;
        void find_peak();
        void normalize();

        // int compare();
        // void debug_print(Spectrum spectrum); 
};

#endif // SPECTRUM_H
