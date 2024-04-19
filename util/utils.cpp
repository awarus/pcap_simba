#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

#include "utils.hpp"

void display_percent_done(std::streampos currentPos, std::streampos totalSize) noexcept
{
    double progress = static_cast<double>(currentPos) / totalSize * 100.0;
    std::cout << "\r";
    std::cout << "Parsing progress: " << std::fixed << std::setprecision(2) << progress << "%" << std::flush;
}

std::string get_string_ns(uint64_t ns_time) noexcept
{
    using namespace std::chrono;

    time_point<system_clock, nanoseconds> time_ns{nanoseconds{ns_time}};
    time_point<system_clock, microseconds> time_us{time_point_cast<microseconds>(time_ns)};

    time_t time_ts = system_clock::to_time_t(time_us);
    struct tm *pts = gmtime(&time_ts);

    char stemp[50] = "";
    snprintf(stemp, 50, "%d2d:%d2d:%d2d.%lld%lld",
             pts->tm_hour, pts->tm_min, pts->tm_sec,
             duration_cast<milliseconds>(time_ns.time_since_epoch() % 1s).count(),
             time_ns.time_since_epoch().count() % 1000000);

    return stemp;
}
