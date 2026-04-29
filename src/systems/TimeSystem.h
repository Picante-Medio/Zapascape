#pragma once
#include "HUGO.h"

HSYSTEM(TimeSystem)
private:
    uint32_t playerStoppedTime;
    uint32_t gameTime;
    uint32_t accumulatedMS;

    bool menssageSent;
    
    EntityID timerEntity;

    void resetTimes();
public:
    void init() override;
    void onLoad() override;
    void fixedUpdate(uint32_t) override;
    void update(uint32_t dt) override;
END_HSYSTEM()