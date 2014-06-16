#include "FileDispatcher.h"

#include <stdexcept>
#include <regex>
#include <algorithm>

#include "Program.h"
#include "utils.h"
#include "file.h"

FileDispatcher::FileDispatcher(const Config &config)
    : config(config)
{

}

void FileDispatcher::add_source(const std::string &filename)
{
    const auto res = std::find_if(source_files.begin(),
                                  source_files.end(),
                                  [&](const SourceFile &sf) -> bool
                                  {
                                      return (sf.get_filename() == filename);
                                  });
    if (res != source_files.end())
    {
        return;
    }

    source_files.push_back(SourceFile(filename));
}

void FileDispatcher::remove_source(const std::string &filename)
{
    filter<SourceFile>(
        source_files,
        [&](const SourceFile &sf) -> bool
        {
            return (sf.get_filename() == filename);
        });
}

bool FileDispatcher::update(const std::string &filename)
{
    bool updated = false;

    for (auto &source_file: source_files)
    {
        // We can't early exit because multiple source files might
        // depend on the same header.
        updated = source_file.update(config, filename) || updated;
    }

    return updated;
}

void FileDispatcher::generate_programs()
{
    for (auto &source_file: source_files)
    {
        source_file.update(config);
    }

    auto objs = map< SourceFile, std::shared_ptr<Object> >(
        source_files,
        [](const SourceFile &input) -> std::shared_ptr<Object>
        {
            return input.get_object();
        });

    auto progs = map< std::shared_ptr<Object>, Program * >(
        objs,
        [](const std::shared_ptr<Object> &obj)
        {
            return obj->get_program();
        });

    filter<Program *>(
        progs,
        [](Program *ptr) -> bool
        {
            return ptr == nullptr;
        });

    for (auto &prog: progs)
    {
        prog->resolve_dependencies(objs);
        prog->link(config);
    }
}
