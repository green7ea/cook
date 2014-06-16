#pragma once

#include <exception>
#include <string>

class UnistdError: public std::exception
{

public:
    UnistdError(const std::string &message, int error);
    virtual const char * what() const noexcept override;

private:
    std::string message;
};
