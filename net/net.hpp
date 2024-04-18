#ifndef INCLUDE_NET
#define INCLUDE_NET

#include <unordered_map>
#include <string>

#include "net_types.hpp"

struct unpack_data_t
{
    void    *data;
    size_t   nsize;
    uint32_t ntype;
};

/* despite the possible overhead, virtual functions are less tedious than switch statements for all types */
class g_header {
public:
    virtual ~g_header() = default;

    /* get data field */
    virtual void *get_data() const = 0;

    /* set data and get type field back */
    virtual unpack_data_t set_data(void *ndata, size_t nsize) = 0;

    /* get type field */
    virtual uint32_t get_ntype() const = 0;

    /* dump the header fields into the string */
    virtual std::string dump_fields() const = 0;

    /* get pointer to the header struct */
    virtual char *get_ptr() = 0;
};

/* Link types */
class eth_stor_header_t : public g_header {
public:
    void *get_data() const override
        { return hdr->data; }
    unpack_data_t set_data(void *ndata, size_t nsize) override;
    uint32_t get_ntype() const override
        { return hdr->ether_type; }
    std::string dump_fields() const override;
    char *get_ptr() override
        { return reinterpret_cast<char*>(hdr); }
private:
    eth_header_t *hdr;
    size_t        total_size;
};

/* Net types */
class ipv4_stor_header_t : public g_header {
public:
    void *get_data() const override
        { return hdr->data; }
    unpack_data_t set_data(void *ndata, size_t nsize) override;
    uint32_t get_ntype() const override
        { return hdr->protocol; }
    std::string dump_fields() const override;
    char *get_ptr() override
        { return reinterpret_cast<char*>(hdr); }
private:
    ipv4_header_t *hdr;
    size_t         total_size;
};
class ipv6_stor_header_t : public g_header {
public:
    void *get_data() const override
        { return hdr->data; }

    /* TBD */
    unpack_data_t set_data(void *ndata, size_t nsize) override
        { return {}; }
    uint32_t get_ntype() const override
        { return 0; }

    /* TBD */
    std::string dump_fields() const override
        { return ""; }
    char *get_ptr() override
        { return reinterpret_cast<char*>(hdr); }
private:
    ipv6_header_t *hdr;
    size_t         total_size;
};
class arp_stor_header_t : public g_header {
public:
    void *get_data() const override
        { return hdr->data; }

    /* TBD */
    unpack_data_t set_data(void *ndata, size_t nsize) override
        { return {}; }
    uint32_t get_ntype() const override
        { return hdr->protocolType; }
    /* TBD */
    std::string dump_fields() const override
        { return ""; }
    char *get_ptr() override
        { return reinterpret_cast<char*>(hdr); }
private:
    arp_header_t *hdr;
    size_t        total_size;
};

/* Transport types */
class icmp_stor_header_t : public g_header {
public:
    void *get_data() const override
        { return hdr->data; }

    /* TBD */
    unpack_data_t set_data(void *ndata, size_t nsize) override
        { return {}; }
    uint32_t get_ntype() const override
        { return hdr->type; }

    /* TBD */
    std::string dump_fields() const override
        { return ""; }
    char *get_ptr() override
        { return reinterpret_cast<char*>(hdr); }
private:
    icmp_header_t *hdr;
    size_t        total_size;
};
class tcp_stor_header_t : public g_header {
public:
    void *get_data() const override
        { return hdr->data; }

    /* TBD */
    unpack_data_t set_data(void *ndata, size_t nsize) override
        { return {}; }
    uint32_t get_ntype() const override
        { return 0; }

    /* TBD */
    std::string dump_fields() const override
        { return ""; }
    char *get_ptr() override
        { return reinterpret_cast<char*>(hdr); }
private:
    tcp_header_t *hdr;
    size_t        total_size;
};
class udp_stor_header_t : public g_header {
public:
    void *get_data() const override
        { return hdr->data; }
    unpack_data_t set_data(void *ndata, size_t nsize) override;
    uint32_t get_ntype() const override
        { return 0; }
    std::string dump_fields() const override;
    char *get_ptr() override
        { return reinterpret_cast<char*>(hdr); }
private:
    udp_header_t *hdr;
    size_t        total_size;
};

static std::unordered_map<uint16_t, std::shared_ptr<g_header>> link_types = {
    {0x0, std::make_shared<eth_stor_header_t>()}, /* Loopback */
    {0x1, std::make_shared<eth_stor_header_t>()}  /* Ethernet */
};

static inline bool valid_ltype(uint32_t lt) noexcept
{ return link_types.count(lt) > 0; }

static auto get_link_header(uint32_t lt) noexcept
{
    assert(link_types.count(lt) > 0);
    return link_types[lt].get();
}

static std::unordered_map<uint32_t, std::shared_ptr<g_header>> net_types = {
    {0x0800, std::make_shared<ipv4_stor_header_t>()}, /* IPv4 */
    {0x86DD, std::make_shared<ipv6_stor_header_t>()}, /* IPv6 */
    {0x0806, std::make_shared<arp_stor_header_t>()}   /* ARP */
};

static inline bool valid_nettype(uint32_t nt) noexcept
{ return net_types.count(nt) > 0; }

static auto get_net_header(uint32_t nt) noexcept
{
    assert(net_types.count(nt) > 0);
    return net_types[nt].get();
}

static std::unordered_map<uint32_t, std::shared_ptr<g_header>> trt_types = {
    {0x01, std::make_shared<icmp_stor_header_t>()}, /* ICMP */
    {0x06, std::make_shared<tcp_stor_header_t>()},  /* TCP */
    {0x11, std::make_shared<udp_stor_header_t>()}   /* UPD */
};

static inline bool valid_trttype(uint32_t trt) noexcept
{ return trt_types.count(trt) > 0; }

static auto get_trt_header(uint32_t trt) noexcept
{
    assert(trt_types.count(trt) > 0);
    return trt_types[trt].get();
}

#endif