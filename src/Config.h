#pragma once

#include <string>
#include <jansson.h>
#include <vector>

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

private:
    void pkg_config();
};
