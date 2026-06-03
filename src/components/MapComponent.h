#pragma once
#include "HugoComponentMacros.h"

enum class TileType {
    Undefined = -1,
    Empty = 0,
    Wall = 1,
    Start = 2,
    End = 3
};
HCOMPONENT(MapComponent)
private:
    DEFAULT_SERIALIZED_MEMBER(float, gridSize, 1.0f);
    DEFAULT_SERIALIZED_MEMBER(HUGO::Int, numberOfRooms, 1);
public:
    std::vector<std::vector<int>> mapData;
    SERIALIZED_MEMBER(std::string, wallPrefab);

    SERIALIZED_MEMBER(HUGO::Int, mapWidth);
    SERIALIZED_MEMBER(HUGO::Int, mapHeight);

    DEFAULT_SERIALIZED_MEMBER(HUGO::Int, numRituals, 2);

    float getGridSize() const {
        return gridSize;
    }

    HUGO::Int getNumberOfRooms() const {
        return numberOfRooms;
    }
END_HCOMPONENT()