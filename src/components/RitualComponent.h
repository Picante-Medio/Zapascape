#pragma once
#include "HugoComponentMacros.h"

HCOMPONENT(RitualComponent)
private:
    bool isActive;
public:
    EntityID associatedCubo;
    void setActive(bool active){
        isActive = active;
    }
    bool getActive() const {
        return isActive;
    }
END_HCOMPONENT()