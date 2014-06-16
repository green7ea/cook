#pragma once

#include <memory>

class Pipe
{

public:
    Pipe();

    void init();

    void closeIn();
    void closeOut();

    void bindToStdIn();
    void bindToStdOut();
    void bindToStdErr();

    int getFdIn() const;
    int getFdOut() const;

private:
    std::shared_ptr<bool> created;
    std::shared_ptr<int> pipe_in;
    std::shared_ptr<int> pipe_out;
};
