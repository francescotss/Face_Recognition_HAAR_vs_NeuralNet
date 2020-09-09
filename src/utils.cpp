#include "utils.h"

set<string> img_extentions({".jpg",".jpeg",".png",".pgm"});

void handle_error(const string& msg){
    cerr << msg << endl << strerror(errno) << endl;
    exit(EXIT_FAILURE);
}

bool is_picture(const string& path){
    //string ext = fs::path(path).extension();
    //return img_extentions.count(ext);
    return true;
}

void create_csv(const string& dir, const string& train_name, const string& test_name){
    DIR* root = opendir(dir.c_str());
    if (root == NULL) handle_error("Error opening directory");

    struct dirent *root_entry;
    while ((root_entry = readdir(root)) != NULL) {// && root_entry->d_type == DT_DIR) { //d_type non funziona con tutti i file system

        string group_dir_name = root_entry->d_name;
        DIR* group_dir;
        ofstream out_file;

        if (DEBUG) cerr << "-->" << group_dir_name << endl;

        if (group_dir_name == "Training"){
            out_file.open(train_name, ios::out | ios::trunc);
            if (!out_file.is_open())
                handle_error("Error opening file" + train_name);
            group_dir = opendir((dir+'/'+group_dir_name).c_str());
            if (group_dir == NULL)
                handle_error("Error opening directory");

        } else if (strcmp(root_entry->d_name,"Testing") == 0){
            out_file.open(test_name, ios::out | ios::trunc);
            if (!out_file.is_open())
                handle_error("Error opening file");
            group_dir = opendir((dir+'/'+group_dir_name).c_str());
            if (group_dir == NULL)
                handle_error("Error opening directory");

        } else continue;


        struct dirent *group_entry;
        while ((group_entry = readdir(group_dir)) != NULL) { // && group_entry->d_type == DT_UNKNOWN) {

            if (DEBUG) cerr << "---->" << group_entry->d_name << endl;

            string entry_dir_name = group_entry->d_name;
            if (entry_dir_name == "." || entry_dir_name == "..") continue;
            DIR* entry_dir = opendir((dir+"/"+group_dir_name+"/"+entry_dir_name).c_str());
            if (entry_dir == NULL) continue;

            struct dirent *entry;
            while ((entry = readdir(entry_dir)) != NULL) {
                string entry_name = entry->d_name;
                if (entry_name == "." || entry_name == "..")
                    continue;
                if (DEBUG) cerr << "------>" << entry_name << endl;

                string path = dir+"/"+group_dir_name+"/"+entry_dir_name+"/"+entry_name;
                if (!is_picture(path))
                    continue;

                out_file << path << ";" << entry_dir_name.substr(1) << endl;

            }


        }

        out_file.close();
        closedir(group_dir);
    }
    closedir(root);
}