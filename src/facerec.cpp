#include "facerec.h"

int main(int argc, const char* argv[]) {
    cout << "reading directory " << STD_DIR << "..." << endl;
    create_csv(STD_DIR,STD_TRAIN_CSV,STD_TEST_CSV);
    cout << "done" << endl;
    return 0;
}
