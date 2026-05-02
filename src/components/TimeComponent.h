#pragma once
#include "HUGO.h"

HCOMPONENT(TimeComponent)
public:
    DEFAULT_SERIALIZED_MEMBER(HUGO::Int, remainingGameTime, 0);
END_HCOMPONENT()

