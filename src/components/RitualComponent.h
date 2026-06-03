#pragma once
#include "HUGO.h"

HCOMPONENT(RitualComponent)
public:
    EntityID associatedCubo = -1;
    DEFAULT_SERIALIZED_MEMBER(std::string, ritualActivateSound, "");
    bool isActive = true;
    DEFAULT_SERIALIZED_MEMBER(float, volume3D, 0.7f);
    DEFAULT_SERIALIZED_MEMBER(float, volumeDeactivate, 0.8f);
END_HCOMPONENT()