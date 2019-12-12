#ifndef FILECONTROL_HPP
# define FILECONTROL_HPP

#include <list>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <QString>

//directory function original : https://codingcoding.tistory.com/900

#define NONE_FILE   "FILE EMPTY"

using namespace std;

class FileControl
{
private:
    string dirname;
    list<string> file_list;

    // current pos about img
    list<string>::iterator current_pos;
public:
    explicit FileControl(string& _dirname);
    explicit FileControl(void);
    static void read_directory(const string& name, list<string>& string_vec);

    QString get_filename(void);
    string get_filename_native(void);
    int get_size(void);

    //move cursor
    bool prev(void);
    bool next(void);
    void add_pic(string& filename);
};

#endif
