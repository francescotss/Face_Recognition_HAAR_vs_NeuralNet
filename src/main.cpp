#include "main.h"

int main(int argc, const char* argv[]) {

    GlobalConfig::init();
    create_dataset();
    create_csv(50);
    train_test_recognizer();

    return 0;

    return 0;
}
