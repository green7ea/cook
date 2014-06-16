#include "Program.h"

#include "Object.h"
#include "Process.h"
#include "utils.h"

#include <regex>
#include <algorithm>

Program::Program(Object *obj)
    : program_name(std::regex_replace(obj->get_filename(), std::regex("\\.o"), "")),
      main_obj(obj)
{

}

void Program::resolve_dependencies(const std::vector< std::shared_ptr<Object> > &objs)
{
    dependencies.clear();

    for (const auto &obj: objs)
    {
        if (main_obj->depends_on(*obj))
        {
            dependencies.push_back(obj);
        }
    }

    // Can't change this to the new c++ loop format or it segfaults
    // because it uses iterators.
    for (size_t i = 0; i < dependencies.size(); ++i)
    {
        for (const auto &obj: objs)
        {
            if (dependencies[i]->depends_on(*obj) &&
                !depends_on(*obj))
            {
                dependencies.push_back(obj);
            }
        }
    }
}

bool Program::depends_on(const Object &obj)
{
    return (std::find_if(dependencies.begin(),
                         dependencies.end(),
                         [&](const std::shared_ptr<Object> &item)
                         {
                             return (*item == obj);
                         }) != dependencies.end());
}

void Program::link(const Config &config)
{
    const auto objects = std::vector<std::string>({main_obj->get_filename()}) +
        map<std::shared_ptr<Object>, std::string>(
            dependencies,
            [](const std::shared_ptr<Object> &item) -> std::string
            {
                return item->get_filename();
            });

    const std::string compiler = config.compiler;
    const std::vector<std::string> flags = config.flags;
    std::vector<std::string> args = {compiler};
    args = args + flags;
    args = args + objects;
    args = args + std::vector<std::string>({"-o", program_name});
    args = args + config.libs;

    printf("Linking:\n");
    for (const auto &arg: args)
    {
        printf("%s ", arg.c_str());
    }
    printf("\n");

    Process compilation_process = Process(args);
    compilation_process.join();
}
