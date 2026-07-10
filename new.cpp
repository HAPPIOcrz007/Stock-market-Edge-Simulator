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
enum class Stage{
    Added_in_Book,
    Matching,
    Filled,
    Recorded,

    Modified,
    Re_Added_in_Book,

    Cancelled,
    Flagged
};

