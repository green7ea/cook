#include <iostream>

#include "Config.h"
#include "Process.h"
#include "INotify.h"
#include "FileDispatcher.h"

#include "utils.h"
#include "file.h"
#include "arguments.h"

std::vector<std::string> getFiles()
{
    Process proc = Process({"find", ".", "-iname", "*.cpp"});
    std::string file_list = proc.output();
    return tokenize(file_list);
}

int main(int argc, char **argv)
{
    bool running = false;
    std::vector<Arg> arg_table = { {"--continuous", "-c", false,
                                    [&](const std::string &)
                                    {
                                        running = true;
                                    } },
                                   {"--help", "-h", false,
                                    [&](const std::string &)
                                    {
                                        print_help(argv[0], arg_table);
                                        exit(0);
                                    } } };
    parse_arguments(argc, argv, arg_table);

    Config config("Recipe");
    FileDispatcher file_dispatcher(config);

    auto files = getFiles();
    for (const auto &file: files)
    {
        file_dispatcher.add_source(to_relative_path(to_abs_path(file)));
    }
    file_dispatcher.generate_programs();

    INotify notify;
    notify.add_watch(".");

    if (running)
    {
        printf("Watching for file changes\n");
    }
    while (running)
    {
        if (notify.wait_for_file_change(&file_dispatcher))
        {
            file_dispatcher.generate_programs();
        }
    }

    return 0;
}
