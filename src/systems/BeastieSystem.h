#pragma once
#include "HUGO.h"
#include <random>
struct WorldTransformComponent;
struct MovementRBComponent;
struct BeastieComponent;

HSYSTEM(BeastieSystem)
private:
    EntityID player;
    EntityID timer;
    HUGO::Bool rageSoundIncreased = false;
    void updateForce(BeastieComponent& b);
public:
    void init() override;
    void onLoad() override;
    void fixedUpdate(uint32_t) override;
    void update(uint32_t dt) override;
END_HSYSTEM()