#include <cstdio>
#include "Value.h"

namespace cclox{
    void PrintValue(Value value) {
        switch (value.type)
        {
            case VAL_NUMBER:
                printf("%g", AS_NUMBER(value));
                break;
            case VAL_NIL:
                printf("nil");
                break;
            case VAL_BOOL:
                printf("%s", AS_BOOL(value) ? "true" : "false");
                break;
        }
    }
}
