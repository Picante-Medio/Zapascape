#pragma once
#include "HUGO.h"
#include <random>
struct TransformComponent;
HSYSTEM(BeastieSystem)
private:
    enum BeastieState {STOP, PURSUE, SLOW_MOVE};
    BeastieState currentState; 

    EntityID beastie;
    EntityID player;

    bool isSlowMoving;

    bool isPlayerInRange(float dist, const TransformComponent& beastieTr, const TransformComponent& playerTr);
public:
    void init() override;
    void onLoad() override;
    void fixedUpdate(uint32_t) override;
    void update(uint32_t dt) override;
END_HSYSTEM()