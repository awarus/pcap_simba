#ifndef TRT_TYPES
#define TRT_TYPES

#include <cstdint>
#include <cstdint>

constexpr uint32_t   ETH_ALEN = 6;
constexpr uint32_t ETH_CRCLEN = 4;

struct eth_header_t
{
    uint8_t   ether_dhost[ETH_ALEN];        /* destination eth addr */
    uint8_t   ether_shost[ETH_ALEN];        /* source ether addr    */
    uint16_t  ether_type;                   /* protocol net type    */
    void     *data;
} __attribute__ ((__packed__));
static_assert(sizeof(eth_header_t) == 22);

constexpr uint32_t IPv4_ALEN = 4;

struct ipv4_header_t
{
    uint8_t   version_ihl;          /* Version (4 bits) + Internet Header Length (4 bits) */
    uint8_t   dscp_ecn;             /* DSCP (6 bits) + Congestion Notification (2 bits) */
    uint16_t  total_length;         /* Total length of the packet */
    uint16_t  identification;       /* Identification */
    uint16_t  flags_fragment;       /* Flags (3 bits) + Fragment Offset (13 bits) */
    uint8_t   ttl;                  /* Time to Live */
    uint8_t   protocol;             /* Protocol (e.g., TCP = 6, UDP = 17) */
    uint16_t  checksum;             /* Header checksum */
    uint8_t   source_ip[IPv4_ALEN]; /* Source IP address */
    uint8_t   dest_ip[IPv4_ALEN];   /* Destination IP address */
    void     *data;
} __attribute__ ((__packed__));
static_assert(sizeof(ipv4_header_t) == 28);

constexpr uint32_t IPv6_ALEN = 16;
struct ipv6_header_t
{
    uint32_t  version_traffic_flow; /* Version (4 bits) + Traffic Class (8 bits) + Flow Label (20 bits) */
    uint16_t  payload_length;       /* Payload Length (in bytes) */
    uint8_t   next_header;          /* Next Header */
    uint8_t   hop_limit;            /* Hop Limit */
    uint8_t   source_ip[IPv6_ALEN]; /* Source IPv6 Address */
    uint8_t   dest_ip[IPv6_ALEN];   /* Destination IPv6 Address */
    void     *data;                 /* payload */
} __attribute__ ((__packed__));
static_assert(sizeof(ipv6_header_t) == 48);

struct arp_header_t
{
    uint16_t hardwareType;          /* Hardware Type */
    uint16_t protocolType;          /* Protocol Type (2 bytes) */
    uint8_t  hardwareAddrLength;    /* Hardware Address Length */
    uint8_t  protocolAddrLength;    /* Protocol Address Length */
    uint16_t operation;             /* Operation (2 bytes) */
    uint8_t  senderHardwareAddr[6]; /* Sender Hardware Address */
    uint32_t senderProtocolAddr;    /* Sender Protocol Address */
    uint8_t  targetHardwareAddr[6]; /* Target Hardware Address */
    uint32_t targetProtocolAddr;    /* Target Protocol Address */
    void     *data;                 /* payload */
} __attribute__ ((__packed__));
static_assert(sizeof(arp_header_t) == 36);

struct tcp_header_t
{
    uint16_t source_port;      /* Source port */
    uint16_t dest_port;        /* Destination port */
    uint32_t seq_number;       /* Sequence number */
    uint32_t ack_number;       /* Acknowledgment number */
    uint8_t  data_offset;      /* Data offset (4 bits) + Reserved (4 bits) */
    uint8_t  flags;            /* Flags (8 bits) */
    uint16_t window_size;      /* Window size */
    uint16_t checksum;         /* Checksum */
    uint16_t urgent_pointer;   /* Urgent pointer */
    void    *data;             /* data */
} __attribute__ ((__packed__));
static_assert(sizeof(tcp_header_t) == 28);

struct udp_header_t
{
    uint16_t source_port;      /* Source port */
    uint16_t dest_port;        /* Destination port */
    uint16_t length;           /* Length */
    uint16_t checksum;         /* Checksum */
    void    *data;             /* data */
} __attribute__ ((__packed__));
static_assert(sizeof(udp_header_t) == 16);

struct icmp_header_t
{
    uint8_t  type;          /* Type */
    uint8_t  code;          /* Code */
    uint16_t checksum;      /* Checksum */
    void     *data;         /* data */
} __attribute__ ((__packed__));
static_assert(sizeof(icmp_header_t) == 12);

#endif /* TRT_TYPES */
