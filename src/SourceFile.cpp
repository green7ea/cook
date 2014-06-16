#include "SourceFile.h"

#include "FileDispatcher.h"
#include "Config.h"
#include "Process.h"
#include "Header.h"
#include "utils.h"

SourceFile::SourceFile(const std::string &filename)
    : filename(filename),
      object(new Object(filename))
{

}

SourceFile::~SourceFile()
{

}

const std::string & SourceFile::get_filename() const
{
    return filename;
}

std::shared_ptr<Object> SourceFile::get_object() const
{
    return object;
}

void SourceFile::update(const Config &config)
{
    headers.clear();
    extract_headers(config);
    object->update(config);
}

bool SourceFile::update(const Config &config, const std::string &changed_file)
{
    if (filename == changed_file)
    {
        update(config);
        return true;
    }

    for (auto &header: headers)
    {
        if (header.get_filename() == changed_file)
        {
            header.update(config);
            return true;
        }
    }

    return false;
}

void SourceFile::extract_headers(const Config &config)
{
    const std::string compiler = config.compiler;
    const std::vector<std::string> flags = config.flags;
    std::vector<std::string> args = {compiler};
    args = args + flags;
    args = args + std::vector<std::string>({"-MM", filename});

    Process header_extract = Process(args);
    auto header_files = tokenize(header_extract.output());

    filter<std::string>(
        header_files,
        [](const std::string &item) -> bool
        {
            if (item.size() < 2)
            {
                return true;
            }
            int pos = item.size() - 1;
            return (item[pos--] != 'h' ||
                    item[pos--] != '.');
        });

    headers.clear();
    for (const auto &header_file: header_files)
    {
        headers.push_back(Header(header_file, object));
    }
}
