#pragma once

#include "HUGO.h"

struct MapComponent;
HSYSTEM(MapSystem)
private:
    const std::vector<HUGO::Vector2> directions = {
        {0, 1}, // Up
        {0, -1},  // Down
        {-1, 0}, // Left
        {1, 0}   // Right
    };

    std::vector<HUGO::Vector2> getAdjacentFreePositions(HUGO::Vector2 pos, std::vector<std::vector<int>> mapData);
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
