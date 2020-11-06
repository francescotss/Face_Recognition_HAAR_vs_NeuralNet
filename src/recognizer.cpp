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

    csv.close();

    if (images.size() == labels.size() && images.size() > 1) {
        cout << format("%lu images loaded", images.size()) <<endl;
        return true;
    }

    cout << "Your dataset is too small, try again" << endl;
    return false;


}

void train_recognizer(){
    vector<Mat> images;
    vector<int> labels;
    string csv_path = GlobalConfig::get_string("TRAINING_CSV");
    string model_path = GlobalConfig::get_string("FACE_RECOGNITION_MODEL");

    cout << "Loading train dataset... ";

    if (!load_from_csv(csv_path,images,labels))
        fatal_error("Recognizer module: error loading csv");

    cout << "Training... ";
    Ptr<FisherFaceRecognizer> model = FisherFaceRecognizer::create();
    double start = getTickCount();
    model->train(images,labels);
    double stop = getTickCount();

    cout << "done" << endl;
    double time = (stop - start)/getTickFrequency();

    cout << "Training time: " << time << endl;

    cout << "Saving model...";
    model->write(model_path);
    cout << "done" << endl;

}

void test_recognizer(){
    vector<Mat> images;
    vector<int> labels;
    string csv_path = GlobalConfig::get_string("TESTING_CSV");
    string model_path = GlobalConfig::get_string("FACE_RECOGNITION_MODEL");
    int verbose = GlobalConfig::get_value("VERBOSITY");

    cout << "Loading test dataset... ";

    if (!load_from_csv(csv_path,images,labels))
        fatal_error("Recognizer module: error loading csv");

    cout << "Testing... " << endl;

    Ptr<FisherFaceRecognizer> model = FisherFaceRecognizer::create();
    model->read(model_path);

    double start = getTickCount();
    int i, correct = 0;
    for (i = 0; i < images.size(); ++i) {
        int predict = model->predict(images[i]);
        correct += labels[i] == predict ? 1 : 0;

        if (verbose > 0){
            string name = get_name(labels[i]);
            string predict_name = get_name(predict);
            putText(images[i],
                    format("Name: %s",name.c_str()),
                    cv::Point(10,20),
                    cv::FONT_HERSHEY_SIMPLEX,
                    0.5,
                    cv::Scalar(255,255,255)
                    );

            putText(images[i],
                    format("Predict: %s",predict_name.c_str()),
                    cv::Point(10,40),
                    cv::FONT_HERSHEY_SIMPLEX,
                    0.5,
                    cv::Scalar(255,255,255)
            );

            imshow("Results",images[i]);
            waitKey(0);
        }
    }
    double stop = getTickCount();
    double time = (stop - start)/getTickFrequency();
    float precision = (float) correct / (float) i;
    cout << format("Precision: %f (%d samples, %d correct predictions)",precision,i,correct) << endl;
    cout << "Testing time: " << time << endl;

}

void train_test_recognizer(){
    train_recognizer();
    test_recognizer();
}