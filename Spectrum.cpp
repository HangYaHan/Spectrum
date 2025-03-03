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

// void read_spectrum(const std::string& file_name, Spectrum& spectrum) {
//     std::ifstream file(file_name);
//     std::string line;
//     size_t comma_pos;
//     int position;
//     float value;

//     spectrum.data.clear();

//     std::getline(file, line);
//     std::getline(file, line);

//     std::getline(file, line);
//     comma_pos = line.find(',');
//     if (comma_pos != std::string::npos) {
//         position = std::stof(line.substr(0, comma_pos));
//         value = std::stof(line.substr(comma_pos + 1));
//         spectrum.data.push_back(value);
//     }
//     spectrum.start_wl = position;

//     while (std::getline(file, line)) {
//         comma_pos = line.find(',');
//         if (comma_pos != std::string::npos) {
//             value = std::stof(line.substr(comma_pos + 1));
//             spectrum.data.push_back(value);
//         }
//         position += 2;
//     }

//     spectrum.end_wl = position;
//     file.close();
// }

// void Spectrum::debug_print(Spectrum spectrum)
// {
//     std::cout << "The first five data is" << std::endl;
//     for (int i = 0; i < 5; i++) {
//         std::cout << spectrum.data[i] << std::endl;
//     }
//     std::cout << "The last five data is" << std::endl;
//     for (int i = spectrum.data.size() - 5; i < spectrum.data.size(); i++) {
//         std::cout << spectrum.data[i] << std::endl;
//     }
//     std::cout << "The size of the data is " << spectrum.data.size() << std::endl;
//     std::cout << "The start wavelength is " << spectrum.start_wl << std::endl;
//     std::cout << "The end wavelength is " << spectrum.end_wl << std::endl;
//     std::cout << "The start offset is " << spectrum.start_offset << std::endl;
//     std::cout << "The step offset is " << spectrum.step_offset << std::endl;
//     std::cout << "The first peak position is " << spectrum.first_peak_pos << std::endl;
//     return ;
// }

// SpecSeries::SpecSeries(int n, std::string name, std::string description)
//     : n(n), name(name), description(description){
//         series.resize(n);
//     }
    
// void SpecSeries::show_info(){
//     for (int i = 0; i < n; i++) {
//         std::cout << "The size of Material " << i + 1 << " is ";
//         series[i].show_size();
//     }
// }

// void SpecSeries::set_spectrum(int index, const Spectrum& spectrum) {
//     if (index >= 0 && index < n) {
//         series[index] = spectrum;
//     }
// }

// void SpecSeries::to_csv(std::string file_name){
//     std::ofstream file(file_name);
//     file << "Wavelength" << ",";
//     for(int i = START_WL_need_upgrade; i < END_WL_need_upgrade; i+=2) 
//         file << i << ",";
//     file << std::endl;
//     for (int i = 0; i < series.size(); i++) {
//         file << series[i].name << ",";
//         for (int j = 0; j < series[i].data.size(); j++) {
//             file << series[i].data[j] << ",";
//         }
//         file << std::endl;
//     }
// }

// void SpecSeries::sort_by_first_peak(){
//     for (int i = 0; i < n; i++) {
//         series[i].find_peaks();
//     }

//     for (int i = 0; i < n - 1; i++) {
//         for (int j = i + 1; j < n; j++) {
//             if (series[i].first_peak_pos > series[j].first_peak_pos) {
//                 Spectrum tmp = series[i];
//                 series[i] = series[j];
//                 series[j] = tmp;
//             }
//         }
//     }
// }

// void SpecSeries::peak_info(int level = 1){
//     if (level == 1) {
//         for (int i = 0; i < n; i++) {
//             std::cout << "Material " << series[i].name << " has first peak at " << series[i].first_peak_pos << std::endl;
//         }
//     } else if (level == 2) {
//         for (int i = 0; i < n; i++) {
//             std::cout << "Material " << series[i].name << " has second peak at " << series[i].second_peak_pos << std::endl;
//         }
//     } else {
//         std::cout << "Invalid level!" << std::endl;
//     }
// }

// void SpecSeries::normalize(){
//     for (int i = 0; i < n; i++) {
//         series[i].normalize();
//     }
// }
