#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <fstream>
#include <time.h>
#include <sstream>

#include "parser.hpp"
#include "net/net.hpp"
#include "util/utils.hpp"
#include "decode/net_decoder.hpp"

std::string get_pcap_timestamp(pcap_packet_t *p_packet) noexcept
{
    time_t time_ts = p_packet->ts_s;
    struct tm *pts = gmtime(&time_ts);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << pts->tm_hour << ":"
       << std::setfill('0') << std::setw(2) << pts->tm_min << ":"
       << std::setfill('0') << std::setw(2) << pts->tm_sec << "."
       << p_packet->ts_ms << "; ";
    return ss.str();
}

/* Parsing PCAP packet header here */
inline void parse_pcap_packet(pcap_packet_t *p_packet, std::ofstream &os, uint16_t ltype) noexcept
{
#ifdef PCAP_VERBOSE
    os << get_pcap_timestamp(p_packet) << ": ";
#endif
    os << unpack_link(p_packet->data, p_packet->p_len_captured, ltype) << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "No valid input pcap filename was provided" << std::endl;
        return -ENOATTR;
    }

    std::string pcap_fname = argv[1];
    std::string save_fname = argc == 3? argv[2] : pcap_fname + ".out";

    std::cout << "save fname: " << save_fname << std::endl;

    std::ofstream out_stream(save_fname);
    if (!out_stream.is_open())
    {
        std::cout << "failed to open " << save_fname << std::endl;
        return -ENOENT;
    }

    std::ifstream in_stream(pcap_fname, std::ios_base::in | std::ios_base::binary);
    if (!in_stream.is_open())
    {
        std::cout << "failed to open " << pcap_fname << std::endl;
        return -ENOENT;
    }

    char *packet_block = new char[sizeof(pcap_fheader_t)];
    in_stream.read(packet_block, sizeof(pcap_fheader_t));

    pcap_fheader_t pheader;
    memcpy(&pheader, packet_block, sizeof(pcap_fheader_t));
    if (pheader.magic != magic_pcap_ms && pheader.magic != magic_pcap_ns) /* check the magic number */
    {
        std::cout << "pcap magic number mismatch, is it big-endian pcap file?" << std::endl;
        delete [] packet_block;
        return -ENOENT;
    }

    if (!valid_ltype(pheader.ltype))
    {
        std::cerr <<  "unrecognized link type " << pheader.ltype << std::endl;
        delete [] packet_block;
        return -EFTYPE;
    }

    packet_block = reinterpret_cast<char*>(realloc(packet_block, sizeof(pcap_packet_t)));
    char *data_block = new char[def_packet_size];

    /* Main loop of reading pcap packets */
    while (!in_stream.eof())
    {
        in_stream.read(packet_block, sizeof(pcap_packet_t) - sizeof(void*));
        pcap_packet_t *p_packet = reinterpret_cast<pcap_packet_t*>(packet_block);
        if ((unlikely(p_packet->p_len_captured > def_packet_size)))
            data_block = (char*)realloc(data_block, p_packet->p_len_captured);

        in_stream.read(data_block, p_packet->p_len_captured);
        p_packet->data = data_block;
        parse_pcap_packet(reinterpret_cast<pcap_packet_t*>(packet_block), out_stream, pheader.ltype);
    }

    delete [] data_block;
    delete [] packet_block;

    return 0;
}