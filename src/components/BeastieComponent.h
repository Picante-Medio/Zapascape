#pragma once
#include "HUGO.h"

enum BeastieState {STOP, PURSUE};
HCOMPONENT(BeastieComponent)
    BeastieState currentState = BeastieState::STOP;
    DEFAULT_SERIALIZED_MEMBER(HUGO::Float, normalForce, 20.0f);
    DEFAULT_SERIALIZED_MEMBER(HUGO::Float, fastForce, 30.0f);
    DEFAULT_SERIALIZED_MEMBER(HUGO::Float, slowForce, 2.0f);

    DEFAULT_SERIALIZED_MEMBER(HUGO::Float, minRange, 6.0f);
    DEFAULT_SERIALIZED_MEMBER(HUGO::Float, maxRange, 10.0f);

    bool isFastMove = false;
    bool isSlowMove = false;
END_HCOMPONENT()

