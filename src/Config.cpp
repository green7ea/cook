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

static std::string get_value(json_t *root, const std::string &name)
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

static std::vector<std::string> get_array(json_t *root, const std::string &name)
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

static std::vector<Library> get_libraries(json_t *root, const std::string &name)
{
    json_t *shared_map = json_object_get(root, name.c_str());
    if (!shared_map)
    {
        return {};
    }

    std::vector<Library> shared_map_res;
    if (json_is_object(shared_map))
    {
        const char *key;
        json_t *value;

        json_object_foreach(shared_map, key, value)
        {
            if (json_is_array(value))
            {
                Library shared_object;
                shared_object.name = key;

                for (int i = 0, size = json_array_size(value);
                     i < size; ++i)
                {
                    json_t *elem = json_array_get(value, i);

                    if (json_is_string(elem))
                    {
                        shared_object.objects.push_back(json_string_value(elem));
                    }
                }
                shared_map_res.push_back(shared_object);
            }
        }
    }

    return shared_map_res;
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

    compiler = get_value(root, "compiler");
    nm = get_value(root, "nm");
    flags = get_array(root, "flags");
    libs = get_array(root, "libs");
    packages = get_array(root, "packages");
    defines = get_array(root, "defines");
    shared = get_libraries(root, "shared");

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
