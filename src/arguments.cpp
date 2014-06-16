#include "arguments.h"

#include <algorithm>

void parse_arguments(int argc, char **argv,
                     const std::vector<Arg> &arg_table)
{
    for (int i = 1; i < argc; ++i)
    {
        const std::string arg = argv[i];

        const auto res = std::find_if(
            arg_table.begin(), arg_table.end(),
            [&](const Arg &item) -> bool
            {
                return (arg == item.short_arg ||
                        arg == item.long_arg);
            });
        if (res != arg_table.end())
        {
            if (res->takes_argument)
            {
                if ((i + 1) >= argc)
                {
                    throw "Not enough args";
                }
                res->action(argv[++i]);
            }
            else
            {
                res->action("");
            }
        }
    }
}

void print_help(const std::string &program_name,
                const std::vector<Arg> &arg_table)
{
    printf("%s [options]\n", program_name.c_str());
    for (const auto &arg: arg_table)
    {
        if (arg.takes_argument)
        {
            printf("\t%s or %s [argument]\n",
                   arg.long_arg.c_str(),
                   arg.short_arg.c_str());
        }
        else
        {
            printf("\t%s or %s\n",
                   arg.long_arg.c_str(),
                   arg.short_arg.c_str());
        }
    }
}
