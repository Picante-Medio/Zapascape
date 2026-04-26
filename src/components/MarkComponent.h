#pragma once
#include "HUGO.h"

HCOMPONENT(MarkComponent)
    DEFAULT_SERIALIZED_MEMBER(HUGO::Int, maxNum, 10);
    HUGO::Int currentNum = 0;
END_HCOMPONENT()

