#pragma once
#include "HUGO.h"

HCOMPONENT(MovementRBComponent)
    HUGO::Vector3 direction = HUGO::Vector3(0.0f);
    DEFAULT_SERIALIZED_MEMBER(float, force, 25.0f);
END_HCOMPONENT()

