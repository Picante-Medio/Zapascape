#pragma once
#include "HUGO.h"

HCOMPONENT(MovementRBComponent)
    HUGO::Vector3 direction = HUGO::Vector3(0.0f);
    DEFAULT_SERIALIZED_MEMBER(HUGO::Float, speed, 5.0f);
END_HCOMPONENT()

