#include <iostream>
#include <chrono>

uint64_t generate_order_ID(const std::string &ticker);

// ? - why enum class
/* ANS -
    enum - it is better in understanding than
    integers to the reader*/
// ? but ... why enum class
/* ANS -
    enums hold integer value at the back so classing
    reduces that leak into the global scope
    protecting and reducing errors
    classed enums cannot be implicitly converted
    to int
*/
enum class Stage
{
    New,
    Partially_filled,
    Filled,
    Cancelled,
    Replaced
    // ? what do real systems use
    /* ANS -
        The core values are:
        New, Partially Filled, Filled, Cancelled, Replaced,
        Pending Cancel, Pending New, Pending Replace,
        Rejected, Expired, Suspended, Done for Day
    */
    // ? why not using the pending** cases
    /* ANS -
        I looked at FIX OrdStatus,
        took the subset that applies to a synchronous
        single-threaded engine, and left the Pending states
        as a known gap that matters once I go async
    */
    // ? not using stage - replaced here
    /* ANS -
         many systems allow replacing of order specifications like
         price and volume
         so when a change is made in most systems, mainly price change
         the system cancels the current order and gives a new orderId making
         a new order at the timestamp of when the replacement is accepted by the
         system, this is also same in volume increase as its like a boost someone
         would get increaseing the position at the last timestamp
         but if a entity is shrinking its volume we can allow them to keep the
         same Id and the last timestamp as it wont impact others
         (According to process of CME Group)
         changing sides gives different orderID too at new timestamp

         so we keep replaced_orderId tag blank for all orders and if they are
         a continuation of another order we put the last order's id there
    */
};

struct Order
{
};

uint64_t generate_order_ID(){
    uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    
}