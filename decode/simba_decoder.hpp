#include <cstdint>
#include <string>

#include "simba_msg.hpp"
#include "../util/utils.hpp"
#include "../util/json.hpp"

static std::string get_state_string(uint64_t md_field, uint64_t md_field2)
{
    std::string state_string = "[\"";
    /* Function to add state to the string if the corresponding bit is set */
    auto addState = [&](uint64_t mask, const std::string& state_name) {
        if ((md_field & mask) || (md_field2 & mask)) {
            if (!state_string.empty()) {
                state_string += ", ";
            }
            state_string += state_name;
        }
    };

    addState(0x1, "Day");
    addState(0x2, "IOC");
    addState(0x4, "OTC");
    addState(0x1000, "End of transaction bit");
    addState(0x2000, "Sign of canceling a passive cross-order");
    addState(0x80000, "Fill-or-Kill");
    addState(0x100000, "The entry is the result of the order move");
    addState(0x200000, "The entry is the result of the order cancel");
    addState(0x400000, "The entry is the result of the orders mass cancel");
    addState(0x4000000, "Negotiated order");
    addState(0x8000000, "Multi-leg order");
    addState(0x20000000, "Sign of order deletion due to a cross-trade");
    addState(0x80000000, "Negotiated order matched by reference");
    addState(0x100000000, "The entry is the result of the orders cancel by 'Cancel on Disconnect' service");
    addState(0x200000000000, "Synthetic order");
    addState(0x400000000000, "RFS order");
    addState(0x200000000000000, "Passive synthetic order");
    addState(0x1000000000000000, "Book-or-cancel order (Passive only)");
    addState(0x4000000000000000, "Sign of an order/trade during the opening auction");

    return state_string + "\"]";
}

class smb_decoder_t
{
public:
    smb_decoder_t()
        {}

    void process(void *data, size_t len) noexcept;
    void process_inc(void *data, size_t len) noexcept;
    uint16_t process_sbe(void *data_ptr, size_t len) noexcept;
    std::string dump();

    template<typename T>
    std::string dump_sbe_msg(void *data);

    template<>
    std::string dump_sbe_msg<smb_order_update_msg_t>(void *data)
    {
        smb_order_update_msg_t *smou = reinterpret_cast<smb_order_update_msg_t*>(data);

        json_t order_update_j;
        json_t order_update_nj;

        order_update_nj.add("md_entry_id",        smou->md_entry_id);
        order_update_nj.add("md_entry_px",        get_decimal_val(smou->md_entry_px));
        order_update_nj.add("md_entry_size",      smou->md_entry_size);
        order_update_nj.add("md_flags",           get_state_string(smou->md_flags, smou->md_flags2));
        order_update_nj.add("sec_id",             smou->sec_id);
        order_update_nj.add("rpt_seq",            smou->rpt_seq);

        if (smou->md_update_action == 0)
            order_update_nj.add("md_update_action", "New");
        else if (smou->md_update_action == 2)
            order_update_nj.add("md_update_action", "Delete");

        if (smou->md_entry_type == '0')
            order_update_nj.add("md_entry_type", "Bid");
        else
            order_update_nj.add("md_entry_type", "Ask");

        order_update_j.add("data", order_update_nj);
        order_update_j.add("msg", "OrderUpdate");
        return order_update_j.to_string();
    }

    template<>
    std::string dump_sbe_msg<smb_order_exec_msg_t>(void *data)
    {
        smb_order_exec_msg_t *soem = reinterpret_cast<smb_order_exec_msg_t*>(data);

        json_t order_exec_j;
        json_t order_exec_nj;

        order_exec_nj.add("md_entry_id",        soem->md_entry_id);
        order_exec_nj.add("md_entry_px",        get_decimal_val(soem->md_entry_px));
        order_exec_nj.add("md_entry_size",      soem->md_entry_size);
        order_exec_nj.add_double("md_entry_px", get_decimal_val(soem->last_px));
        order_exec_nj.add("last_qty",           soem->last_qty);
        order_exec_nj.add("trade_id",           soem->trade_id);
        order_exec_nj.add("md_flags",           get_state_string(soem->md_flags, soem->md_flags2));
        order_exec_nj.add("sec_id",             soem->sec_id);
        order_exec_nj.add("rpt_seq",            soem->rpt_seq);

        if (soem->md_update_action == 1)
            order_exec_nj.add("md_update_action", "Change");
        else if (soem->md_update_action == 2)
            order_exec_nj.add("md_update_action", "Delete");

        if (soem->md_entry_type == '0')
            order_exec_nj.add("md_entry_type", "Bid");
        else
            order_exec_nj.add("md_entry_type", "Ask");

        order_exec_j.add("data", order_exec_nj);
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

        book_snp_nj.add("num_md_entries", gr_size_j);
        for (int i = 0; i < sobm->num_md_entries.num_in_group; i++)
        {
            json_t md_entry_j;
            md_entry_j.add("md_entry_id",            sobm->md_entry.md_entry_id);
            md_entry_j.add("trans_time",             get_string_ns(sobm->md_entry.trans_time));
            md_entry_j.add_double("md_entry_px",     get_decimal_val(sobm->md_entry.md_entry_px));
            md_entry_j.add("md_entry_size",          sobm->md_entry.md_entry_size);
            md_entry_j.add("trade_id",               sobm->md_entry.trade_id);
            md_entry_j.add("md_flags",               get_state_string(sobm->md_entry.md_flags,
                                                                      sobm->md_entry.md_flags2));
            if (sobm->md_entry.md_entry_type == '0')
                md_entry_j.add("md_entry_type", "Bid");
            else if (sobm->md_entry.md_entry_type == '1')
                md_entry_j.add("md_entry_type", "Ask");
            else if (sobm->md_entry.md_entry_type == 'J')
                md_entry_j.add("md_entry_type", "Empty Book");
            book_snp_j.append("md_entry", md_entry_j);
        }

        book_snp_j.add("data", book_snp_nj);
        book_snp_j.add("msg", "OrderBookSnapshot");

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
