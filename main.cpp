#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Spectrum.h"
#include "System.h"
#include "Spectrum.cpp"
#include "System.cpp"
#include "Settings.h"

// 1. class definition need to be optimized
// 2. peak position should be recorded speraterly both in index && wavelength
// 3. sort agrithom is bubble sort, which is very bad
// 4. it can not be interacted
// 5. read data is probably inefficient
// 6. the read_spectrum sucks ***

#if 0
int main() {
    SpecSeries organic(NUM_OF_ORGANIC_MATERIAL, "Organic Materials", "25 organic materials");
    Spectrum tmp;

    for (int i = 0; i < NUM_OF_ORGANIC_MATERIAL; i++) {
        read_spectrum(FILE_PATH + std::to_string(i + INDEX_OFFSET_ + 1) + std::string(".txt"), tmp);
        organic.set_spectrum(i, tmp);
        organic.series[i].name = "ID " + std::to_string(i + 1 + INDEX_OFFSET_);
    }

    //organic.cut_off();
    organic.show_info();
    organic.sort_by_first_peak();
    organic.peak_info();
    organic.normalize();
    organic.to_csv("organic.csv");
    std::cout << "Done!" << std::endl;
    return 0;
}

#else
int main(){
    System sys;
    int choice;
    //sys.preprocess_cutoff(sys.get_orginal_names());
}  
#endif
