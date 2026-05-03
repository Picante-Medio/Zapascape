#pragma once

#include "HUGO.h"

struct MapComponent;
struct TransformComponent;
HSYSTEM(MapSystem)
private:
    const std::vector<HUGO::Vector2> directions = {
        {0, 1}, // Up
        {0, -1},  // Down
        {-1, 0}, // Left
        {1, 0}   // Right
    };

    std::vector<HUGO::Vector2> getAdjacentFreePositions(HUGO::Vector2 pos, std::vector<std::vector<int>> mapData);
    
    //prueba se quitara cuando funciona los prefabs
    void createPlayer(const HUGO::Vector3& pos);
    void createBeastie(const HUGO::Vector3& pos);
    void createFloor(const HUGO::Vector3& pos, int width, int height);

public:
    void init() override;
    void onLoad() override;

    void fixedUpdate(uint32_t dt) override;
    void update(uint32_t dt) override;
END_HSYSTEM()
