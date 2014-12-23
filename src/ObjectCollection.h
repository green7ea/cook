#pragma once

#include <memory>
#include <vector>

#include "Config.h"

class Object;

class ObjectCollection
{

public:
    ObjectCollection(const std::string &artifact_name,
                     const std::vector< std::shared_ptr<Object> > &initial_objects);
    void resolve_dependencies(const std::vector< std::shared_ptr<Object> > &objs);
    bool depends_on(const Object &obj);
    void link_into_program(const Config &config);
    void link_into_shared_object(const Config &config);

private:
    std::vector<std::string> build_object_arguments(const Config &config,
                                                    bool shared);
    std::string artifact_name;
    std::vector< std::shared_ptr<Object> > initial_objects;
    std::vector< std::shared_ptr<Object> > dependencies;
};
