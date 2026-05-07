#pragma once
#include "HUGO.h"

enum BeastieState {STOP, PURSUE, WANDER};
HCOMPONENT(BeastieComponent)
    BeastieState currentState = BeastieState::STOP;
    DEFAULT_SERIALIZED_MEMBER(HUGO::Float, normalForce, 20.0f);
    DEFAULT_SERIALIZED_MEMBER(HUGO::Float, fastForce, 30.0f);
    DEFAULT_SERIALIZED_MEMBER(HUGO::Float, slowForce, 2.0f);

    DEFAULT_SERIALIZED_MEMBER(HUGO::Float, minRange, 6.0f);
    DEFAULT_SERIALIZED_MEMBER(HUGO::Float, maxRange, 10.0f);

    HUGO::Bool isFastMove = false;
    HUGO::Bool isSlowMove = false;

    HUGO::Float changeDirTime = 4.0f; // seconds

    HUGO::Float currentSpawnDelay = 6.0f; // seconds

    HUGO::Vector3 desiredDirection = HUGO::Vector3(0.0f);
    HUGO::Vector3 wanderDirection = HUGO::Vector3(0.0f);
    HUGO::Float wanderTime = 0.0f;
    DEFAULT_SERIALIZED_MEMBER(HUGO::Float, wanderInterval, 2.0f); // seconds

    HUGO::Bool aiEnabled = false;
END_HCOMPONENT()

