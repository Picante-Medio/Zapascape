#pragma once
#include "HugoComponentMacros.h"

HCOMPONENT(InstantiateComponent)
private:
    SERIALIZED_MEMBER(HUGO::String, prefabName);
public:
    HUGO::String getPrefabName() const {
        return prefabName;
    }
END_HCOMPONENT()