#include <string>
#include "net_types.hpp"
#include "net.hpp"

std::string inet_ntoa_util(const uint8_t (&ip_num)[IPv4_ALEN]) noexcept
{
    std::string tmp = std::to_string(ip_num[0]);
    for (int i = 1; i < IPv4_ALEN; i++)
        tmp += ":" + std::to_string(ip_num[i]);

    return tmp;
}

std::string inet_ntop_util(const uint8_t (&ip_num)[IPv6_ALEN]) noexcept
{
    std::string tmp = std::to_string(ip_num[0]);
    for (int i = 1; i < IPv6_ALEN; i++)
        tmp += ":" + std::to_string(ip_num[i]);

    return tmp;
}