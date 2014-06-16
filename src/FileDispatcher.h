#pragma once

#include <string>
#include <map>
#include <functional>
#include <memory>

#include "Config.h"
#include "SourceFile.h"

class FileDispatcher
{
    typedef std::function<void (FileDispatcher *,
                                const Config &,
                                const std::string &)> extension_func;

public:
    FileDispatcher(const Config &config);
    void add_source(const std::string &filename);
    void remove_source(const std::string &filename);
    bool update(const std::string &filename);
    void generate_programs();

private:
    Config config;
    std::vector<SourceFile> source_files;
};
