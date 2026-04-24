#pragma once
#include "HugoComponentMacros.h"

HCOMPONENT(MapComponent)
private:
    DEFAULT_SERIALIZED_MEMBER(HUGO::Float, gridSize, 1.0f);
public:
    std::vector<std::vector<int>> mapData;
    SERIALIZED_MEMBER(HUGO::String, wallPrefab);

    SERIALIZED_MEMBER(HUGO::Int, mapWidth);
    SERIALIZED_MEMBER(HUGO::Int, mapHeight);
END_HCOMPONENT()