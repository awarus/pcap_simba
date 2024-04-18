#include <cstdint>
#include <string>

#include "../util/utils.hpp"
#include "../net/net.hpp"
#include "../net/net_types.hpp"
#include "net_decoder.hpp"
#include "simba_decoder.hpp"
#include "../parser.hpp"

#define PCAP_VERBOSE false
#define LINK_VERBOSE false
#define NET_VERBOSE  false
#define TRT_VERBOSE  false

/* Unpack at data link layer */
std::string unpack_link(void *pdata, size_t psize, uint16_t type_val) noexcept
{
    if (unlikely(!valid_ltype(type_val)))
        return "unable to parse link with type: " + std::to_string(type_val);

    auto header = get_link_header(type_val);
    unpack_data_t ud = header->set_data(pdata, psize);

    std::string ret = "";
#if LINK_VERBOSE
    ret = header->dump_fields();
#endif

    return ret + unpack_nt(ud.data, ud.nsize, ud.ntype);
}

std::string unpack_nt(void *pdata, size_t psize, uint32_t type_val) noexcept
{
    if (unlikely(!valid_nettype(type_val)))
        return "unable to parse net with type: " + std::to_string(type_val);

    auto header = get_net_header(type_val);
    unpack_data_t ud = header->set_data(pdata, psize);

    std::string ret = "";
#if NET_VERBOSE
    ret = header->dump_fields();
#endif

    return ret + unpack_trt(ud.data, ud.nsize, ud.ntype);
}

std::string unpack_trt(void *pdata, size_t psize, uint32_t type_val) noexcept
{
    if (unlikely(!valid_trttype(type_val)))
        return "unable to parse ttr with type: " + std::to_string(type_val);

    auto header = get_trt_header(type_val);
    unpack_data_t ud = header->set_data(pdata, psize);

    std::string ret = "";
#if TRT_VERBOSE
    ret = header->dump_fields();
#endif

    smb_decoder_t smd_dec;
    smd_dec.process(ud.data, ud.nsize);
    return ret + smd_dec.dump();
}