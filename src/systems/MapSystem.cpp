#include "MapSystem.h"
#include "MapComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "AnimationComponent.h"
#include "PlayerComponent.h"
#include "RigidBodyComponent.h"
#include "MovementRBComponent.h"
#include "BeastieComponent.h"
#include "LightComponent.h"
#include "CameraComponent.h"
#include "ParentComponent.h"
#include "ColliderComponent.h"
#include "RitualComponent.h"
#include "DoorComponent.h"
#include "AudioComponent.h"

void MapSystem::init() {
    scene.changeAmbientLight(HUGO::Vector3(0.1f));
}

void 
MapSystem::onLoad() {
    mt = std::mt19937(std::random_device{}());
    directionsDist = std::uniform_int_distribution<int>(0, directions.size()-1);
    for (auto& pack: scene.query<MapComponent>()) {
        MapComponent& map = pack.getDeprecated<MapComponent>();
        map.mapData = std::vector<std::vector<HUGO::Int>>(map.mapWidth, std::vector<HUGO::Int>(map.mapHeight, static_cast<HUGO::Int>(TileType::Undefined)));

        HUGO::Vector2 startPos(map.mapWidth / 2.0f, map.mapHeight / 2.0f);
        map.mapData[static_cast<HUGO::Int>(startPos.x)][static_cast<HUGO::Int>(startPos.y)] = static_cast<HUGO::Int>(TileType::Start);

        HUGO::Vector2 currentPos = startPos;
        std::vector<HUGO::Vector2> roomPositions;
        
        HUGO::Int nRooms = map.getNumberOfRooms();
        HUGO::Int currentRooms = 0;

        while(currentRooms < nRooms) {
            HUGO::Int randomIndex = directionsDist(mt);
            currentPos = currentPos + directions[randomIndex];
            if (currentPos.x < 0 || currentPos.x >= map.mapWidth ||
                currentPos.y < 0 || currentPos.y >= map.mapHeight) {
                continue;
            }
            TileType mapTile = static_cast<TileType>(map.mapData[currentPos.x][currentPos.y]);
            if(mapTile == TileType::Undefined) {
                map.mapData[currentPos.x][currentPos.y] = static_cast<HUGO::Int>(TileType::Empty);
                roomPositions.push_back(currentPos);
                currentRooms += 1;
            }
        }

        for(HUGO::Int i =0; i<roomPositions.size(); ++i) {
            auto pos = roomPositions[i];
            HUGO::Int off = (HUGO::Int)(roomPositions.size()/10);
            if (i% off ==0){
                HUGO::Vector3 worldPos(pos.x * map.getGridSize(), 3.0f, pos.y * map.getGridSize());
                createRoomLight(worldPos);
            }

            std::vector<HUGO::Vector2> freePositions = getAdjacentFreePositions(pos, map.mapData);

            for(const HUGO::Vector2& freePos : freePositions) {

                HUGO::ErrorOrType<EntityID> wall = scene.instantiatePrefab(map.wallPrefab);
                if(!wall.isError()) {
                    scene.modifyComponent<TransformComponent>(wall.getValue(), 
                    [&freePos, &map](TransformComponent& wallTransform){
                        wallTransform.position = HUGO::Vector3(freePos.x * map.getGridSize(), 0, freePos.y * map.getGridSize());
                    });
                }
                map.mapData[freePos.x][freePos.y] = static_cast<HUGO::Int>(TileType::Wall);
            }
        }
        

        HUGO::Vector3 start =HUGO::Vector3(startPos.x * map.getGridSize(), 0, startPos.y * map.getGridSize());
        HUGO::Vector2 doorDirection = roomPositions[0] - startPos; 
        createDoor(start, doorDirection);

        std::uniform_int_distribution<HUGO::Int> roomDist(0, roomPositions.size()-1);
        
        HUGO::Int playerIndex = roomDist(mt);
        HUGO::Int beastIndex;

        HUGO::Vector3 playerPos = HUGO::Vector3(roomPositions[playerIndex].x * map.getGridSize(), -2.5f, roomPositions[playerIndex].y * map.getGridSize());
        HUGO::Vector3 beastiePos;

        int minDist = 10; 
        int cont = 10;
        do {
            beastIndex = roomDist(mt);
            beastiePos = HUGO::Vector3(roomPositions[beastIndex].x * map.getGridSize(), -2.5f, roomPositions[beastIndex].y * map.getGridSize());
            cont--;
        } while(cont >=0 && ( beastIndex == playerIndex || (beastiePos - playerPos).length() < minDist * map.getGridSize()));
        
        std::vector<HUGO::Int> ritualIndices;
        for(HUGO::Int i = 0; i < map.numRituals; ++i) {
            HUGO::Int ritualIndex = roomDist(mt);
            while(ritualIndex == beastIndex || std::find(ritualIndices.begin(), ritualIndices.end(), ritualIndex) != ritualIndices.end()) {
                ritualIndex = roomDist(mt);
            }
            ritualIndices.push_back(ritualIndex);
            HUGO::ErrorOrType<EntityID> ritual = scene.instantiatePrefab("ritual");
            if(!ritual.isError()) {
                scene.modifyComponent<TransformComponent>(ritual.getValue(),
                    [&roomPositions, &ritualIndex, &map](TransformComponent& ritualTr){
                    ritualTr.position = HUGO::Vector3( roomPositions[ritualIndex].x * map.getGridSize(), -2.48f, roomPositions[ritualIndex].y * map.getGridSize());
                });

                scene.modifyComponent<ColliderComponent>(ritual.getValue(),
                    [](ColliderComponent& coll){
                        coll.isTrigger = true;
                    }
                );
            }
            createRitual(HUGO::Vector3(roomPositions[ritualIndex].x * map.getGridSize(), -2.48f, roomPositions[ritualIndex].y * map.getGridSize()), ritual.getValue());
        }

        //suelo y techo
        HUGO::Vector3 floorPos(map.mapWidth * map.getGridSize() / 2.0f, -2.5, map.mapHeight * map.getGridSize() / 2.0f);
        HUGO::Vector3 floorScale(map.mapWidth / 2.0f, 0.1f, map.mapHeight / 2.0f);
        createFloor(floorPos, floorScale, "floor");
        createFloor(HUGO::Vector3(floorPos.x, map.getGridSize()/2, floorPos.z), floorScale, "ceiling");

        createPlayer(playerPos);
        createBeastie(beastiePos);
    }
}

std::vector<HUGO::Vector2> 
MapSystem::getAdjacentFreePositions(const HUGO::Vector2& pos, const std::vector<std::vector<HUGO::Int>>& mapData) {
    std::vector<HUGO::Vector2> freePositions;

    for(HUGO::Int i = 0; i < directions.size(); i++) {
        HUGO::Vector2 positionToCheck = pos + directions[i];
        if(positionToCheck.x >= 0 && positionToCheck.x < mapData.size() && positionToCheck.y >= 0 && positionToCheck.y < mapData[0].size()) {
            if(mapData[positionToCheck.x][positionToCheck.y] == static_cast<HUGO::Int>(TileType::Undefined)) {
                freePositions.push_back(positionToCheck);
            }
        }

    }

    return freePositions;
}


void MapSystem::update(uint32_t) {}
void MapSystem::fixedUpdate(uint32_t) {}

void MapSystem::createPlayer(const HUGO::Vector3& pos){

    HUGO::ErrorOrType<EntityID> player = scene.instantiatePrefab("player");
    if(!player.isError()) {
        scene.modifyComponent<TransformComponent>(player.getValue(), [pos](TransformComponent& tr){
            tr.position = pos + HUGO::Vector3(0.0f,1.0f,0.0f);
        });
        scene.modifyComponent<LightComponent>(player.getValue(), [](LightComponent& l){
            l.setLightType(HUGO::LightType::LT_SPOTLIGHT);
            l.setSpotInner(HUGO::Radian(HUGO::Degree(5.0f)));
        });
        scene.modifyComponent<AudioComponent>(player.getValue(), [](AudioComponent& a){
            a.type = HUGO::SOUND2D;
        });
    }

}
void MapSystem::createBeastie(const HUGO::Vector3& pos){
    HUGO::ErrorOrType<EntityID> beastie = scene.instantiatePrefab("beastie");
    if(!beastie.isError()) {
        scene.modifyComponent<TransformComponent>(beastie.getValue(), [&pos](TransformComponent& beastieTr){
            beastieTr.position = pos;
        });
    }
}

void MapSystem::createRitual(const HUGO::Vector3 &pos, EntityID ritual)
{
    EntityID cube = scene.instantiatePrefab("auxiliarCube").getValue();
    scene.modifyComponent<TransformComponent>(cube, [pos](TransformComponent& t) {
        t.position.y += 1.0f;
    });
    scene.addComponent<RitualComponent>(ritual, [cube](RitualComponent& rc){
        rc.associatedCubo = cube;
        rc.isActive= true;
    });
}

void MapSystem::createFloor(const HUGO::Vector3 &pos, const HUGO::Vector3 & scale, const std::string& prefab)
{
    HUGO::ErrorOrType<EntityID> floor = scene.instantiatePrefab(prefab);
    if(!floor.isError()) {
        scene.modifyComponent<TransformComponent>(floor.getValue(), [pos, scale](TransformComponent& floorTr){
            floorTr.position = pos + HUGO::Vector3(0.0f, -scale.y/2.0f, 0.0f);
            floorTr.scale = scale;
        });
        scene.modifyComponent<ColliderComponent>(floor.getValue(), [scale](ColliderComponent& collider){
            collider.info = HUGO::BoxColliderInfo{scale};
        });
    }
}

void MapSystem::createRoomLight(const HUGO::Vector3& pos) {
    HUGO::ErrorOrType<EntityID> light = scene.instantiatePrefab("ceilingLight");
    scene.modifyComponent<TransformComponent>(light.getValue(), [pos](TransformComponent& tr){
        tr.position = pos;
    });
    scene.modifyComponent<LightComponent>(light.getValue(), [](LightComponent& lg){
        lg.setLightType(HUGO::LightType::LT_POINT);
    });
}

void MapSystem::createDoor(const HUGO::Vector3 &pos, const HUGO::Vector2& dir)
{
    HUGO::ErrorOrType<EntityID> door = scene.instantiatePrefab("door");
    if(!door.isError()) {
        TransformComponent& doorTr = scene.getComponent<TransformComponent>(door.getValue());
        doorTr.position = pos;

        float rotationY = 0.0f;
        if(dir.x > 0) rotationY = 90.0f;     
        else if(dir.x < 0) rotationY = -90.0f;
        else if(dir.y > 0) rotationY = 0.0f;  
        else if(dir.y < 0) rotationY = 180.0f; 
        
        HUGO::Radian angleRad = rotationY * (HUGO::PI / 180.0f);
        HUGO::Quaternion finalRotation = HUGO::Quaternion(angleRad, HUGO::Vector3(0, 1, 0));
        doorTr.rotation = finalRotation;

        //PopUp
        EntityID cube = scene.instantiatePrefab("auxiliarCube").getValue();
        HUGO::Vector3 forward(dir.x, 0.0f, dir.y);
        forward.normalize();
        scene.modifyComponent<TransformComponent>(cube, [pos, forward, finalRotation](TransformComponent& t) {
            t.position = pos + forward;
            t.rotation = finalRotation;
            t.scale = {0.3f, 0.3f, 0.3f};
        });

        scene.addComponent<DoorComponent>(door.getValue(), [cube](DoorComponent& dc){
            dc.associatedCubo = cube;
        });
        scene.modifyComponent<AudioComponent>(door.getValue(), [](AudioComponent& a){
            a.type = HUGO::SOUND2D;
        });
    }
}
