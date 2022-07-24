#include "GarbageCollect/GarbageCollector.h"
#include "Object.h"

namespace cclox {
    Object::Object()
    {
        GarbageCollector::Get().AddObject(this);
    }

    ObjType Object::GetType() const {
        return OBJ_RAW;
    }

    bool Object::Equal(const Object* other) const {
        return other == this;
    }

    std::string Object::PrintObj() const {
        return "[Object]";
    }

    Object::~Object() {
    }
}
