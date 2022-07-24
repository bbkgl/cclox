
#include "ObjString.h"

namespace cclox {
    ObjString::ObjString(const std::string& fromStr)
    : Object()
    , _view(fromStr)
    {
    }

    ObjType ObjString::GetType() const {
        return OBJ_STRING;
    }

    inline const char* ObjString::GetAnsiData() const {
        return _view.data();
    }

    const std::string& ObjString::GetData() const {
        auto&& kv = _sObjStringTable.find(_view.data());
        if (kv != _sObjStringTable.end())
            return kv->first;
        return _sEmpty;
    }

    bool ObjString::Equal(const Object *const other) const {
        if (other->GetType() == OBJ_STRING)
            return _view == static_cast<const ObjString*>(other)->_view;
        return false;
    }

    std::string ObjString::PrintObj() const {
        return _view.data();
    }

    ObjString* ObjString::Concatenate(ObjString* left, ObjString* right) {
        return CreateString(left->GetData() + right->GetData());
    }

    ObjString* ObjString::CreateString(std::string_view strView) {
        auto&& iter = _sObjStringTable.find(strView.data());
        if (iter == _sObjStringTable.end()) {
            auto&& pair = _sObjStringTable.emplace(strView, nullptr);
            pair.first->second = new ObjString(pair.first->first);
            return pair.first->second;
        }
        return iter->second;
    }

    ObjString::~ObjString() {

    }

    std::unordered_map<std::string, ObjString*> ObjString::_sObjStringTable;
    std::string ObjString::_sEmpty;
}
