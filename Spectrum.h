#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Spectrum.h"
#include "Settings.h"


class Spectrum {
    public:
        int id;
        int first_peak_pos;
        int second_peak_pos;
        int start_wl;
        int end_wl;
        int start_offset;
        int step_offset;
        std::string name;
        std::vector<float> data;

        Spectrum();
        void show_data() const;
        void show_size() const;
        void find_peaks();
        void normalize();
        void debug_print(Spectrum spectrum); 
};

void read_spectrum(const std::string& file_name, Spectrum& spectrum);

class SpecSeries{
    public:
        int n; // number of materials in the series
        std::string name; 
        std::string description; 
        std::vector<Spectrum> series;

        SpecSeries(int n, std::string name, std::string description);
        void show_info();
        void set_spectrum(int index, const Spectrum& spectrum);
        void to_csv(std::string file_name);
        void sort_by_first_peak();
        void peak_info(int level);
        void normalize();
};

#endif // SPECTRUM_H
