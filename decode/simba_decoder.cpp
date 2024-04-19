#include <bitset>
#include <chrono>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "simba_decoder.hpp"
#include "simba_msg.hpp"
#include "../net/net_utils.hpp"
#include "../util/utils.hpp"

void
smb_decoder_t::process(void *data_ptr, size_t len) noexcept
{
    smb_mdata_header_t *smh = reinterpret_cast<smb_mdata_header_t *>(data_ptr);

    std::bitset<16> m_flags = smh->msg_flags;
    void *ndata = reinterpret_cast<char*>(data_ptr) + sizeof(smb_mdata_header_t);
    size_t nsize = smh->msg_size - sizeof(smb_mdata_header_t); /* MsgSize, without the smb_mdata_header_t */
    if (m_flags.test(3))
    {
        if (unlikely(smh->msg_size < sizeof(smb_mdata_header_t) + sizeof(smb_inc_header_t)))
        {
            processed += "size mismatch, skip";
            return;
        }
        process_inc(ndata, nsize);
    }
    else
    {
        if (unlikely(smh->msg_size < sizeof(smb_mdata_header_t) + sizeof(smb_sbe_header_t)))
        {
            processed += "size mismatch, skip";
            return;
        }
        process_sbe(ndata, nsize);
    }
}

void
smb_decoder_t::process_inc(void *data_ptr, size_t len) noexcept
{
    smb_inc_header_t *sih = reinterpret_cast<smb_inc_header_t*>(data_ptr);

    int64_t nlen = len - sizeof(smb_inc_header_t); /* total length of the message(s) block */
    void  *ndata = reinterpret_cast<char*>(data_ptr) + sizeof(smb_inc_header_t);

    while (nlen > 0)
    {
        if (unlikely(nlen < sizeof(smb_sbe_header_t)))
        {
            processed += "size mismatch, skip";
            return;
        }

        uint16_t processed_len = process_sbe(ndata, nlen); /* should contain header + payload size */
        ndata = reinterpret_cast<char*>(data_ptr) + processed_len + sizeof(smb_sbe_header_t);
        nlen -= processed_len; /* subtracting uint16_t from int64_t */
    }
}

uint16_t
smb_decoder_t::process_sbe(void *data_ptr, size_t len) noexcept
{
    smb_sbe_header_t *ssh = reinterpret_cast<smb_sbe_header_t*>(data_ptr);
    /* just skip, without logging any messages */
    if (!msg_type_valid(ssh->template_id))
        return ssh->block_len;

    uint16_t header_s = 0;

    switch (static_cast<msg_type_e>(ssh->template_id))
    {
        case msg_type_e::ORDER_UPDATE_MSG:
            process_sbe_msg<smb_order_update_msg_t>(data_ptr, len);
            header_s += sizeof(smb_order_update_msg_t);
            break;
        case msg_type_e::ORDER_EXEC_MSG:
            process_sbe_msg<smb_order_exec_msg_t>(data_ptr, len);
            header_s += sizeof(smb_order_exec_msg_t);
            break;
        case msg_type_e::ORDER_BOOK_SNAPSHOT_MSG:
            process_sbe_msg<smb_order_booksnp_msg_t>(data_ptr, len);
            header_s += sizeof(smb_order_booksnp_msg_t);
            break;

        default:
            processed += "unable to parse SBE, skip";
            break;
    }

    return ssh->block_len + header_s;
}

std::string
smb_decoder_t::dump()
{  return std::move(processed); }