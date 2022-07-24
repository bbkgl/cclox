#ifndef CCLOX_OBJSTRING_H
#define CCLOX_OBJSTRING_H

#include <string>
#include <unordered_map>
#include "Object.h"

namespace cclox {
#define IS_STRING(value)    (AS_OBJ(value)->GetType() == cclox::OBJ_STRING)

#define AS_STRING(value)    (static_cast<ObjString*>AS_OBJ(value))
#define AS_CSTRING(value)    ((static_cast<ObjString*>AS_OBJ(value))->GetAnsiData())

    class ObjString : public Object {
    private:
        explicit ObjString(const std::string& fromStr);
    public:
        ~ObjString() override;

        [[nodiscard]] ObjType GetType() const override;

        bool Equal(const Object *other) const override;
        [[nodiscard]] const char* GetAnsiData() const;
        [[nodiscard]] const std::string& GetData() const;
        [[nodiscard]] std::string PrintObj() const override;

        // static method
        static ObjString* Concatenate(ObjString* left, ObjString* right);
        static ObjString* CreateString(std::string_view strView);
    protected:
        std::string_view _view;

        // static properties
        static std::unordered_map<std::string, ObjString*> _sObjStringTable;
        static std::string _sEmpty;
    };

}


#endif //CCLOX_OBJSTRING_H
