#pragma once

#include <functional>

class Ressource
{

public:
    Ressource(const std::function<void ()> &cleanup_function);
    ~Ressource();

private:
    std::function<void ()> cleanup_function;
};
