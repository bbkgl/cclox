#ifndef CCLOX_VALUE_H
#define CCLOX_VALUE_H

#include <vector>
#include "Common.h"

namespace cclox {
    // constant define
    constexpr uint32 STACK_MAX = 256;

    typedef double Value;

    typedef std::vector<Value> ValueArray;

    void PrintValue(Value value);
}

#endif //CCLOX_VALUE_H
