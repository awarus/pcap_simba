#include <cstdint>
#include <string>

#include "simba_msg.hpp"
#include "../util/utils.hpp"
#include "../util/json.hpp"

class smb_decoder_t
{
public:
    smb_decoder_t()
        {}

    void process(void *data, size_t len) noexcept;
    void process_inc(void *data, size_t len) noexcept;
    void process_snp(void *data, size_t len) noexcept;
    uint16_t process_sbe(void *data_ptr, size_t len) noexcept;
    std::string dump();

    template<typename T>
    std::string dump_sbe_msg(void *data);

    template<>
    std::string dump_sbe_msg<smb_order_update_msg_t>(void *data)
    {
        smb_order_update_msg_t *smou = reinterpret_cast<smb_order_update_msg_t*>(data);
        json_t order_update_j;

        order_update_j.add("md_entry_id",        smou->md_entry_id);
        order_update_j.add("md_entry_px",        get_decimal_val(smou->md_entry_px));
        order_update_j.add("md_entry_size",      smou->md_entry_size);
        order_update_j.add("md_flags",           smou->md_flags);
        order_update_j.add("md_flags2",          smou->md_flags2);
        order_update_j.add("sec_id",             smou->sec_id);
        order_update_j.add("rpt_seq",            smou->rpt_seq);
        order_update_j.add("md_update_action",   smou->md_update_action);

        if (smou->md_entry_type == '0')
            order_update_j.add("md_entry_type", "Bid");
        else
            order_update_j.add("md_entry_type", "Ask");

        order_update_j.add("msg", "OrderUpdate");
        return order_update_j.to_string();
    }

    template<>
    std::string dump_sbe_msg<smb_order_exec_msg_t>(void *data)
    {
        smb_order_exec_msg_t *soem = reinterpret_cast<smb_order_exec_msg_t*>(data);
        json_t order_exec_j;

        order_exec_j.add("md_entry_id",        soem->md_entry_id);
        order_exec_j.add("md_entry_px",        get_decimal_val(soem->md_entry_px));
        order_exec_j.add("md_entry_size",      soem->md_entry_size);
        order_exec_j.add_double("md_entry_px", get_decimal_val(soem->last_px));
        order_exec_j.add("last_qty",           soem->last_qty);
        order_exec_j.add("trade_id",           soem->trade_id);
        order_exec_j.add("md_flags",           soem->md_flags);
        order_exec_j.add("md_flags2",          soem->md_flags2);
        order_exec_j.add("sec_id",             soem->sec_id);
        order_exec_j.add("rpt_seq",            soem->rpt_seq);

        if (soem->md_entry_type == '0')
            order_exec_j.add("md_entry_type", "Bid");
        else
            order_exec_j.add("md_entry_type", "Ask");
        order_exec_j.add("msg", "OrderExecution");
        return order_exec_j.to_string();
    }

    template<>
    std::string dump_sbe_msg<smb_order_booksnp_msg_t>(void *data)
    {
        smb_order_booksnp_msg_t *sobm = reinterpret_cast<smb_order_booksnp_msg_t*>(data);
        json_t book_snp_j;

        json_t book_snp_nj;
        book_snp_nj.add("sec_id",                  sobm->sec_id);
        book_snp_nj.add("last_msg_seq_nproc",      sobm->last_msg_seq_nproc);
        book_snp_nj.add("rpt_seq",                 sobm->rpt_seq);
        book_snp_nj.add("xchg_trade_session_id",   sobm->xchg_trade_session_id);

        json_t gr_size_j;
        gr_size_j.add("block_len",    sobm->num_md_entries.block_len);
        gr_size_j.add("num_in_group", sobm->num_md_entries.num_in_group);

        book_snp_nj.add("num_md_entries",         gr_size_j);
        book_snp_nj.add("md_entry_id",            sobm->md_entry_id);
        book_snp_nj.add("trans_time",             get_string_ns(sobm->trans_time));
        book_snp_nj.add_double("md_entry_px",     get_decimal_val(sobm->md_entry_px));
        book_snp_nj.add("md_entry_size",          sobm->md_entry_size);
        book_snp_nj.add("trade_id",               sobm->trade_id);

        json_t md_flags_j;
        md_flags_j.add("Day",              sobm->md_flags & 0x1);
        md_flags_j.add("OTC order",        sobm->md_flags & 0x04);
        md_flags_j.add("Negotiated order", sobm->md_flags & 0x4000000);

        book_snp_nj.add("md_flags",  md_flags_j);
        book_snp_nj.add("md_flags2", sobm->md_flags2);

        if (sobm->md_entry_type == '0')
            book_snp_nj.add("md_entry_type", "Bid");
        else if(sobm->md_entry_type == '1')
            book_snp_nj.add("md_entry_type", "Ask");
        else
            book_snp_nj.add("md_entry_type", "Empty Book");

        book_snp_j.add("data", book_snp_nj);
        book_snp_j.add("msg", "OrderBookSnapshot");

        /* iterate all over the entries */
        return book_snp_j.to_string();
    }

    template<typename T>
    void process_sbe_msg(void *data, size_t len)
    {
        T *msg = reinterpret_cast<T*>(data);
        processed = dump_sbe_msg<T>(msg);
    }
private:
    std::string processed;
};

