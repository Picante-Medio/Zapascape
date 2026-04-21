#pragma once
#include "HUGO.h"

HCOMPONENT(MovementComponent)
    HUGO::Vector2 direction = HUGO::Vector2(0.0f);
    DEFAULT_SERIALIZED_MEMBER(HUGO::Float, speed, 1.0f);
END_HCOMPONENT()

