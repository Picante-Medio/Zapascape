#pragma once
#include "HUGO.h"

HSYSTEM(TimeSystem)
private:
    uint32_t playerStoppedTime;
    uint32_t gameTime;
    uint32_t accumulatedMS;
    uint32_t MAX_GAME_TIME;
    uint32_t MAX_PLAYER_STOP_TIME;
    void resetTimes();
public:
    void init() override;
    void onLoad() override;
    void fixedUpdate(uint32_t) override;
    void update(uint32_t dt) override;
END_HSYSTEM()