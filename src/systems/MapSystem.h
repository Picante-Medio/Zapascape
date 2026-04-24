#pragma once

#include "HUGO.h"

struct MapComponent;
HSYSTEM(MapSystem)
public:
    void init() override;
    void onLoad() override;

    void fixedUpdate(uint32_t dt) override;
    void update(uint32_t dt) override;
END_HSYSTEM()

enum class TileType {
    Undefined = -1,
    Empty = 0,
    Wall = 1,
    Start = 2,
    End = 3
};
