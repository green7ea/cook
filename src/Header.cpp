#include "Header.h"

Header::Header(const std::string &filename,
               const std::shared_ptr<Object> &obj)
    : filename(filename),
      obj(obj)
{

}

Header::~Header()
{

}

void Header::update(const Config &config)
{
    obj->update(config);
}

const std::string & Header::get_filename() const
{
    return filename;
}
