#pragma once
#include "HUGO.h"

enum BeastieState {STOP, PURSUE, WANDER};
HCOMPONENT(BeastieComponent)
    BeastieState currentState = BeastieState::STOP;
    DEFAULT_SERIALIZED_MEMBER(float, normalForce, 20.0f);
    DEFAULT_SERIALIZED_MEMBER(float, fastForce, 30.0f);
    DEFAULT_SERIALIZED_MEMBER(float, slowForce, 2.0f);

    DEFAULT_SERIALIZED_MEMBER(float, minRange, 6.0f);
    DEFAULT_SERIALIZED_MEMBER(float, maxRange, 10.0f);

    bool isFastMove = false;
    bool isSlowMove = false;

    float changeDirTime = 4.0f; // seconds

    float currentSpawnDelay = 6.0f; // seconds

    HUGO::Vector3 desiredDirection = HUGO::Vector3(0.0f);
    HUGO::Vector3 wanderDirection = HUGO::Vector3(0.0f);
    float wanderTime = 0.0f;
    DEFAULT_SERIALIZED_MEMBER(float, wanderInterval, 2.0f); // seconds

    bool aiEnabled = false;
END_HCOMPONENT()

