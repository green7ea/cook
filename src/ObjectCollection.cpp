#include "ObjectCollection.h"

#include "Object.h"
#include "Process.h"
#include "utils.h"

#include <regex>
#include <algorithm>

ObjectCollection::ObjectCollection(const std::string &artifact_name,
                                   const std::vector< std::shared_ptr<Object> >
                                   &initial_objects)
    : artifact_name(artifact_name),
      initial_objects(initial_objects)
{

}

void ObjectCollection::resolve_dependencies(const std::vector< std::shared_ptr<Object> > &objs)
{
    dependencies.clear();

    for (const auto &potential_dependency: objs)
    {
        for (const auto &object: initial_objects)
        {
            if (object->depends_on(*potential_dependency))
            {
                dependencies.push_back(potential_dependency);
                break;
            }
        }
    }

    // Can't change this to the new c++ loop format or it segfaults
    // because it uses iterators and we have to recalculate the size
    // on every loop.
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

bool ObjectCollection::depends_on(const Object &obj)
{
    return (std::find_if(dependencies.begin(),
                         dependencies.end(),
                         [&](const std::shared_ptr<Object> &item)
                         {
                             return (*item == obj);
                         }) != dependencies.end());
}

void ObjectCollection::link_into_program(const Config &config)
{
    printf("Building %s\n", artifact_name.c_str());
    const auto args = build_object_arguments(config, false);
    Process compile(args);
    compile.join();
}

void ObjectCollection::link_into_shared_object(const Config &config)
{
    printf("Linking %s\n", artifact_name.c_str());
    const auto args = build_object_arguments(config, true);
    Process compile(args);
    compile.join();
}

std::vector<std::string> ObjectCollection::build_object_arguments(const Config &config,
                                                                  bool shared)
{
    const auto objects =
        map<std::shared_ptr<Object>, std::string>(
            initial_objects,
            [](const std::shared_ptr<Object> &item) -> std::string
            {
                return item->get_filename();
            }) +
        map<std::shared_ptr<Object>, std::string>(
            dependencies,
            [](const std::shared_ptr<Object> &item) -> std::string
            {
                return item->get_filename();
            });

    const std::string compiler = config.compiler;
    const std::vector<std::string> flags = config.flags;
    std::vector<std::string> args = {compiler};

    if (shared)
    {
        args = args + std::vector<std::string>({"-shared"});
    }

    args = args + flags;
    args = args + objects;
    args = args + std::vector<std::string>({"-o", artifact_name});
    args = args + config.libs;

    return args;
}
