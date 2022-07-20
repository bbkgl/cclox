
#include "ObjString.h"

namespace cclox {
    ObjString::ObjString(std::string_view strView)
    : _data(strView)
    {
    }
    ObjType ObjString::GetType() const {
        return OBJ_STRING;
    }

    inline const char* ObjString::GetAnsiData() const {
        return _data.c_str();
    }

    const std::string& ObjString::GetData() const {
        return _data;
    }

    bool ObjString::Equal(const Object *const other) const {
        if (other->GetType() == OBJ_STRING)
            return _data == static_cast<const ObjString*>(other)->_data;
        return false;
    }

    std::string ObjString::PrintObj() const {
        return _data;
    }

    ObjString* Concatenate(ObjString* left, ObjString* right) {
        return new ObjString(left->GetData() + right->GetData());
    }
}
