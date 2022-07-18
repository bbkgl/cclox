#ifndef CCLOX_VALUE_H
#define CCLOX_VALUE_H

#include <vector>
#include <cstring>
#include "Common.h"

namespace cclox {
    // constant define
    constexpr uint32 STACK_MAX = 256;

#define VALUE_T_TABLE() \
    DEF_VALUE_T(VAL_BOOL) \
    DEF_VALUE_T(VAL_NIL) \
    DEF_VALUE_T(VAL_NUMBER) \
    DEF_VALUE_T(VAL_OBJ) \

    typedef enum : uint8 {
#define DEF_VALUE_T(VALUE_TYPE) VALUE_TYPE,
        VALUE_T_TABLE()
#undef DEF_VALUE_T
    } ValueType;

    inline const char* GetValueTypeStr(ValueType type) {
        switch (type)
        {
#define DEF_VALUE_T(VALUE_TYPE) case ValueType::VALUE_TYPE: \
                                  return #VALUE_TYPE;
            VALUE_T_TABLE()
#undef DEF_VALUE_T
        }
        return nullptr;
    }

    typedef double Number;
    class Object;

    typedef struct Value {
        ValueType type;

        union {
            bool boolean;
            Number number;
            Object* obj;
        } as;
    } Value;

#define IS_BOOL(value)        ((value).type == cclox::VAL_BOOL)
#define IS_NIL(value)         ((value).type == cclox::VAL_NIL)
#define IS_NUMBER(value)      ((value).type == cclox::VAL_NUMBER)
#define IS_OBJ(value)         ((value).type == cclox::VAL_OBJ)

#define AS_BOOL(value)        ((value).as.boolean)
#define AS_NUMBER(value)      ((value).as.number)
#define AS_OBJ(value)         ((value).as.obj)

#define BOOL_VAL(value)       ((cclox::Value){cclox::VAL_BOOL,     {.boolean = (value)}})
#define NIL_VAL(value)        ((cclox::Value){cclox::VAL_NIL,      {.number = 0}})
#define NUMBER_VAL(value)     ((cclox::Value){cclox::VAL_NUMBER,   {.number = (value)}})
#define OBJ_VAL(value)        ((cclox::Value){cclox::VAL_OBJ,      {.obj = (value)}})

    inline Value VAL_VAL(Number number) {
        return NUMBER_VAL(number);
    }

    inline Value VAL_VAL(bool boolean) {
        return BOOL_VAL(boolean);
    }

    inline Value VAL_VAL(std::nullptr_t nil) {
        return NIL_VAL(nil);
    }

    inline Value VAL_VAL(Object* obj) {
        return OBJ_VAL(obj);
    }

    inline Value& VAL_VAL(Value& value) {
        return value;
    }

    inline Value operator+(Value& a, Value& b) {
        return VAL_VAL(AS_NUMBER(a) + AS_NUMBER(b));
    }

    inline Value operator-(Value& a, Value& b) {
        return VAL_VAL(AS_NUMBER(a) - AS_NUMBER(b));
    }

    inline Value operator*(Value& a, Value& b) {
        return VAL_VAL(AS_NUMBER(a) * AS_NUMBER(b));
    }

    inline Value operator/(Value& a, Value& b) {
        return VAL_VAL(AS_NUMBER(a) / AS_NUMBER(b));
    }

    inline bool operator==(Value& a, Value& b) {
        if (a.type != b.type)
            return false;
        switch (a.type) {
            case VAL_NUMBER:
                return AS_NUMBER(a) == AS_NUMBER(b);
            case VAL_BOOL:
                return AS_BOOL(a) == AS_BOOL(b);
            case VAL_NIL:
                return true;
        }
        return false;
    }

    inline bool operator<(Value& a, Value& b) {
        return AS_NUMBER(a) < AS_NUMBER(b);
    }

    inline bool operator>(Value& a, Value& b) {
        return AS_NUMBER(a) > AS_NUMBER(b);
    }

    typedef std::vector<Value> ValueArray;

    void PrintValue(Value value);
}

#endif //CCLOX_VALUE_H
