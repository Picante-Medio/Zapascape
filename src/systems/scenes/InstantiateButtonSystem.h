#pragma once

#include "HUGO.h"
HSYSTEM(InstantiateButtonSystem)
private:
    EntityID click;
    EntityID loading;
    HUGO::Bool countdownStart = false;
    uint32_t cont = 0;
    uint32_t contMax = 1000;
public:
    void init() override;
    void onLoad() override;
    void fixedUpdate(uint32_t) override;
    void update(uint32_t dt) override;
END_HSYSTEM()