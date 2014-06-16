#pragma once

#include <string>
#include <vector>

#include "Pipe.h"

class Process
{

public:
    Process(const std::vector<std::string> &args);
    Process(const Process &) = delete;
    Process(Process &&orig);
    ~Process();

    Process & operator=(const Process &process) = delete;
    Process & operator=(Process &&process);
    Process && operator|(Process &&receiver);

    void run();
    void join();
    const std::string & output();

private:
    enum ProcessState
    {
        NOT_STARTED,
        RUNNING,
        COLLECTED
    };

    void readPipe();

    std::vector<std::string> args;
    std::string result;

    pid_t pid;
    Pipe standard_in;
    Pipe standard_out;

    Process *next;
    ProcessState state;
};
