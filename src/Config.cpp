#include "Config.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "UnistdError.h"
#include "Error.h"
#include "Ressource.h"
#include "Process.h"
#include "utils.h"

static std::string getValue(json_t *root, const std::string &name)
{
    json_t *res = json_object_get(root, name.c_str());
    if (!res)
    {
        return "";
    }

    if (json_is_string(res))
    {
        return json_string_value(res);
    }

    return "";
}

static std::vector<std::string> getArray(json_t *root, const std::string &name)
{
    json_t *res = json_object_get(root, name.c_str());
    if (!res)
    {
        return {};
    }

    if (json_is_array(res))
    {
        std::vector<std::string> result;

        for (int i = 0, size = json_array_size(res);
             i < size; ++i)
        {
            json_t *elem = json_array_get(res, i);

            if (json_is_string(elem))
            {
                result.push_back(json_string_value(elem));
            }
        }

        return result;
    }

    return {};
}

Config::Config(const std::string &filename)
{
    json_error_t js_error;

    int fd = open(filename.c_str(), O_RDONLY);
    if (fd < 0)
    {
        throw UnistdError("Openning " + filename, errno);
    }
    Ressource fd_cleanup([&]() { close(fd); });
    std::string text = read_all_fd(fd);

    json_t *root = json_loads(text.c_str(), 0, &js_error);
    Ressource root_ressource([=]()
                             {
                                 json_decref(root);
                             });

    if (!root)
    {
        char buffer[1024];
        snprintf(buffer, 1024, "Couldn't read JSON (%i): %s\n",
                 js_error.line, js_error.text);
        throw Error(buffer);
    }

    if (!json_is_object(root))
    {
        throw Error("Badly formed file " + filename);
    }

    compiler = getValue(root, "compiler");
    nm = getValue(root, "nm");
    flags = getArray(root, "flags");
    libs = getArray(root, "libs");
    packages = getArray(root, "packages");
    defines = getArray(root, "defines");

    pkg_config();
}

void Config::pkg_config()
{
    for (const auto &pkg: packages)
    {
        Process pkg_cflags({"pkg-config", "--cflags", pkg});
        Process pkg_libs({"pkg-config", "--libs", pkg});

        pkg_cflags.run();
        pkg_libs.run();

        flags = flags + tokenize(pkg_cflags.output());
        libs = libs + tokenize(pkg_libs.output());
    }
}
