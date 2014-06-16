#include "utils.h"

#include <unistd.h>

#include <algorithm>

#include "UnistdError.h"
#include "Error.h"

std::vector<std::string> tokenize(const std::string &input,
                                  std::vector<char> seperator)
{
    std::vector<std::string> res;

    std::string collector = "";
    for (char c: input)
    {
        if (std::find(seperator.begin(),
                      seperator.end(),
                      c) != seperator.end())
        {
            res.push_back(collector);
            collector = "";
        }
        else
        {
            collector += c;
        }
    }
    if (collector != "")
    {
        res.push_back(collector);
    }

    filter<std::string>(
        res,
        [&](const std::string &item) -> bool
        {
            for (char c: item)
            {
                if ( !(std::find(seperator.begin(),
                                 seperator.end(),
                                 c) != seperator.end()) )
                {
                    return false;
                }
            }
            return true;
        });

    return res;
}

std::string read_all_fd(int fd)
{
    if (fd < 0)
    {
        throw Error("Couldn't open fd");
    }

    std::string result = "";
    char buffer[2048];

    for (ssize_t rd = read(fd, buffer, 2048);
         rd != 0;
         rd = read(fd, buffer, 2048))
    {
        if (rd < 0)
        {
            throw UnistdError("Reading FD", errno);
        }
        result += std::string(buffer, rd);
    }

    return result;
}

std::vector<std::string> operator+(const std::vector<std::string> &a,
                                   const std::vector<std::string> &b)
{
    std::vector<std::string> res = a;
    res.insert(res.end(), b.begin(), b.end());
    return res;
}
