#ifndef PARSER_CONSTANTS
#define PARSER_CONSTANTS

#include <cstdint>

constexpr uint32_t magic_pcap_ms   = 0xA1B2C3D4;
constexpr uint32_t magic_pcap_ns   = 0xA1B23C4D;
constexpr uint32_t def_packet_size = 2000;

struct pcap_fheader_t {
    uint32_t  magic;          /* Magic Number */
    uint16_t  major_v;        /* Major Version */
    uint16_t  minor_v;        /* Minor Version */
    uint64_t  resvd_;         /* Reserved */
    uint32_t  snap_len;       /* Maximum number of octets captured from each packet */
    uint16_t  fcs;            /* reserved + FCS len */
    uint16_t  ltype;          /* Link type field, refer to the http://www.tcpdump.org/linktypes.html */
} __attribute__ ((__packed__));
static_assert(sizeof(pcap_fheader_t) == 24);

struct pcap_packet_t {
    uint32_t  ts_s;           /* Timestamp in seconds */
    uint32_t  ts_ms;          /* Timestamp (microseconds or nanoseconds) */
    uint32_t  p_len_captured; /* Captured Packet Length */
    uint32_t  p_len_original; /* Original Packet Length */
    void     *data;
} __attribute__ ((__packed__));
static_assert(sizeof(pcap_packet_t) == 24);

#endif /* PARSER_CONSTANTS */