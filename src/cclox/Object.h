#ifndef CCLOX_OBJECT_H
#define CCLOX_OBJECT_H

#include <string>
#include "Common.h"

namespace cclox {
#define OBJ_T_TABLE() \
    DEF_OBJ_T(OBJ_RAW) \
    DEF_OBJ_T(OBJ_STRING) \

    typedef enum : uint32 {
#define DEF_OBJ_T(OBJ_TYPE) OBJ_TYPE,
        OBJ_T_TABLE()
#undef DEF_OBJ_T
    } ObjType;

    inline const char* GetObjTypeStr(ObjType type) {
        switch (type)
        {
#define DEF_OBJ_T(OBJ_TYPE) case ObjType::OBJ_TYPE: \
                                  return #OBJ_TYPE;
            OBJ_T_TABLE()
#undef DEF_OBJ_T
        }
        return nullptr;
    }

#define OBJ_TYPE(value) AS_OBJ(value)->GetType()

    class Object {
    public:
        explicit Object();
        virtual ~Object();

        [[nodiscard]] virtual ObjType GetType() const;

        virtual bool Equal(const Object* other) const;

        [[nodiscard]] virtual std::string PrintObj() const;
    private:
    };
}

#endif //CCLOX_OBJECT_H
