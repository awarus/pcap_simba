#include <cstdint>
#include "net_types.hpp"
#include "net_utils.hpp"
#include "net.hpp"

#include <iostream>

unpack_data_t
eth_stor_header_t::set_data(void *ndata, size_t nsize)
{
    hdr = reinterpret_cast<eth_header_t*>(ndata);

    assert(hdr != nullptr);
    assert(nsize + sizeof(void*) > sizeof(eth_header_t) + ETH_CRCLEN);

    total_size = nsize;
    hdr->data = static_cast<char*>(hdr->data) - sizeof(void*);

    size_t under_size = nsize + sizeof(void*) - sizeof(eth_header_t) - ETH_CRCLEN;
    return {&hdr->data, under_size, ntohs_util(hdr->ether_type)};
}

std::string
eth_stor_header_t::dump_fields() const
{
    std::string src = std::to_string(hdr->ether_shost[0]);
    for (int i = 1; i < ETH_ALEN; i++)
        src += ":" + std::to_string(hdr->ether_shost[i]);

    std::string dst = std::to_string(hdr->ether_dhost[0]);
    for (int i = 1; i < ETH_ALEN; i++)
        dst += ":" + std::to_string(hdr->ether_dhost[i]);

    return "src mac: " + src + "; dst mac: " + dst + "; "
            + std::to_string(ntohs_util(hdr->ether_type)) + ";;";
}

unpack_data_t
ipv4_stor_header_t::set_data(void *ndata, size_t nsize)
{
    hdr = reinterpret_cast<ipv4_header_t*>(ndata);

    assert(hdr != nullptr);
    assert(nsize + sizeof(void*) > sizeof(ipv4_header_t));

    total_size = nsize;
    hdr->data = reinterpret_cast<char*>(hdr->data) - sizeof(void*);

    size_t under_size = ntohs_util(hdr->total_length) + sizeof(void*) - sizeof(ipv4_header_t);
    return {&hdr->data, under_size, hdr->protocol};
}

std::string
ipv4_stor_header_t::dump_fields() const
{
    return "IP src: " + inet_ntoa_util(hdr->source_ip)
            + "; dst: " + inet_ntoa_util(hdr->dest_ip)+ ", ";
}

unpack_data_t
udp_stor_header_t::set_data(void *ndata, size_t nsize)
{
    hdr = reinterpret_cast<udp_header_t*>(ndata);

    assert(hdr != nullptr);
    assert(nsize + sizeof(void*) > sizeof(udp_header_t));

    total_size = nsize;
    hdr->data = reinterpret_cast<char*>(hdr->data) - sizeof(void*);

    size_t under_size = ntohs_util(hdr->length) + sizeof(void*) - sizeof(udp_header_t);
    return {&hdr->data, nsize, 0};
}

std::string
udp_stor_header_t::dump_fields() const
{
    return "src port: " + std::to_string(ntohs_util(hdr->source_port)) +
            ", dst port: " + std::to_string(ntohs_util(hdr->dest_port)) +
            "; UDP len: " + std::to_string(ntohs_util(hdr->length));
}