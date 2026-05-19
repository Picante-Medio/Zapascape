#pragma once
#include "HUGO.h"

HCOMPONENT(TimeComponent)
public:
    uint32_t remainingGameTime = 0;
    DEFAULT_SERIALIZED_MEMBER(HUGO::Int, maxGameTime, 120);
    DEFAULT_SERIALIZED_MEMBER(HUGO::Int, maxStopTime, 10);
    bool playerStoppedLongTime = false;
    bool alert = false;
END_HCOMPONENT()
