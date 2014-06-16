#pragma once

#include <string>
#include <map>
#include <sys/inotify.h>

class FileDispatcher;

class INotify
{

public:
    INotify();
    ~INotify();
    void add_watch(const std::string &filepath,
                  int mask = IN_MODIFY | IN_CREATE | IN_DELETE);
    bool wait_for_file_change(FileDispatcher *dispatch);

private:
    int fd;
};
