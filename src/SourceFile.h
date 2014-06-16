#pragma once

#include "Object.h"
#include "Header.h"

#include <vector>
#include <memory>
#include <string>

class SourceFile
{

public:
    SourceFile(const std::string &filename);
    ~SourceFile();
    const std::string & get_filename() const;
    std::shared_ptr<Object> get_object() const;
    void update(const Config &config);
    bool update(const Config &config, const std::string &changed_file);

private:
    void extract_headers(const Config &config);

    std::string filename;
    std::shared_ptr<Object> object;
    std::vector<Header> headers;
};
