#include "dataset.h"


set<string> img_extentions({".jpg",".jpeg",".png",".pgm"});

bool is_picture(const fs::path& path){
    string ext = path.extension();
    return img_extentions.count(ext);
}

void read_dir(const string& in_path,vector<string>& img_paths,  int min_images){

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

//TODO
void write_to_path(Mat& img,const string& in_path, const string& out_path, const string& img_path){
    string local_img_path = img_path.substr(in_path.size());
    string final_path = out_path + local_img_path;

    if (DEBUG) cerr << "Wrinting image in: " << final_path << endl;

    fs::create_directories(final_path.substr(0,final_path.rfind('/')));
    imwrite(final_path, img);


}


void detect_faces(const string& in_path, const string& out_path, const string& face_model_path, vector<string>& img_paths){
    CascadeClassifier classifier;
    classifier.load(face_model_path);
    long int total = img_paths.size();
    long int done = 0;
    short int steps = 1;

    for(const auto& img_path : img_paths){
        Mat img;
        Mat gray;
        Mat out;
        vector<Rect> faces;
        img = imread(img_path);

        if (DEBUG) cerr << img_path << " loaded" << endl;

        if (img.channels() > 1)
            cvtColor(img, gray, COLOR_BGR2GRAY);
        else
            gray = img.clone();

        equalizeHist(gray,gray);
        classifier.detectMultiScale(gray,faces);

        if (!faces.empty()){
            out = img(faces[0]);
            write_to_path(out,in_path,out_path,img_path);
        }

        if (done++ > ((total/10) * steps)){
            cout << steps*10 << "%...";
            steps++;
            flush(cout);
        }
    }
}


void create_dataset(const string& in_dir_path, const string& out_dir_path, const string& face_model_path, int min_images){

    vector<string> img_paths;

    cout << "Reading directory " << in_dir_path << endl;
    read_dir(in_dir_path,img_paths,min_images);
    cout << format("%lu images founded. (%d minimum images per person)", img_paths.size(),min_images) << endl;

    cout << "Detecting faces and saving in " << out_dir_path << endl;
    cout << "(can take a while...)" << endl;
    detect_faces(in_dir_path,out_dir_path,face_model_path,img_paths);
    cout << "Done" << endl;

}

void create_csv(const string& in_dir_path,const string& out_dir_path, int img_for_training){
    vector<string> img_paths;
    ofstream train_csv;
    ofstream test_csv;

    cout << "Reading directory " << in_dir_path << endl;
    read_dir(in_dir_path,img_paths,img_for_training);
    cout << format("%lu images founded. (using %d images per person)", img_paths.size(),img_for_training) << endl;

    train_csv.open(out_dir_path+"/train.csv",ios::out | ios::trunc);
    test_csv.open(out_dir_path+"/test.csv",ios::out | ios::trunc);
    if (!train_csv.is_open() || !test_csv.is_open())
        fatal_error("Error creating database config");

    int id = 0, count = 0;

    string last_read = "";

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

    train_csv.close();
    test_csv.close();

}

