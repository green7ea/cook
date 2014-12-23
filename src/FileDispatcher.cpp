#include "FileDispatcher.h"

#include <stdexcept>
#include <regex>
#include <algorithm>

#include "ObjectCollection.h"
#include "Error.h"
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

static std::vector< std::shared_ptr<Object> > objects_from_sources(
    const std::vector<SourceFile> &source_files)
{
    auto objects = map< SourceFile, std::shared_ptr<Object> >(
        source_files,
        [](const SourceFile &input) -> std::shared_ptr<Object>
        {
            return input.get_object();
        });
    return objects;
}

void FileDispatcher::generate_programs()
{
    for (auto &source_file: source_files)
    {
        source_file.update(config);
    }
    auto objects = objects_from_sources(source_files);
    auto mains = objects;

    filter< std::shared_ptr<Object> >(
        mains,
        [](const std::shared_ptr<Object> &obj) -> bool
        {
            return !(obj->contains("main"));
        } );

    for (auto &main: mains)
    {
        std::string progam_name = strip_extension(main->get_filename());
        ObjectCollection collection(progam_name,
                                    std::vector< std::shared_ptr<Object> >({main}));
        collection.resolve_dependencies(objects);
        collection.link_into_program(config);
    }
}

void FileDispatcher::generate_libraries()
{
    for (const Library &lib: config.shared)
    {
        try
        {
            auto initial_objects = get_objects(lib);
            auto objects = objects_from_sources(source_files);
            ObjectCollection collection(lib.name, initial_objects);
            collection.resolve_dependencies(objects);
            collection.link_into_shared_object(config);
        }
        catch (Error &e)
        {
            printf("%s\n\nNot building %s\n\n",
                   e.what(), lib.name.c_str());
        }
    }
}

std::vector< std::shared_ptr<Object> > FileDispatcher::get_objects(const Library &lib)
{
    std::vector< std::shared_ptr<Object> > results;

    for (auto &source_name: lib.objects)
    {
        auto source = std::find_if(source_files.begin(),
                                   source_files.end(),
                                   [&](const SourceFile &source) -> bool
                                   {
                                       return (source.get_filename() == source_name);
                                   });
        if (source == source_files.end())
        {
            throw Error("Source file not found: '" + source_name + "'.");
        }
        results.push_back(source->get_object());
    }

    return results;
}
