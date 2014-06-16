#pragma once

#include <memory>
#include <vector>

#include "Config.h"

class Object;

class Program
{

public:
    Program(Object *obj);
    void resolve_dependencies(const std::vector< std::shared_ptr<Object> > &objs);
    bool depends_on(const Object &obj);
    void link(const Config &config);

private:
    std::string program_name;
    Object *main_obj;
    std::vector< std::shared_ptr<Object> > dependencies;
};
