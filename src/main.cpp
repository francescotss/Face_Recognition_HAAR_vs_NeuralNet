#include "main.h"

void create_dataset_menu(){
    cout << "Minimum images per person: ";
    short entry;
    cin >> entry;
    create_csv(entry);
}

void train_neural(){
    cout << "Loading..." << endl;
    cout.flush();
    system("pipenv run python ../src/neural_network.py");
}

int main(int argc, const char* argv[]) {

    GlobalConfig::init();

    bool quit = false;
    while (!quit){
        cout << "Welcome in HAAR Cascade and Neural Network comparator for face recognition \nBy Francesco Riccardo Tassone" << endl;
        cout << "For reference check Readme.txt (Italian)" << endl;
        cout << "Choose Option:\n"
                "1 - Create faces folder and analyze\n"
                "2 - Create dataset (You have to run [1] at least once)\n"
                "3 - Train HAAR model\n"
                "4 - Train Neural Network model\n"
                "5 - Test HAAR model\n"
                "6 - Test Neural Network model\n"
                "0 - Quit\n"
                "Select: ";



        int entry;
        cin >> entry;
        switch (entry) {
            case 1:
                create_dataset();
                break;
            case 2:
                create_dataset_menu();
                break;
            case 3:
                train_recognizer();
                break;
            case 4:
                train_neural();
                break;
            case 5:
                test_recognizer();
                break;
            case 6:
                //TODO
                break;
            case 0:
                quit = true;
                cout << "Bye" << endl;
                break;
        }
    }

    return 0;
}
