#pragma once
#include "HUGO.h"

HCOMPONENT(TimeComponent)
public:
    uint32_t remainingGameTime = 0;
    uint32_t remainingStopTime = 0;
    DEFAULT_SERIALIZED_MEMBER(HUGO::Int, maxGameTime, 120);
    DEFAULT_SERIALIZED_MEMBER(HUGO::Int, maxStopTime, 10);
    bool alert = false;
END_HCOMPONENT()
