#include "filecontrol.hpp"

using namespace std;

void FileControl::read_directory(const string& name, list<string>& string_vec)
{
    DIR* dirp = opendir(name.c_str());
    struct dirent* dp;

    while((dp = readdir(dirp)) != NULL)
    {
        string cmp = dp->d_name;
        if(cmp != ".." && cmp != ".")
            string_vec.push_back(dp->d_name);
    }
    closedir(dirp);
}

FileControl::FileControl(string& _dirname)
{
    this->dirname = _dirname;
    read_directory(this->dirname, this->file_list);
    this->current_pos = this->file_list.begin();
}

FileControl::FileControl(void)
{
    this->dirname = "pictures";
    read_directory(this->dirname, this->file_list);
    this->current_pos = this->file_list.begin();
}

QString FileControl::get_filename(void)
{
    if(this->file_list.size() == 0)
        return QString::fromStdString(NONE_FILE);
    else
        return QString::fromStdString(*current_pos);
}
string FileControl::get_filename_native(void)
{
    if(this->file_list.size() == 0)
        return NONE_FILE;
    else
        return *current_pos;
}

int FileControl::get_size(void)
{
    return this->file_list.size();
}

//move function
bool FileControl::prev(void)
{
    if(this->file_list.size() <= 1)
        return false;
    else
    {
        if(this->current_pos == this->file_list.begin())
        {
            //only over 2 can run back function
            if( this->file_list.size() > 1)
            {
                this->current_pos = this->file_list.end();
                this->current_pos--;
            }
        }
        else
        {
            this->current_pos--;
        }
        return true;
    }

}
bool FileControl::next(void)
{
    if(this->file_list.size() <= 1)
        return false;
    else
    {
        this->current_pos++;
        if(this->current_pos == this->file_list.end())
        {
            this->current_pos = this->file_list.begin();
        }
        return true;
    }
}

void FileControl::add_pic(string& filename)
{
    this->file_list.push_back(filename);
    this->current_pos = --this->file_list.end();
}

//wrapping functions
int wrapped::w_open(const char* filename, int flags)
{
    return open(filename, flags);
}
ssize_t wrapped::w_read(int fd, void* buf, size_t count)
{
    return read(fd, buf, count);
}
ssize_t wrapped::w_write(int fd, const void* buf, size_t count)
{
    return write(fd, buf, count);
}

