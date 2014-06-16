#pragma once

#include <memory>
#include <string>

#include "Object.h"

class Header
{

public:
    Header(const std::string &filename,
           const std::shared_ptr<Object> &obj);
    ~Header();
    void update(const Config &config);
    const std::string & get_filename() const;

private:
    std::string filename;
    std::shared_ptr<Object> obj;
};
