#include "Spectrum.h"

Spectrum::Spectrum()
    : id(-1), first_peak_pos(-1), FWHM_first_peak(-1), step(-1) {
        data.clear();
    }
    

int Spectrum::read_spectrum(const std::string& file_name) {
    std::ifstream file(file_name);
    std::string line;
    int comma_pos;
    int count;

    data.clear();
    id = std::stoi(file_name.substr(file_name.find_last_of('/') + 1, file_name.find_last_of('.') - file_name.find_last_of('/') - 1));
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << file_name << std::endl;
        return ERR_FILE;
    }

    while (std::getline(file, line)) {
        comma_pos = line.find(',');
        if (comma_pos != std::string::npos) {
            data.push_back(std::stof(line.substr(comma_pos + 1)));
            count++;
        }
    }

    // pre-process
    judge_step();
    find_peak();
    normalize();

    file.close();

    return ERR_SUCCESS;
}

void Spectrum::show_data() const {
    for (int i=0; i<data.size(); i++) {
        std::cout << "The data at " << START_WL + i * step << " is " << data[i] << std::endl;
    }
}

void Spectrum::show_info() const {
    std::cout << "---------Spectrum Info---------" << std::endl;
    std::cout << "ID:" << id << std::endl;
    std::cout << "The size of the data is " << data.size() << std::endl;
    std::cout << "The step is " << step << " nm" << std::endl;
    std::cout << "The first peak position is " << first_peak_pos*step + START_WL << " nm" << std::endl;
    std::cout << "The FWHM of the first peak is " << FWHM_first_peak << " nm" << std::endl;
    std::cout << "-------------------------------" << std::endl;
}

void Spectrum::judge_step() {
    if (data.size() > 0) {
        step = (END_WL - START_WL) / (data.size() - 1);
    }
}

void Spectrum::find_peak() {
    float max_value = 0;
    int left = 0;
    int right = 0;

    for (int i = 0; i < data.size(); i++) {
        if (data[i] > max_value) {
            max_value = data[i];
            first_peak_pos = i;
        }
    }

    for (int i = first_peak_pos; i < data.size(); i++) {
        if (data[i] < max_value / 2) {
            right = i - 1;
            break;
        }
    }

    for (int i = first_peak_pos; i >= 0; i--) {
        if (data[i] < max_value / 2) {
            left = i + 1;
            break;
        }
    }

    FWHM_first_peak = (right - left) * step;
    return ;
}

void Spectrum::normalize() {
    float max_value = data[first_peak_pos];
    for (auto& value : data) {
        value /= max_value;
    }
}

