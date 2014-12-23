#include <iostream>

#include "Config.h"
#include "Process.h"
#include "INotify.h"
#include "FileDispatcher.h"
#include "UnistdError.h"

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
                                    } },
                                   {"--flags", "-f", false,
                                    [&](const std::string &)
                                    {
                                        Config config("Recipe");
                                        for (const auto &flag: config.flags)
                                        {
                                            printf("%s ", flag.c_str());
                                        }
                                        printf("\n");
                                        exit(0);
                                    } } };
    parse_arguments(argc, argv, arg_table);

    try
    {
        Config config("Recipe");


        FileDispatcher file_dispatcher(config);

        auto files = getFiles();
        for (const auto &file: files)
        {
            file_dispatcher.add_source(to_relative_path(to_abs_path(file)));
        }
        file_dispatcher.generate_programs();
        file_dispatcher.generate_libraries();

        if (running)
        {
            printf("Watching for file changes\n");

            INotify notify;
            notify.add_watch(".");

            while (running)
            {
                if (notify.wait_for_file_change(&file_dispatcher))
                {
                    file_dispatcher.generate_programs();
                    file_dispatcher.generate_libraries();
                }
            }
        }
    }
    catch (UnistdError e)
    {
        fprintf(stderr, "%s.\n", e.what());
        return 1;
    }

    return 0;
}
