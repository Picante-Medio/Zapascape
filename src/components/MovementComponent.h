#pragma once
#include "HUGO.h"

HCOMPONENT(MovementComponent)
    HUGO::Vector3 direction = HUGO::Vector3(0.0f);
    DEFAULT_SERIALIZED_MEMBER(float, speed, 1.0f);
END_HCOMPONENT()

