#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

#include "utils.hpp"

void display_percent_done(std::streampos currentPos, std::streampos totalSize)
{
    double progress = static_cast<double>(currentPos) / totalSize * 100.0;
    std::cout << "\r";
    std::cout << "Parsing progress: " << std::fixed << std::setprecision(2) << progress << "%" << std::flush;
}

std::string get_string_ns(uint64_t ns_time)
{
    using namespace std::chrono;

    time_point<system_clock, nanoseconds> time_ns{nanoseconds{ns_time}};
    time_point<system_clock, microseconds> time_us{time_point_cast<microseconds>(time_ns)};

    time_t time_ts = system_clock::to_time_t(time_us);
    struct tm *pts = gmtime(&time_ts);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << pts->tm_hour << ":"
        << std::setfill('0') << std::setw(2) << pts->tm_min << ":"
        << std::setfill('0') << std::setw(2) << pts->tm_sec << "."
        << duration_cast<milliseconds>(time_ns.time_since_epoch() % 1s).count()
        << time_ns.time_since_epoch().count() % 1000000;

    return ss.str();
}

double get_decimal_val(decimal5_t dec)
{
    double result = dec.mantissa * pow(10, dec.exp);
    return static_cast<uint64_t>(result);
}