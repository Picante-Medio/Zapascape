#pragma once
#include "HugoComponentMacros.h"

HCOMPONENT(RitualComponent)
private:
    bool isActive;
public:
    void setActive(bool active){
        isActive = active;
    }
END_HCOMPONENT()