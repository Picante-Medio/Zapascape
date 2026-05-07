#pragma once
#include "HUGO.h"
#include <random>
struct TransformComponent;
struct MovementRBComponent;
struct BeastieComponent;
HSYSTEM(BeastieSystem)
private:
    EntityID beastie;
    EntityID player;
    uint32_t cont =0;
    HUGO::Bool isPlayerInRange(HUGO::Float dist, const TransformComponent& beastieTr, const TransformComponent& playerTr);
    void updateForce(BeastieComponent& b);
public:
    void init() override;
    void onLoad() override;
    void fixedUpdate(uint32_t) override;
    void update(uint32_t dt) override;
END_HSYSTEM()