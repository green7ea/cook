#pragma once

#include "Config.h"

class Program;

class Object
{

public:
    Object(const std::string &cpp_file);
    ~Object();
    bool operator==(const Object &obj) const;
    const std::string & get_filename() const;
    Program * get_program() const;
    void update(const Config &config);
    bool contains(const std::string &symbol) const;
    bool depends_on(const Object &obj) const;

private:
    void compile(const Config &config);
    void extract_symbols(const Config &config);

    std::string cpp_file;
    std::string obj_file;
    std::vector<std::string> defined_symbols;
    std::vector<std::string> undefined_symbols;

    Program *program;
};
