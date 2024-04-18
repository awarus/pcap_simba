#ifndef DECODER_LIB
#define DECODER_LIB

#include <string>
#include "../net/net_types.hpp"
#include "../net/net_utils.hpp"

std::string unpack_link(void *pdata, size_t psize, uint16_t type_val) noexcept;
std::string unpack_nt(void *pdata, size_t size, uint32_t type_val) noexcept;
std::string unpack_trt(void *pdata, size_t size, uint32_t type_val) noexcept;

#endif