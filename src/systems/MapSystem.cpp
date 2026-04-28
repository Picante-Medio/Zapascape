#include "MapSystem.h"
#include "MapComponent.h"
#include "TransformComponent.h"
#include <random>

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
        std::vector<HUGO::Vector2> roomPositions;
        
        int nRooms = map.getNumberOfRooms();
        int currentRooms = 0;

        while(currentRooms < nRooms) {
            int randomIndex = std::rand() % directions.size();
            currentPos = currentPos + directions[randomIndex];

            TileType mapTile = static_cast<TileType>(mapData[currentPos.x][currentPos.y]);
            if(mapTile == TileType::Undefined) {
                mapData[currentPos.x][currentPos.y] = static_cast<int>(TileType::Empty);
                roomPositions.push_back(currentPos);
                nRooms += 1;
            }
        }

        for(HUGO::Vector2 pos : roomPositions) {
            std::vector<HUGO::Vector2> freePositions = getAdjacentFreePositions(pos, mapData);

            for(HUGO::Vector2 freePos : freePositions) {

                ErrorOrType<EntityID> wall = scene.instantiatePrefab("wall");

                if(!wall.isError()) {
                    TransformComponent wallTransform = scene.getComponent<TransformComponent>(wall.getValue());
                    wallTransform.setPosition(HUGO::Vector3(currentPos.x * map.getGridSize(), 0, currentPos.y * map.getGridSize()));
                }

                mapData[freePos.x][freePos.y] = static_cast<int>(TileType::Wall);
            }
        }

    }
}

std::vector<HUGO::Vector2> 
MapSystem::getAdjacentFreePositions(HUGO::Vector2 pos, std::vector<std::vector<int>> mapData) {
    std::vector<HUGO::Vector2> freePositions;

    for(int i = 0; i < directions.size(); i++) {
        HUGO::Vector2 positionToCheck = pos + directions[i];
        if(positionToCheck.x >= 0 && positionToCheck.x < mapData.size() && positionToCheck.y >= 0 && positionToCheck.y < mapData[0].size()) {
            if(mapData[positionToCheck.x][positionToCheck.y] == static_cast<int>(TileType::Undefined)) {
                freePositions.push_back(positionToCheck);
            }
        }

    }

    return freePositions;
}


void
MapSystem::update(uint32_t dt) 
{
}

void
MapSystem::fixedUpdate(uint32_t) {}
