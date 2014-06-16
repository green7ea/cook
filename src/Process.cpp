#include "Process.h"

#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <functional>
#include <memory>

#include "Ressource.h"
#include "UnistdError.h"
#include "utils.h"

#define READ 0
#define WRITE 1

static void customExec(const std::vector<std::string> &args)
{
    std::vector<char *> cargs;

    for (auto &arg: args)
    {
        // const casting is not ideal. Should fix this.
        cargs.push_back((char *)arg.c_str());
    }
    cargs.push_back(nullptr);
    execvp(cargs[0], cargs.data());
    const std::string message = "Creating process " + args[0];
    throw UnistdError(message, errno);
}

Process::Process(const std::vector<std::string> &args)
    : args(args),
      pid(0),
      next(nullptr),
      state(NOT_STARTED)
{
    standard_out.init();
}

Process::Process(Process &&orig)
    : args(std::move(orig.args)),
      result(std::move(orig.result)),
      pid(orig.pid),
      standard_in(std::move(orig.standard_in)),
      standard_out(std::move(orig.standard_out)),
      next(orig.next),
      state(orig.state)
{
    orig.pid = 0;
    orig.next = nullptr;
}

Process::~Process()
{
    if (next)
    {
        delete next;
    }

    int status;
    if (pid > 0 && waitpid(pid, &status, 0) != -1)
    {
        kill(pid, SIGKILL);
    }
}

Process & Process::operator=(Process &&process)
{
    args = std::move(process.args);
    result = std::move(process.result);
    pid = process.pid;
    process.pid = 0;
    standard_in = std::move(process.standard_in);
    standard_out = std::move(process.standard_out);
    state = process.state;
    process.state = COLLECTED;

    return *this;
}

Process && Process::operator|(Process &&receiver)
{
    if (next)
    {
        next->operator|(std::move(receiver));
    }
    else
    {
        next = new Process(std::move(receiver));
        next->standard_in = standard_out;
        next->standard_in.closeOut();
    }

    return std::move(*this);
}

void Process::run()
{
    if (state != NOT_STARTED)
    {
        return;
    }

    pid = fork();
    if (pid == 0)
    {
        // We are the child process
        standard_in.bindToStdIn();

        standard_out.closeIn();
        standard_out.bindToStdOut();

        customExec(args);
    }

    standard_in.closeIn();
    standard_in.closeOut();
    standard_out.closeOut();

    state = RUNNING;

    if (next)
    {
        standard_out.closeIn();
        next->run();
    }
}

void Process::join()
{
    if (state == NOT_STARTED)
    {
        run();
    }
    if (state != RUNNING)
    {
        return;
    }

    Process *child = this;
    while (child->next != nullptr)
    {
        child = child->next;
    }
    Pipe &std_out = child->standard_out;
    const int fd = std_out.getFdIn();

    if (fd < 0)
    {
        return;
    }

    result = read_all_fd(fd);

    std_out.closeIn();

    state = COLLECTED;
}

const std::string & Process::output()
{
    if (state == NOT_STARTED)
    {
        run();
    }
    if (state == RUNNING)
    {
        join();
    }
    return result;
}
