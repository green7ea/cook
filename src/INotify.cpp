#include "INotify.h"

#include <unistd.h>
#include <vector>

#include "UnistdError.h"
#include "FileDispatcher.h"

INotify::INotify()
    : fd(inotify_init())
{
    if (fd < 0)
    {
        throw UnistdError("Couldn't initialize inotify", errno);
    }
}

INotify::~INotify()
{
    if (fd >= 0)
    {
        close(fd);
    }
}

void INotify::add_watch(const std::string &filepath, int mask)
{
    const int wd = inotify_add_watch(fd, filepath.c_str(), mask);
    if (wd < 0)
    {
        throw UnistdError("Addind inotify watch", errno);
    }
}

bool INotify::wait_for_file_change(FileDispatcher *dispatch)
{
    const int size = sizeof(inotify_event) + 512 + 1;
    char buffer[size];

    const size_t amount_read = read(fd, buffer, size);
    if (amount_read < sizeof(struct inotify_event))
    {
        throw UnistdError("Read inotify event", errno);
    }

    struct inotify_event *event = (struct inotify_event *)buffer;
    std::string filename;
    if (event->len > 0)
    {
        filename = event->name;
    }

    if (event->mask & IN_MODIFY)
    {
        return dispatch->update(filename);
    }
    else if (event->mask & IN_CREATE)
    {
        dispatch->add_source(filename);
        return true;
    }
    else if (event->mask & IN_DELETE)
    {
        dispatch->remove_source(filename);
    }
    else
    {
        printf("Unknown INotify event\n");
    }

    return false;
}
