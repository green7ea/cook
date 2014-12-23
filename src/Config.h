#pragma once

#include <string>
#include <jansson.h>
#include <vector>

struct Library
{
    std::string name;
    std::vector<std::string> objects;
};

class Config
{

public:
    Config(const std::string &filename);

    std::string compiler;
    std::string nm;
    std::vector<std::string> flags;
    std::vector<std::string> libs;
    std::vector<std::string> packages;
    std::vector<std::string> defines;
    std::vector<Library> shared;

private:
    void pkg_config();
};
