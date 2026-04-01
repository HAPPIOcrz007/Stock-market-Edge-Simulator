// core_structure.h
#ifndef CORE_STRUCTURE_H
#define CORE_STRUCTURE_H
/**
 * [Command.txt] ⟶ [batch_order_generator.cpp] ⟶ [order_maker.cpp]
 * ↑   ────────────────────────────────────────────────────────↵
 * ↑   ↳⟶ [orders0<stock_id>.bin] or [orders1<stock_id>.bin]
 * ↑   ↳⟶ [matchine_engine.cpp] ⟶ price_change
 * ↑                                       ↑
 * └──────── Human Readable ───────────────┘
*/

/**
 * [Command.txt] ⟹ we enter texted orders(human understandable format)
 *  ⬨ format
 *  {stock_id} {orders count} {gradient}
 *  ⬨
 *  {stock_id} --> to be moved
 *  {orders count} --> sum of bids/asks total
 *  {gradient} --> expected slove of price change +/- must be mentioned
*/

/**
 * [batch_order_generator.cpp] ⟹ based on the [Command.txt] makes orders
 *                                  and passes to [order_maker.cpp]
 * 
 * [order_maker.cpp] ⟹ makes the orders to put into [orders0<stock_id>.bin]
 *                          or [orders1<stock_id>.bin] based on order
 * 
 * [orders0<stock_id>.bin] ⟹ the Sell order book based on <stock_id>
 * [orders1<stock_id>.bin] ⟹ the Buy order book based on <stock_id>
 * 
 * [matchine_engine.cpp] ⟹ does matchings and manages the orderbooks
 *                              and send changes to <price_change>
 * 
 * price_change ⟹ the final output and feed of this whole system
 */
#include <cstdint>
#include <cstddef>

#pragma pack(push,1)
struct alignas(8) Order{
    uint32_t price;     // 4 bytes = 0 to 4 294 967 295 raw units ⟹ $ 42 . 9 M
    /**
     * accuracy is price / 1000
     * max price = 4 294 967 . 295
     */
    uint32_t volume;    // 4 bytes = 0 to 4,294,967,295 raw units ⟹ 4 . 29 M units
    /**
     * accuracy is volume / 10 , 000
     * max price = 429 496 . 7295
     */

    uint32_t trader_id; // 4 bytes , max 4.29 B traders

    uint16_t stock_id;  // 2 bytes 65 . 5 k stocks
    uint8_t  flags;
    /**
     * flags ⟹ 0 ⟹ SELL + GTC (default)
     * flags ⟹ 1 ⟹ BUY  + GTC (default)
     * flags ⟹ 2 ⟹ SELL + FOK
     * flags ⟹ 3 ⟹ BUY  + FOK
     * flags ⟹ 4 ⟹ SELL + IOC
     * flags ⟹ 5 ⟹ BUY  + IOC
     * flags ⟹ 6 ⟹ XXXXXXXXXX
     * flags ⟹ 7 ⟹ XXXXXXXXXX
     */
    uint8_t  reserved;  // to match 16 bytes, for future markings
    // total 16 bytes / order
    
    // --- --- --- --- --- --- --- --- --- ---
    // --- --- --- --- --- --- --- --- --- ---
    inline bool is_sellGTC() { return flags == 0x00; }
    inline bool is_buyGTC()  { return flags == 0x01; }
    inline bool is_sellFOK() { return flags == 0x02; }
    inline bool is_buyFOK()  { return flags == 0x03; }
    inline bool is_sellIOC() { return flags == 0x04; }
    inline bool is_buyIOC()  { return flags == 0x05; }

    inline void set_sellGTC() { flags = 0x00; }
    inline void set_buyGTC()  { flags = 0x01; }
    inline void set_sellFOK() { flags = 0x02; }
    inline void set_buyFOK()  { flags = 0x03; }
    inline void set_sellIOC() { flags = 0x04; }
    inline void set_buyIOC()  { flags = 0x05; }
    // --- --- --- --- --- --- --- --- --- ---
    // --- --- --- --- --- --- --- --- --- ---
    inline double get_price()       const { return price/1000.0;   }
    inline double get_volume()      const { return volume/10000.0; }
    inline uint32_t get_trader_id() const { return trader_id;     }
    inline uint16_t get_stock_id()  const { return stock_id;      }
    // --- --- --- --- --- --- --- --- --- ---
    // --- --- --- --- --- --- --- --- --- ---
    inline uint32_t get_trader_id(){
        return trader_id;
    }
    inline uint16_t get_stock_id(){
        return stock_id;
    }

    inline void set_trader_id(uint32_t t){
        trader_id = static_cast<uint32_t>(t);
    }
    inline void set_stock_id(uint16_t s){
        stock_id = static_cast<uint16_t>(s);
    }
};
#pragma pack(pop)

struct BatchCommand {
    uint16_t stock_id;      // 2 bytes - max 65 535
    uint16_t order_count;   // 2 bytes - max 65 535
    
    int16_t gradient_x100;  // 2 bytes - Gradient ×100 - 327 . 68 to + 327 . 67)
    uint16_t noise_x100;    // 2 bytes - Noise factor ×100 (0 to 655 . 35)
        
    // Total: 2+2+2+2 = 8 bytes
    
    inline double get_gradient() const { return gradient_x100 / 100.0; }
    inline void set_gradient(double g) { 
        gradient_x100 = static_cast<int16_t>(g * 100); 
    }
    
    inline double get_noise() const { return noise_x100 / 100.0; }
    inline void set_noise(double n) { 
        noise_x100 = static_cast<uint16_t>(n * 100); 
    }
};

#pragma pack(push, 1)
struct StockState {
    uint32_t last_price;        // 4
    uint32_t last_volume;       // 4
    uint32_t bid_price;         // 4
    uint32_t ask_price;         // 4
    uint32_t bid_volume;        // 4
    uint32_t ask_volume;        // 4
    uint32_t day_high;          // 4
    uint32_t day_low;           // 4
    // 32 bytes
};
#pragma pack(pop)

#pragma pack(push,1)
struct Match {
    uint64_t buyer_ref;     // 32b trader_id + 32b order_seq
    uint64_t seller_ref;    // 32b trader_id + 32b order_seq
    // 16 bytes total
    
    // Getters
    inline uint32_t buyer_id() const { return buyer_ref >> 32; }
    inline uint32_t buyer_seq() const { return buyer_ref & 0xFFFFFFFF; }
    
    inline uint32_t seller_id() const { return seller_ref >> 32; }
    inline uint32_t seller_seq() const { return seller_ref & 0xFFFFFFFF; }
    
    // Setters
    inline void set_buyer(uint32_t id, uint32_t seq) {
        buyer_ref = (static_cast<uint64_t>(id) << 32) | seq;
    }
    
    inline void set_seller(uint32_t id, uint32_t seq) {
        seller_ref = (static_cast<uint64_t>(id) << 32) | seq;
    }
};
#pragma pack(pop)

namespace OrderConstants {
    // Fixed-point multipliers
    constexpr uint32_t PRICE_MULT  = 100;
    constexpr uint32_t VOLUME_MULT = 1000;

    constexpr const char* COMMANDS_DIR        = "data_holders/commands/";
    constexpr const char* ORDERS_TEXT_DIR     = "data_holders/orders/text/";
    constexpr const char* ORDERS_BIN_DIR      = "data_holders/orders/bin/";
    constexpr const char* STATES_CURRENT_DIR  = "data_holders/states/currents/";
    constexpr const char* STATES_SNAPSHOT_DIR = "data_holders/states/snapshots/";
    constexpr const char* MATCHING_DIR        = "data_holders/matching/";
    
    /**
     * flags ⟹ 0 ⟹ SELL + GTC (default)
     * flags ⟹ 1 ⟹ BUY  + GTC (default)
     * flags ⟹ 2 ⟹ SELL + FOK
     * flags ⟹ 3 ⟹ BUY  + FOK
     * flags ⟹ 4 ⟹ SELL + IOC
     * flags ⟹ 5 ⟹ BUY  + IOC
     */
    constexpr uint8_t FLAG_SELLGTC = 0x00;
    constexpr uint8_t FLAG_BUYGTC  = 0x01;
    constexpr uint8_t FLAG_SELLFOK = 0x02;
    constexpr uint8_t FLAG_BUYFOK  = 0x03;
    constexpr uint8_t FLAG_SELLIOC = 0x04;
    constexpr uint8_t FLAG_BUYIOC  = 0x05; 
}

#define STATIC_ASSERT(cnd, msg) static_assert(cnd, msg)
STATIC_ASSERT(sizeof(Order) == 16, "Order must be 16 bytes");
STATIC_ASSERT(sizeof(BatchCommand) == 8, "BatchCommand must be 8 bytes");
STATIC_ASSERT(sizeof(StockState) == 32, "StockState must be 32 bytes");
STATIC_ASSERT(sizeof(Match) == 16, "Match must be 16 bytes");

STATIC_ASSERT(alignof(Order) <= 8, "Order alignment too large");
STATIC_ASSERT(alignof(StockState) <= 8, "StockState alignment too large");

#endif