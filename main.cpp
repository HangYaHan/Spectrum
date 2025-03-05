#include "Spectrum.h"
#include "System.h"
#include "Settings.h"


// 2. peak position should be recorded speraterly both in index && wavelength
// 3. sort agrithom is bubble sort, which is very bad
// 4. it can not be interacted
// 5. read data is probably inefficient
// 6. the read_spectrum sucks ***

#if 0
int main() {
    System sys;
    int choice;

    while(1){
        sys.print_menu();
        std::cout << "Please select the operation: ";
        std::cin >> choice;
        switch(choice)
        {
            case 1:
                sys.read_spectrum();
                break;
            case 2:
                sys.read_csv();
                break;
            case 3:
                sys.show_info();
                break;
            case 4:
                sys.show_detail();
                break;
            case 5:
                sys.save_csv();
                break;
            case 6:
                return 0;
            default:
                std::cout << "Invalid choice!" << std::endl;
                break;
        }

        system("cls");
    }   
    return 0;
}

#elif 0
int main(){
    std::cout << std::endl;
    //System sys;
    int choice;
    //sys.preprocess_cutoff(sys.get_orginal_names());
    Spectrum tmp;
    tmp.read_spectrum(DATA_FILE_PATH + std::string("/26.txt"));
    tmp.show_info();
    std::cout << "That's the end!" << std::endl; 
}  

#else
int main(){
    System sys;
    sys.read_csv();
    sys.show_info();


    sys.read_spectrum();
    sys.show_info();

    
    sys.save_csv();
    return 0;
}
#endif
