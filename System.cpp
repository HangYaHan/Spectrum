#include "System.h"

void System::print_menu(){
    std::cout << "Welcome to the spectrum operator!" << std::endl;
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Please select the operation." << std::endl;
    std::cout << "1. Read the spectrum data from new files." << std::endl;
    std::cout << "2. Read the spectrum data from database." << std::endl;
    std::cout << "3. Show the spectrum info." << std::endl;
    std::cout << "4. Show the detail of a certain spectrum." << std::endl;
    std::cout << "5. Save the spectrum data to a csv file." << std::endl;
    std::cout << "----------------------------------" << std::endl;
    std::cout << "6. Exit." << std::endl;
    return ;
}

std::vector<std::string> System::get_orginal_names() {
    std::vector<std::string> fileNames;
    DIR* dir;
    struct dirent* ent;

    // Open directory
    if ((dir = opendir(ORGINAL_FILE_PATH)) != NULL) {
        // Read all files in the directory
        while ((ent = readdir(dir)) != NULL) {
            // Skip current directory (".") and parent directory ("..")
            if (std::string(ent->d_name) != "." && std::string(ent->d_name) != "..") {
                fileNames.push_back(ent->d_name);
            }
        }
        // Close directory
        closedir(dir);
    } else {
        // If unable to open directory, output error message
        std::cerr << "Unable to open directory: " << ORGINAL_FILE_PATH << std::endl;
    }
    return fileNames;
}

int System::write_ini(){
    std::ofstream ini_file("system.ini");
    if (!ini_file) {
        std::cerr << "Error opening system.ini for writing." << std::endl;
        return ERR_FILE;
    }

    ini_file << "Number of Spectrums : " << n_Specs << std::endl;

    ini_file.close();
    if (!ini_file) {
        std::cerr << "Error closing system.ini file." << std::endl;
        return ERR_FILE;
    }
    return ERR_SUCCESS;
}

int System::read_ini(){
    std::ifstream ini_file("system.ini");
    if (!ini_file) {
        std::cerr << "Error opening system.ini for reading." << std::endl;
        std::cout << "Creating a new system.ini file..." << std::endl;
        write_ini();
        return ERR_FILE;
    }

    std::string line;
    std::getline(ini_file, line);
    n_Specs = std::stoi(line.substr(line.find(":") + 1));

    ini_file.close();
    if (!ini_file) {
        std::cerr << "Error closing system.ini file." << std::endl;
        return ERR_FILE;
    }
    return ERR_SUCCESS;
}

System::System(){
    n_Specs = 0;
    std::cout << "System is initializing..." << std::endl;
    std::cout << n_Specs << std::endl;
    std::cout << "System is ready!" << std::endl;
    return ;
}

int System::preprocess_cutoff(std::vector<std::string> file_names){
    for (std::vector<std::string>::iterator it = file_names.begin(); it != file_names.end(); it++) {
            rm_two_line(*it);
    }
    return ERR_SUCCESS;
}

int System::rm_two_line(std::string file_name){
    std::ifstream in_file(std::string(ORGINAL_FILE_PATH) + "/" + file_name);
    if (!in_file) {
        std::cerr << "Error opening input file: " << std::string(ORGINAL_FILE_PATH) + "/" + file_name << std::endl;
        return ERR_FILE;
    }
    std::ofstream out_file(std::string(DATA_FILE_PATH) + "/" + file_name);
    if (!out_file) {
        std::cerr << "Error opening output file: " << std::string(DATA_FILE_PATH) + "/" + file_name << std::endl;
        return ERR_FILE;
    }
    std::string line;
    int count = 0;
    while (std::getline(in_file, line)) {
        if (count > 1) {
            out_file << line << std::endl;
        }
        count++;
    }
    in_file.close();
    out_file.close();
    return ERR_SUCCESS;
}

int System::read_spectrum(){
    std::vector<std::string> file_names = get_orginal_names();
    n_Specs += file_names.size();
    Spectrums.resize(n_Specs);
    preprocess_cutoff(file_names);  //cut off the first two lines of the original files
    for (int i = 0; i < file_names.size(); i++) {
        Spectrums[i].read_spectrum(std::string(DATA_FILE_PATH) + "/" + file_names[i]);
        //spectrums[i].show_info();
    }
    std::cout << "Read " << file_names.size() << " files complete." << std::endl;
    return ERR_SUCCESS;
}

int System::read_csv(){
    std::ifstream in_file("database/data.csv");
    if (!in_file) {
        std::cerr << "Error opening input file: database/data.csv" << std::endl;
        return ERR_FILE;
    }

    std::string line;
    std::getline(in_file, line);
    std::vector<int> ids;

    for (int i = 0; i < line.size(); i++) {
        if (line[i] == ',') {
            ids.push_back(std::stoi(line.substr(0, i)));
            line = line.substr(i + 1);
            i = 0;
        }
    }

    n_Specs += ids.size();
    Spectrums.resize(n_Specs);

    for (int i=0; i<ids.size(); i++) {
        Spectrums[i].id = ids[i];
    }

    for (int i = 0; i < (END_WL-START_WL)/STEP_WL; i++) {
        std::getline(in_file, line);
        for (int j = 0; j < ids.size(); j++) {
            Spectrums[j].data.push_back(std::stof(line.substr(0, line.find(','))));
            line = line.substr(line.find(',') + 1);
        }
    }

    for (int i = 0; i < n_Specs; i++) {
        Spectrums[i].find_peak();
        Spectrums[i].judge_step();
        Spectrums[i].normalize();
    }

    in_file.close();
    if (!in_file) {
        std::cerr << "Error closing input file: database/data.csv" << std::endl;
        return ERR_FILE;
    }

    std::cout << "----------------------------------" << std::endl;
    std::cout << "Read data from /database/data.csv complete." << std::endl;
    std::cout << "Total " << n_Specs << " spectrums read." << std::endl;
    std::cout << "----------------------------------" << std::endl;
    return ERR_SUCCESS;
}

int System::show_info(){
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Total " << n_Specs << " spectrums." << std::endl;
    std::cout << "----------------------------------" << std::endl;
    return ERR_SUCCESS;
}

int System::show_detail(){
    return ERR_SUCCESS;
}

int System::save_csv(){
    std::ofstream out_file("database/data1.csv");
    if (!out_file) {
        std::cerr << "Error opening output file: database/data.csv" << std::endl;
        std::cout << "Creating a new database/data.csv file..." << std::endl;
        out_file.open("database/data.csv");
        if (!out_file) {
            std::cerr << "Error opening output file: database/data.csv" << std::endl;
            return ERR_FILE;
        }
        return ERR_FILE;
    }

    for (int i = 0; i < Spectrums.size(); i++) {
        out_file << Spectrums[i].id << ",";
    }

    out_file << std::endl;

    for (int i = 0; i < (END_WL-START_WL)/STEP_WL; i++) { 
        for (int j = 0; j < Spectrums.size(); j++) {
            out_file << Spectrums[j].data[i] << ",";
        }
        out_file << std::endl;
    }

    std::cout << "----------------------------------" << std::endl;
    std::cout << "Save data to /database/data.csv complete." << std::endl;
    std::cout << "Total " << n_Specs << " spectrums saved. " << std::endl;
    std::cout << "----------------------------------" << std::endl;
    return ERR_SUCCESS;
}

