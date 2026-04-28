#include "MapSystem.h"
#include "MapComponent.h"
#include "TransformComponent.h"

void MapSystem::init() {}

void 
MapSystem::onLoad() {
    for (auto& pack: scene.query<MapComponent>()) {
        MapComponent map = pack.getConst<MapComponent>();
        map.mapData = std::vector<std::vector<int>>(map.mapHeight, std::vector<int>(map.mapWidth, static_cast<int>(TileType::Undefined)));
        std::vector<std::vector<int>> mapData = map.mapData;

        HUGO::Vector2 startPos(map.mapWidth / 2.0f, map.mapHeight / 2.0f);
        mapData[static_cast<int>(startPos.y)][static_cast<int>(startPos.x)] = static_cast<int>(TileType::Start);

        HUGO::Vector2 currentPos = startPos;
        ErrorOrType<EntityID> wall = scene.instantiatePrefab("wall");
        std::vector<HUGO::Vector2> roomPositions;

        int nRooms = map.getNumberOfRooms();
        int currentRooms = 0;

        while(currentRooms < nRooms) {
            int randomIndex = 0;
            currentPos = currentPos + directions[randomIndex];

            TileType mapTile = static_cast<TileType>(mapData[currentPos.x][currentPos.y]);
            if(mapTile == TileType::Undefined) {
                mapData[currentPos.x][currentPos.y] = static_cast<int>(TileType::Empty);
                roomPositions.push_back(currentPos);
                nRooms += 1;
            }
        }

        if(!wall.isError()) {
            TransformComponent wallTransform = scene.getComponent<TransformComponent>(wall.getValue());
            wallTransform.setPosition(HUGO::Vector3(currentPos.x * map.getGridSize(), 0, currentPos.y * map.getGridSize()));
        }
    }
}

std::vector<HUGO::Vector2> 
MapSystem::getAdjacentFreePositions(HUGO::Vector2 pos, std::vector<std::vector<int>> mapData) {
    std::vector<HUGO::Vector2> freePositions;

    HUGO::Vector2 positionToCheck;
    for(int i = 0; i < directions.size(); i++) {

    }

    return freePositions;
}


void
MapSystem::update(uint32_t dt) 
{
}

void
MapSystem::fixedUpdate(uint32_t) {}
