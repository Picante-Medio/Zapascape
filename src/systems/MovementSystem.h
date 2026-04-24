#pragma once

#include "HUGO.h"

struct MovementComponent;
HSYSTEM(MovementSystem)
public:
    void init() override;
    void onLoad() override;

    void fixedUpdate(uint32_t dt) override;
    void update(uint32_t dt) override;

END_HSYSTEM()

