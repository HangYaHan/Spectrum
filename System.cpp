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
    this->read_ini();
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
    Spectrums.resize(file_names.size());
    preprocess_cutoff(file_names);
    for (int i = 0; i < file_names.size(); i++) {
        Spectrums[i].read_spectrum(std::string(DATA_FILE_PATH) + "/" + file_names[i]);
        //spectrums[i].show_info();
    }
    std::cout << "Read " << file_names.size() << " files complete." << std::endl;
    return ERR_SUCCESS;
}

int System::read_csv(){
    return ERR_SUCCESS;
}

int System::show_info(){
    return ERR_SUCCESS;
}

int System::show_detail(){
    return ERR_SUCCESS;
}

int System::save_csv(){
    std::ofstream out_file("database/data.csv");
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
        out_file << Spectrums[i].name
    }
    return ERR_SUCCESS;
}

