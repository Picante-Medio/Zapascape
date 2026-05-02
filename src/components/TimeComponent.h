#pragma once
#include "HUGO.h"

HCOMPONENT(TimeComponent)
public:
    uint32_t remainingGameTime = 0;
    bool playerStoppedLongTime = false;
END_HCOMPONENT()