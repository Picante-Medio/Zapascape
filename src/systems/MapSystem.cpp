#include "MapSystem.h"
#include "MapComponent.h"
#include "TransformComponent.h"

void MapSystem::init() {}

void 
MapSystem::onLoad() {
    const std::vector<HUGO::Vector2> directions = {
        {0, 1}, // Up
        {0, -1},  // Down
        {-1, 0}, // Left
        {1, 0}   // Right
    };

    for (auto& pack: scene.query<MapComponent>()) {
        MapComponent map = pack.getConst<MapComponent>();
        map.mapData = std::vector<std::vector<int>>(map.mapHeight, std::vector<int>(map.mapWidth, static_cast<int>(TileType::Undefined)));

        HUGO::Vector2 startPos(map.mapWidth / 2.0f, map.mapHeight / 2.0f);
        map.mapData[static_cast<int>(startPos.y)][static_cast<int>(startPos.x)] = static_cast<int>(TileType::Start);

        HUGO::Vector2 currentPos = startPos;
        ErrorOrType<EntityID> wall = scene.instantiatePrefab("wall");

        if(!wall.isError()) {
            TransformComponent wallTransform = scene.getComponent<TransformComponent>(wall.getValue());
            wallTransform.setPosition(HUGO::Vector3(currentPos.x * map.getGridSize(), 0, currentPos.y * map.getGridSize()));
        }
    }
}

void
MapSystem::update(uint32_t dt) 
{
}

void
MapSystem::fixedUpdate(uint32_t) {}
