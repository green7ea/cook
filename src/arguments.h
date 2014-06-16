#pragma once

#include <functional>
#include <vector>

struct Arg
{
    std::string long_arg;
    std::string short_arg;
    bool takes_argument;
    std::function<void (const std::string &value)> action;
};

void parse_arguments(int argc, char **argv,
                     const std::vector<Arg> &arg_table);
void print_help(const std::string &program_name,
                const std::vector<Arg> &arg_table);
