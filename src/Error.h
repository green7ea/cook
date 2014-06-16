#pragma once

#include <exception>
#include <string>

class Error: public std::exception
{

public:
    Error(const std::string &message);
    virtual const char * what() const noexcept override;

private:
    std::string message;
};
