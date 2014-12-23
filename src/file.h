#pragma once

#include <string>

std::string get_extension(const std::string &filename);
std::string strip_extension(const std::string &filename);
std::string to_abs_path(const std::string &filepath);
std::string to_relative_path(const std::string &filepath);
time_t modification_time(const std::string &filepath);
