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

    processed += get_string_ns(smh->stime) + " num: " + std::to_string(smh->msg_seq_num)
                 + ", size: " + std::to_string(smh->msg_size) + " flags " + std::to_string(smh->msg_flags);

    std::bitset<16> m_flags = smh->msg_flags;
    void *ndata = reinterpret_cast<char*>(data_ptr) + sizeof(smb_mdata_header_t);
    size_t nsize = smh->msg_size - sizeof(smb_mdata_header_t);
    if (m_flags.test(3))
    {
        processed += " Type: inc;";
        if (unlikely(smh->msg_size < sizeof(smb_mdata_header_t) + sizeof(smb_inc_header_t)))
        {
            processed += "size mismatch, skip";
            return;
        }
        process_inc(ndata, nsize);
    }
    else
    {
        processed += " Type: snp;";
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
    processed += "transact time: " + get_string_ns(sih->transact_time);

    int64_t nlen = len - sizeof(smb_inc_header_t);
    void *ndata = reinterpret_cast<char*>(data_ptr) + sizeof(smb_inc_header_t);

    while (nlen > 0)
    {
        if (unlikely(nlen < sizeof(smb_sbe_header_t)))
        {
            processed += "size mismatch, skip";
            return;
        }

        uint16_t processed_len = process_sbe(ndata, nlen); /* subtracting uint16_t from int64_t */
        ndata = reinterpret_cast<char*>(data_ptr) + processed_len;
        nlen -= processed_len;
    }
}

uint16_t
smb_decoder_t::process_sbe(void *data_ptr, size_t len) noexcept
{
    smb_sbe_header_t *ssh = reinterpret_cast<smb_sbe_header_t*>(data_ptr);
    if (!msg_type_valid(ssh->template_id))
    {
        /* just skip, without logging any messages */
        // processed += "template_id unrecognized:" + std::to_string(ssh->template_id)
                    //  + "unable to parse SBE, skip. ";
        return ssh->block_len;
    }

    switch (static_cast<msg_type_e>(ssh->template_id))
    {
        case msg_type_e::ORDER_UPDATE_MSG:
            process_sbe_msg<smb_order_update_msg_t>(data_ptr, len);
            break;
        case msg_type_e::ORDER_EXEC_MSG:
            process_sbe_msg<smb_order_exec_msg_t>(data_ptr, len);
            break;
        case msg_type_e::ORDER_BOOK_SNAPSHOT_MSG:
            process_sbe_msg<smb_order_booksnp_msg_t>(data_ptr, len);
            break;

        default:
            processed += "unable to parse SBE, skip";
            break;
    }

    return ssh->block_len;
}

// void
// smb_decoder_t::process_snp(void *data_ptr, size_t len) noexcept
// {
//     smb_sbe_header_t *sih = reinterpret_cast<smb_sbe_header_t*>(data_ptr);
//     processed += "block len: " + std::to_string(sih->block_len);

//     if (unlikely(len < sizeof(smb_sbe_header_t)))
//     {
//         processed += "size mismatch, skip";
//         return;
//     }

//     size_t nlen = sih->block_len;
// }

std::string
smb_decoder_t::dump()
{  return std::move(processed); }