#include "Pipe.h"

#include <unistd.h>

#include "UnistdError.h"

#define READ 0
#define WRITE 1
#define ERROR 2

Pipe::Pipe()
    : created(std::shared_ptr<bool>(new bool))
{
    *created = false;
}

void Pipe::init()
{
    if (*created)
    {
        return;
    }
    *created = true;

    int unix_pipes[2];
    if (pipe(unix_pipes) != 0)
    {
        throw UnistdError("Creating pipes", errno);
    }

    pipe_in = std::shared_ptr<int>(
        new int(unix_pipes[READ]),
        [](int *input)
        {
            close(*input);
            delete input;
        });
    pipe_out = std::shared_ptr<int>(
        new int(unix_pipes[WRITE]),
        [](int *input)
        {
            close(*input);
            delete input;
        });
}

void Pipe::closeIn()
{
    pipe_in = std::shared_ptr<int>(nullptr);
}

void Pipe::closeOut()
{
    pipe_out = std::shared_ptr<int>(nullptr);
}

void Pipe::bindToStdIn()
{
    if (pipe_in != nullptr)
    {
        dup2(*pipe_in, READ);
    }
}

void Pipe::bindToStdOut()
{
    if (pipe_out != nullptr)
    {
        dup2(*pipe_out, WRITE);
    }
}

void Pipe::bindToStdErr()
{
    if (pipe_out != nullptr)
    {
        dup2(*pipe_out, ERROR);
    }
}

int Pipe::getFdIn() const
{
    if (pipe_in != nullptr)
    {
        return *pipe_in;
    }
    return -1;
}

int Pipe::getFdOut() const
{
    if (pipe_out != nullptr)
    {
        return *pipe_out;
    }
    return -1;
}
