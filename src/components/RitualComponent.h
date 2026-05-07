#pragma once
#include "HUGO.h"

HCOMPONENT(RitualComponent)
public:
    EntityID associatedCubo = -1;
    DEFAULT_SERIALIZED_MEMBER(HUGO::String, ritualActivateSound, "");
    HUGO::Bool isActive = true;
    DEFAULT_SERIALIZED_MEMBER(HUGO::Float, volume3D, 0.7f);
    DEFAULT_SERIALIZED_MEMBER(HUGO::Float, volumeDeactivate, 0.8f);
END_HCOMPONENT()

