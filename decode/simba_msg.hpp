#ifndef SIMBA_MSG_TYPES
#define SIMBA_MSG_TYPES

#include <cstdint>
#include <unordered_set>

/* Decimal5 type */
struct decimal5_t
{
    int64_t mantissa;
    int8_t  exp;
} __attribute__ ((__packed__));
static_assert(sizeof(decimal5_t) == 9);

/* groupSize type */
struct gr_size
{
    uint16_t block_len;
    uint8_t  num_in_group;
}__attribute__ ((__packed__));
static_assert(sizeof(gr_size) == 3);

/* Market Data Packet Header */
struct smb_mdata_header_t
{
    uint32_t msg_seq_num; /* Package sequence number */
    uint16_t msg_size;    /* The length of the entire message in bytes (including header) */
    uint16_t msg_flags;   /* Message flags */
    uint64_t stime;       /* UTC time when the packet was sent by the gateway (ns, unix epoch */
} __attribute__ ((__packed__));
static_assert(sizeof(smb_mdata_header_t) == 16);

/* Incremental Packet Header */
struct smb_inc_header_t
{
    uint64_t transact_time;    /* UTC time, beginning of transaction processing (ns, unix epoch) */
    uint32_t xchg_trade_sess;  /* The length of the entire message in bytes (including header), opt */
} __attribute__ ((__packed__));
static_assert(sizeof(smb_inc_header_t) == 12);

/* SBE Header */
struct smb_sbe_header_t
{
    uint16_t block_len;    /* Length of the message, w/out SBE header and NoMDEntries field group */
    uint16_t template_id;  /* Message template identifier */
    uint16_t schema_id;    /* Message schema identifier */
    uint16_t version;      /* Message schema version */
} __attribute__ ((__packed__));
static_assert(sizeof(smb_sbe_header_t) == 8);

/* Repeating group dimensions header */
struct smb_repeatg_header_t
{
    uint16_t block_len;       /* Field group size */
    uint8_t  num_in_group;    /* Number of field groups  */
} __attribute__ ((__packed__));
static_assert(sizeof(smb_repeatg_header_t) == 3);

/* OrderUpdate, msg_id = 15 */
struct smb_order_update_msg_t
{
    int64_t    md_entry_id;      /* Order ID */
    decimal5_t md_entry_px;      /* Order price */
    int64_t    md_entry_size;    /* Order volume */
    uint64_t   md_flags;         /* Order type, bitmask */
    uint64_t   md_flags2;        /* Extension of md_flags, bitmask */
    int32_t    sec_id;           /* Instrument numeric code. */
    uint32_t   rpt_seq;          /* Incremental refresh sequence number */
    uint8_t    md_update_action; /* Incremental refresh type: '0' - New, '2' - delete */
    char       md_entry_type;    /* Record type */
} __attribute__ ((__packed__));
static_assert(sizeof(smb_order_update_msg_t) == 51);

/* OrderExecution, msg_id = 16 */
struct smb_order_exec_msg_t
{
    int64_t    md_entry_id;      /* Order ID */
    decimal5_t md_entry_px;      /* Order price, none if tech trade */
    int64_t    md_entry_size;    /* Remaining quantity in the order, none if tech trade  */
    decimal5_t last_px;          /* Trade Price */
    int64_t    last_qty;         /* Trade volume */
    int64_t    trade_id;         /* Trade ID */
    uint64_t   md_flags;         /* Trade type, bitmask */
    uint64_t   md_flags2;        /* Extension of md_flags, bitmask */
    int32_t    sec_id;           /* Instrument numeric code. */
    uint32_t   rpt_seq;          /* Incremental refresh sequence number */
    uint8_t    md_update_action; /* Incremental refresh type: '0' - New, '2' - delete */
    char       md_entry_type;    /* Record type */
} __attribute__ ((__packed__));
static_assert(sizeof(smb_order_exec_msg_t) == 76);

/* OrderBookSnapshot, msg_id = 17 */
struct smb_order_booksnp_msg_t
{
    int32_t    sec_id;                /* Instrument numeric code. */
    uint32_t   last_msg_seq_nproc;    /* The 'MsgSeqNum' of the last inc message sent */
    uint32_t   rpt_seq;               /* Incremental refresh sequence number */
    uint32_t   xchg_trade_session_id; /* Trading session ID */
    gr_size    num_md_entries;        /* Number of 'MDEntry' records in the current message */
    int64_t    md_entry_id;           /* Order ID  */
    uint64_t   trans_time;            /* The start time of the event processing. UTC, ns unix epoch*/
    decimal5_t md_entry_px;           /* Order price */
    int64_t    md_entry_size;         /* Remaining quantity in the order */
    int64_t    trade_id;              /* Trade ID */
    uint64_t   md_flags;              /* Trade type, bitmask */
    uint64_t   md_flags2;             /* Extension of md_flags, bitmask */
    char       md_entry_type;         /* Record type */
} __attribute__ ((__packed__));
static_assert(sizeof(smb_order_booksnp_msg_t) == 77);

enum class msg_type_e : uint16_t
{
    ORDER_UPDATE_MSG = 15,
    ORDER_EXEC_MSG = 16,
    ORDER_BOOK_SNAPSHOT_MSG = 17
};

static std::unordered_set<uint16_t> msg_type_set = {
    15, /* ORDER_UPDATE_MSG */
    16, /* ORDER_EXEC_MSG */
    17  /* ORDER_BOOK_SNAPSHOT_MSG */
};

static bool msg_type_valid(uint16_t mt)
{  return msg_type_set.count(mt) > 0; }

#endif /* SIMBA_MSG_TYPES */