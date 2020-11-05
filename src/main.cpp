#include "main.h"

void create_dataset_menu(){
    cout << "Minimum images per person: ";
    short entry;
    cin >> entry;
    create_csv(entry);
}

void train_test_neural(){
    cout << "Loading..." << endl;
    cout.flush();
    system("pipenv run python ../src/neural_network.py --train --test");
}

void train_neural(){
    cout << "Loading..." << endl;
    cout.flush();
    system("pipenv run python ../src/neural_network.py --train");
}

void test_neural(){
    cout << "Loading..." << endl;
    cout.flush();
    system("pipenv run python ../src/neural_network.py --test");
}

int main(int argc, const char* argv[]) {

    GlobalConfig::init();

    cout << "Welcome in HAAR Cascade and Neural Network comparator for face recognition \nBy Francesco Riccardo Tassone" << endl;
    cout << "For reference check Readme.txt (Italian)" << endl;

    bool quit = false;
    while (!quit){
        cout << "\nChoose Option:\n"
                "1 - Create faces folder and analyze\n"
                "2 - Create dataset (You have to run [1] at least once)\n"
                "3 - Train and test HAAR model\n"
                "4 - Train only HAAR model\n"
                "5 - Test only HAAR model\n"
                "6 - Train and test Neural Network model\n"
                "7 - Train only Neural Network model\n"
                "8 - Test only Neural Network model\n"
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
                train_test_recognizer();
                break;
            case 4:
                train_recognizer();
                break;
            case 5:
                test_recognizer();
                break;
            case 6:
                train_test_neural();
                break;
            case 7:
                train_neural();
                break;
            case 8:
                test_neural();
                break;
            case 0:
                quit = true;
                cout << "Bye" << endl;
                break;
        }
    }

    return 0;
}
