#include "recognizer.h"

bool load_from_csv(const string& csv_path, vector<Mat>& images, vector<int>& labels){
    ifstream csv(csv_path, ifstream::in);
    if (!csv) {
        cout << "Error loading csv, file not found" << endl;
        return false;
    }
    string line;
    while (getline(csv,line)) {
        string img_path, img_label;
        img_path = line.substr(0, line.find(';'));
        img_label = line.substr(line.find(';')+1);

        if (DEBUG) cerr << format("Loading %s; label = %s",img_path.c_str(),img_label.c_str()) << endl;

        if(img_path.empty() || img_label.empty())
            continue;

        images.push_back(imread(img_path, IMREAD_GRAYSCALE));
        labels.push_back(stoi(img_label));
    }

    if (images.size() == labels.size() && images.size() > 1) {
        cout << format("%lu images loaded", images.size()) <<endl;
        return true;
    }

    cout << "Your dataset is too small, try again" << endl;
    return false;


}

void train_recognizer(const string& csv_path, const string& model_path){
    vector<Mat> images;
    vector<int> labels;

    cout << "Loading train dataset... ";

    if (!load_from_csv(csv_path,images,labels))
        fatal_error("Recognizer module: error loading csv");

    cout << "Training... ";
    Ptr<FisherFaceRecognizer> model = FisherFaceRecognizer::create();
    model->train(images,labels);

    cout << "done" << endl;

    cout << "Saving model...";
    model->write(model_path);
    cout << "done" << endl;

}

void test_recognizer(const string& csv_path, const string& model_path){
    vector<Mat> images;
    vector<int> labels;

    cout << "Loading test dataset... ";

    if (!load_from_csv(csv_path,images,labels))
        fatal_error("Recognizer module: error loading csv");

    cout << "Testing... " << endl;

    Ptr<FisherFaceRecognizer> model = FisherFaceRecognizer::create();
    model->read(model_path);

    int i, correct = 0;
    for (i = 0; i < images.size(); ++i) {
        int predict = model->predict(images[i]);
        correct += labels[i] == predict ? 1 : 0;
    }
    float precision = (float) correct / (float) i;
    cout << format("Precision: %f (%d samples, %d correct predictions)",precision,i,correct) << endl;

}

void train_test_recognizer(const string& config_dir_path, const string& model_path){
    train_recognizer(config_dir_path+"/train.csv",model_path);
    test_recognizer(config_dir_path+"/test.csv",model_path);
}