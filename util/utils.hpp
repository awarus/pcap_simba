#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>
#include <string>

#include "../decode/simba_msg.hpp"

#define likely(x)    __builtin_expect (!!(x), 1)
#define unlikely(x)  __builtin_expect (!!(x), 0)

std::string get_string_ns(uint64_t ns_time);
double get_decimal_val(decimal5_t dec);
void display_percent_done(std::streampos currentPos, std::streampos totalSize);

#endif