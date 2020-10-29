#include "dataset.h"


set<string> img_extentions({".jpg",".jpeg",".png",".pgm"});

bool is_picture(const fs::path& path){
    string ext = path.extension();
    return img_extentions.count(ext);
}

void read_dir(const string& in_path, vector<string>& img_paths,  short min_images){
    if (DEBUG) cerr << "Reading dir " << in_path <<endl;

    vector<string> temp_paths;
    for (const auto& entry : fs::directory_iterator(in_path)){
        if (entry.is_directory())
            read_dir(entry.path(),img_paths,min_images);
        if (entry.is_regular_file() && is_picture(entry.path()))
            temp_paths.push_back(entry.path());
    }

    if (temp_paths.size() >= min_images){
        for(const auto& entry : temp_paths)
            img_paths.push_back(entry);
    }
}


void write_to_path(Mat& img, const string& img_path){
    string in_path = GlobalConfig::get_string("IMAGES_DIR");
    string out_path = GlobalConfig::get_string("FACES_DIR");
    string local_img_path = img_path.substr(in_path.size());
    string final_path = out_path + local_img_path;

    if (DEBUG) cerr << "Wrinting image in: " << final_path << endl;

    fs::create_directories(final_path.substr(0,final_path.rfind('/')));
    imwrite(final_path, img);

}


/*
 * Analizza le foto e trova un volto per ogni foto
 *
 */
void detect_faces(vector<string>& img_paths, short width, short height){
    string face_model_path = GlobalConfig::get_string("FACE_DETECTION_MODEL");
    CascadeClassifier classifier;
    long int total = img_paths.size();
    long int done = 0;
    short int steps = 1; //Step percentuali da mostrare a schermo in decimi

    classifier.load(face_model_path);

    for(const auto& img_path : img_paths){
        Mat img;
        Mat gray;
        Mat out;
        vector<Rect> faces;
        img = imread(img_path,IMREAD_UNCHANGED);

        if (DEBUG) cerr << img_path << " loaded" << endl;

        if (img.channels() > 1)
            cvtColor(img, gray, COLOR_BGR2GRAY);
        else
            gray = img.clone();

        equalizeHist(gray,gray);
        classifier.detectMultiScale(gray,faces);

        if (!faces.empty()){
            cv::resize(img(faces[0]),out,cv::Size(width,height));
            write_to_path(out,img_path);
        }
        //Calcolo percentuale del lavoro totale compiuto da mostrare a schermo
        if (done++ > ((total/10) * steps)){
            cout << steps*10 << "%...";
            steps++;
            flush(cout);
        }
    }
}


void create_dataset(){

    vector<string> img_paths;
    string images_path = GlobalConfig::get_string("IMAGES_DIR");
    short min_images = GlobalConfig::get_value("MIN_FACES");
    short width = GlobalConfig::get_value("WIDTH");
    short height = GlobalConfig::get_value("HEIGHT");

    read_dir(images_path,img_paths,min_images);

    cout << format("%lu images founded in %s (%d minimum images per person)", img_paths.size(), images_path.c_str(), min_images) << endl;
    cout << "Detecting faces and saving in " << GlobalConfig::get_string("FACES_DIR") << endl;
    cout << "(can take a while...)" << endl << "0%...";


    detect_faces(img_paths,width,height);

    cout << "Done" << endl;

}

void create_csv(short img_for_training){
    vector<string> img_paths;
    ofstream train_csv;
    ofstream test_csv;
    string faces_path = GlobalConfig::get_string("FACES_DIR");
    string training_csv = GlobalConfig::get_string("TRAINING_CSV");
    string testing_csv = GlobalConfig::get_string("TESTING_CSV");

    read_dir(faces_path,img_paths,img_for_training);
    cout << format("%lu images founded in %s (%d minimum images per person)", img_paths.size(), faces_path.c_str(), img_for_training) << endl;


    train_csv.open(training_csv,ios::out | ios::trunc);
    test_csv.open(testing_csv,ios::out | ios::trunc);
    if (!train_csv.is_open() || !test_csv.is_open())
        fatal_error("Error creating database config");

    int id = 0, count = 0;

    string last_read;

    for (const auto& img_path : img_paths){
        string parent_dir = img_path.substr(0,img_path.rfind('/'));
        if(parent_dir != last_read) {
            last_read = parent_dir;
            id++;
            count = 0;
        }

        if (DEBUG) cerr << format("Adding to the database: [ID] %d [Path] %s", id, img_path.c_str()) << endl;
        if (count < img_for_training)
            train_csv << img_path << ";" << id << endl;
        else
            test_csv << img_path << ";" << id << endl;
        count++;
    }

    cout << format("%d people founded",id) << endl;

    train_csv.close();
    test_csv.close();
}

