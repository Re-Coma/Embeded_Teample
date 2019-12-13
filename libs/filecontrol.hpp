#ifndef FILECONTROL_HPP
# define FILECONTROL_HPP

#include <list>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
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

namespace wrapped
{
    int w_open(const char* filename, int flags);
    ssize_t w_read(int fd, void* buf, size_t count);
    ssize_t w_write(int fd, const void* buf, size_t count);
};

#endif
