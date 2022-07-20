
#include "Object.h"

namespace cclox {
    ObjType cclox::Object::GetType() const {
        return OBJ_RAW;
    }

    bool Object::Equal(const Object* const other) const {
        return other == this;
    }

    std::string Object::PrintObj() const {
        return "[Object]";
    }
}
