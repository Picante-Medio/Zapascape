#pragma once
#include "HUGO.h"

struct TimeComponent;
HSYSTEM(HUDSystem)
private:
    EntityID gameTimer;
    EntityID gameTimerText;
public:
    void init() override;
    void onLoad() override;
    void fixedUpdate(uint32_t) override;
    void update(uint32_t dt) override;
END_HSYSTEM()