#pragma once

#include <random>
#include "HUGO.h"

struct TransformComponent;
HSYSTEM(MapSystem)
private:
    const std::vector<HUGO::Vector2> directions = {
        {0, 1}, // Up
        {0, -1},  // Down
        {-1, 0}, // Left
        {1, 0}   // Right
    };
    std::mt19937 mt;
    std::uniform_int_distribution<int> directionsDist;
    bool mapGenerated = false;
    HUGO::Int actX = 0, actY = 0;
    EntityID mapID;
    HUGO::Vector3 beastiePos;
    HUGO::Vector3 playerPos;

    std::vector<HUGO::Vector2> getAdjacentFreePositions(const HUGO::Vector2& pos, const std::vector<std::vector<HUGO::Int>>& mapData);
    
    void createPlayer(const HUGO::Vector3& pos);
    void createBeastie(const HUGO::Vector3& pos);
    void createRitual(const HUGO::Vector3& pos, EntityID ritual);
    void createFloor(const HUGO::Vector3& pos, const HUGO::Vector3 & scale, const std::string& prefab);
    void createRoomLight(const HUGO::Vector3& pos);
    void createDoor(const HUGO::Vector3& pos, const HUGO::Vector2& dir);
    void createNextTile();
    void increasesIt();

public:
    void init() override;
    void onLoad() override;

    void fixedUpdate(uint32_t dt) override;
    void update(uint32_t dt) override;
END_HSYSTEM()
