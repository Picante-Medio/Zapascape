#pragma once
#include "HUGO.h"

HSYSTEM(GameToOtherSystem)
public:
    void init() override;
    void onLoad() override;
    void fixedUpdate(uint32_t) override;
    void update(uint32_t) override;
END_HSYSTEM()
