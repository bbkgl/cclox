#ifndef CCLOX_OBJSTRING_H
#define CCLOX_OBJSTRING_H

#include <string>
#include "Object.h"

namespace cclox {
#define IS_STRING(value)    (AS_OBJ(value)->GetType() == cclox::OBJ_STRING)

#define AS_STRING(value)    (static_cast<ObjString*>AS_OBJ(value))
#define AS_CSTRING(value)    ((static_cast<ObjString*>AS_OBJ(value))->GetAnsiData())

    class ObjString : public Object {
    public:
        explicit ObjString(std::string_view strView);

        [[nodiscard]] ObjType GetType() const override;

        bool Equal(const Object *const other) const override;
        [[nodiscard]] const char* GetAnsiData() const;
        const std::string& GetData() const;
        [[nodiscard]] std::string PrintObj() const override;
    protected:
        std::string _data;
    };

    ObjString* Concatenate(ObjString* left, ObjString* right);
}


#endif //CCLOX_OBJSTRING_H
