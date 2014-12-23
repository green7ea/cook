#include "file.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <regex>

#include "UnistdError.h"
#include "utils.h"

std::string get_pwd()
{
    char * pwd = get_current_dir_name();
    const std::string spwd = pwd;
    free(pwd);

    return spwd;
}

std::string get_extension(const std::string &filename)
{
    std::regex extension("\\.[^\\.]*$");

    std::sregex_iterator iter(filename.begin(),
                              filename.end(),
                              extension);

    if (iter != std::sregex_iterator())
    {
        return iter->str();
    }

    return "";
}

std::string strip_extension(const std::string &filename)
{
    std::regex extension("\\.[^\\.]*$");
    return std::regex_replace(filename, extension, "");
}

std::string to_abs_path(const std::string &filepath)
{
    if (filepath.length() < 1 || filepath[0] == '/')
    {
        return filepath;
    }
    const std::string pwd = get_pwd();
    const std::string path = pwd + "/" + filepath;

    auto path_parts = tokenize(path, {'/'});

    filter<std::string>(path_parts,
                        [](const std::string &item)
                        {
                            return (item == ".");
                        });

    for (size_t i = 0; i < path_parts.size() - 1; ++i)
    {
        if (path_parts[i + 1] == "..")
        {
            for (size_t j = i; j < path_parts.size() - 2; ++j)
            {
                path_parts[j] = path_parts[j + 2];
            }
            path_parts.resize(path_parts.size() - 2);
        }
    }

    return "/" + reduce<std::string>(
        path_parts,
        [](const std::string &a,
           const std::string &b) -> std::string
        {
            return a + "/" + b;
        });
}

std::string to_relative_path(const std::string &filepath)
{
    if (filepath.length() < 1 ||
        filepath[0] != '/')
    {
        return filepath;
    }
    const std::string pwd = get_pwd();

    auto file_parts = tokenize(filepath, {'/'});
    auto pwd_parts = tokenize(pwd, {'/'});

    const size_t file_size = file_parts.size();
    const size_t pwd_size = pwd_parts.size();
    size_t common;
    for (common = 0;
         (common < file_size &&
          common < pwd_size &&
          file_parts[common] == pwd_parts[common]);
         ++common);

    const size_t num_up_dir = pwd_size - common;

    std::vector<std::string> res;
    for (size_t i = 0; i < num_up_dir; ++i)
    {
        res.push_back("..");
    }
    for (size_t i = common; i < file_size; ++i)
    {
        res.push_back(file_parts[i]);
    }

    return reduce<std::string>(
        res,
        [](const std::string &a,
           const std::string &b) -> std::string
        {
            return a + "/" + b;
        });
}

time_t modification_time(const std::string &filepath)
{
    struct stat st;

    if (stat(filepath.c_str(), &st) != 0)
    {
        std::string message = "File " + filepath + " stat";
        throw UnistdError(message, errno);
    }

    return st.st_mtim.tv_sec;
}
