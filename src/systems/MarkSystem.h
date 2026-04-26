#pragma once

#include "HUGO.h"

#include <vector>
#include <queue>

struct MarkComponent;
HSYSTEM(MarkSystem)
private:
    HUGO::Int maxMarks;
    EntityID player;
    EntityID mark;

    std::deque<EntityID> q;
public:
    void init() override;
    void onLoad() override;

    void fixedUpdate(uint32_t dt) override;
    void update(uint32_t dt) override;
END_HSYSTEM()

