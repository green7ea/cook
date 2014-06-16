#pragma once

#include <functional>
#include <vector>
#include <algorithm>

template <typename Type>
Type reduce(const std::vector<Type> &list,
            const std::function<Type (const Type &, const Type &)> &func)
{
    Type res = list[0];
    auto iter = list.begin();
    ++iter;

    for (; iter != list.end(); ++iter)
    {
        res = func(res, *iter);
    }

    return res;
}

template <typename Type>
void filter(std::vector<Type> &list,
            const std::function<bool (const Type &)> &func)
{
    // Why would I not want to erase it? Seriously?
    list.erase(std::remove_if(list.begin(), list.end(), func),
               list.end());
}

template <typename Input, typename Output>
std::vector<Output> map(const std::vector<Input> &list,
                        const std::function<Output (const Input &)> &func)
{
    std::vector<Output> res;

    for (const Input &item: list)
    {
        res.push_back(func(item));
    }

    return res;
}

template <typename Type>
void mapBang(std::vector<Type> &list,
             const std::function<void (Type &)> &func)
{
    for (Type &item: list)
    {
        func(item);
    }
}

std::vector<std::string> tokenize(const std::string &input,
                                  std::vector<char> seperator = {' ', '\t', '\n'});
std::string read_all_fd(int fd);

std::vector<std::string> operator+(const std::vector<std::string> &a,
                                   const std::vector<std::string> &b);
