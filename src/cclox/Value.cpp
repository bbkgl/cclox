#include <cstdio>
#include "Value.h"
#include "ObjString.h"

namespace cclox{
    void PrintValue(std::string_view global) {
        printf("\"%s\"", global.data());
    }

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
            case VAL_OBJ: {
                Object* obj = AS_OBJ(value);
                printf("\"%s\"", obj->PrintObj().c_str());
            }
        }
    }
}
