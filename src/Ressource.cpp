#include "Ressource.h"

Ressource::Ressource(const std::function<void ()> &cleanup_function)
    : cleanup_function(cleanup_function)
{

}

Ressource::~Ressource()
{
    cleanup_function();
}
