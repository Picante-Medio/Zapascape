#pragma once

#include "HUGO.h"

struct InstantiateComponent;
HSYSTEM(InstantiateSystem)
public:
    void init() override;
    void onLoad() override;
    void fixedUpdate(uint32_t) override;
    void update(uint32_t dt) override;
END_HSYSTEM()