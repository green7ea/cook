#include "Object.h"

#include <regex>
#include <algorithm>

#include "file.h"
#include "utils.h"
#include "UnistdError.h"
#include "Process.h"
#include "Program.h"
#include "FileDispatcher.h"

Object::Object(const std::string &cpp_file)
    : cpp_file(cpp_file),
      obj_file(std::regex_replace(cpp_file, std::regex("\\.cpp"), ".o")),
      program(nullptr)
{

}

Object::~Object()
{
    delete program;
}

bool Object::operator==(const Object &obj) const
{
    return (obj_file == obj.obj_file);
}

const std::string & Object::get_filename() const
{
    return obj_file;
}

Program * Object::get_program() const
{
    return program;
}

void Object::update(const Config &config)
{
    compile(config);
    extract_symbols(config);
}

bool Object::contains(const std::string &symbol) const
{
    return std::binary_search(defined_symbols.begin(),
                              defined_symbols.end(),
                              symbol);
}

bool Object::depends_on(const Object &obj) const
{
    if (*this == obj)
    {
        return false;
    }

    for (const auto &undef_sym: undefined_symbols)
    {
        if (obj.contains(undef_sym))
        {
            return true;
        }
    }

    return false;
}

void Object::compile(const Config &config)
{
    const std::string compiler = config.compiler;
    const std::vector<std::string> flags = config.flags;
    std::vector<std::string> args = {compiler};
    args = args + flags;
    args = args + std::vector<std::string>({"-c", cpp_file, "-o", obj_file});

    // TODO move this logic elsewhere because this will block the
    // recompile on a header update.
    try
    {
        if (modification_time(obj_file) > modification_time(cpp_file))
        {
            return;
        }
    }
    catch(const UnistdError &e)
    {
        // One of the files wasn't found, probably the .o
    }

    printf("Compiling:\n");
    for (const auto &arg: args)
    {
        printf("%s ", arg.c_str());
    }
    printf("\n");

    Process compilation_process = Process(args);
    compilation_process.join();
}

void Object::extract_symbols(const Config &config)
{
    Process defined = Process({config.nm, "--defined-only", obj_file}) |
        Process({"awk", "{print $3}"});
    Process undefined = Process({config.nm, "-u", obj_file}) |
        Process({"awk", "{print $2}"});

    defined.run();
    undefined.run();

    defined_symbols = tokenize(defined.output());
    undefined_symbols = tokenize(undefined.output());

    std::sort(defined_symbols.begin(), defined_symbols.end());
    std::sort(undefined_symbols.begin(), undefined_symbols.end());

    if (contains("main"))
    {
        if (!program)
        {
            program = new Program(this);
        }
    }
}
