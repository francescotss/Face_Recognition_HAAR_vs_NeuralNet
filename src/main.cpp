#include "main.h"

int main(int argc, const char* argv[]) {
    cout << "reading directory " << STD_DIR << " ...";
    create_csv(STD_DIR,STD_TRAIN_CSV,STD_TEST_CSV);
    cout << "done" << endl;

    train_recognizer(STD_TRAIN_CSV,STD_MODEL);
    test_recognizer(STD_TEST_CSV,STD_MODEL);
    return 0;
}
