#ifndef NET_UTILS
#define NET_UTILS

#include <cstdint>
#include <string>
#include "net_types.hpp"

inline uint16_t ntohs_util(uint16_t nbytes) noexcept
{
    uint8_t byte1 = nbytes >> 8;
    uint8_t byte2 = nbytes & 0xFF;
    uint16_t ret = (byte2 << 8);
    return  ret | byte1;
}

std::string inet_ntoa_util(const uint8_t (&ip_num)[IPv4_ALEN]) noexcept;
std::string inet_ntop_util(const uint8_t (&ip_num)[IPv6_ALEN]) noexcept;

#endif