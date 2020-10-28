#include "main.h"

int main(int argc, const char* argv[]) {

    //create_dataset(STD_DATA_IN_DIR,STD_FACES_DIR,STD_DETECTION_MODEL_PATH,100,50,50);
    create_csv(STD_FACES_DIR,STD_CONFIG_DIR,50);
    train_test_recognizer(STD_CONFIG_DIR,STD_RECOGNITION_MODEL_PATH);

    return 0;

    return 0;
}
