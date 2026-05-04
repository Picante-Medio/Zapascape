#include "MapSystem.h"
#include "EcsTypes.h"
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
#include "ChildrenComponent.h"
#include "ColliderComponent.h"
#include "RitualComponent.h"

void MapSystem::init() {
    mt = std::mt19937(std::random_device{}());
    directionsDist = std::uniform_int_distribution<int>(0, directions.size()-1);
}

void 
MapSystem::onLoad() {
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

        for(HUGO::Vector2 pos : roomPositions) {
            std::vector<HUGO::Vector2> freePositions = getAdjacentFreePositions(pos, map.mapData);

            for(const HUGO::Vector2& freePos : freePositions) {

                HUGO::ErrorOrType<EntityID> wall = scene.instantiatePrefab(map.wallPrefab);
                if(!wall.isError()) {
                    TransformComponent& wallTransform = scene.getComponent<TransformComponent>(wall.getValue());
                    wallTransform.setPosition(HUGO::Vector3(freePos.x * map.getGridSize(), 0, freePos.y * map.getGridSize()));
                }
                map.mapData[freePos.x][freePos.y] = static_cast<HUGO::Int>(TileType::Wall);
            }
        }
        
        createPlayer(HUGO::Vector3(startPos.x * map.getGridSize(), 0, startPos.y * map.getGridSize()));
        
        std::uniform_int_distribution<HUGO::Int> roomDist(0, roomPositions.size()-1);
        
        HUGO::Int beastIndex = roomDist(mt);
        createBeastie(HUGO::Vector3( roomPositions[beastIndex].x * map.getGridSize(),0,roomPositions[beastIndex].y * map.getGridSize()));
        
        std::vector<HUGO::Int> ritualIndices;
        for(HUGO::Int i = 0; i < 2; ++i) {
            HUGO::Int ritualIndex = roomDist(mt);
            while(ritualIndex == beastIndex || std::find(ritualIndices.begin(), ritualIndices.end(), ritualIndex) != ritualIndices.end()) {
                ritualIndex = roomDist(mt);
            }
            ritualIndices.push_back(ritualIndex);
            createRitual(HUGO::Vector3(roomPositions[ritualIndex].x * map.getGridSize(), -0.9f, roomPositions[ritualIndex].y * map.getGridSize()));
        }

        //suelo y techo
        HUGO::Vector3 floorPos(map.mapWidth * map.getGridSize() / 2.0f, 0, map.mapHeight * map.getGridSize() / 2.0f);
        HUGO::Vector3 floorScale(map.mapWidth / 2.0f, 0.1f, map.mapHeight / 2.0f);
        createFloor(floorPos, floorScale);
        createFloor(HUGO::Vector3(floorPos.x, map.getGridSize(), floorPos.z), floorScale);
    }
}

std::vector<HUGO::Vector2> 
MapSystem::getAdjacentFreePositions(HUGO::Vector2 pos, std::vector<std::vector<HUGO::Int>> mapData) {
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


void
MapSystem::update(uint32_t) {}

void
MapSystem::fixedUpdate(uint32_t) {}
void MapSystem::createPlayer(const HUGO::Vector3& pos){

    // EntityID e1 = scene.createEntity();
    // scene.addComponent<TransformComponent>(e1, [pos](TransformComponent& t1) {
    //     t1.setPosition(pos + HUGO::Vector3(0.0f,1.0f,0.0f));
    // });
    // scene.addComponent<PlayerComponent>(e1);
    // scene.addComponent<MovementRBComponent>(e1);
    // scene.addComponent<ColliderComponent>(e1, [](ColliderComponent& c){
    //     c.info = HUGO::BoxColliderInfo{(HUGO::Vector3(0.6f, 0.6f, 0.6f))};
    //     c.setFriction(0.9f);
    //     c.setPositionOffset({0.0f, 0.2f, 0.0f});
    // });
    // scene.addComponent<RigidBodyComponent>(e1, [](RigidBodyComponent& rb){
    //     rb.setGravity(HUGO::Vector3(0.0f));
    //     rb.setMass(5.0f);
    //     rb.setLinearDamping(0.95f);
    //     rb.setBodyType(HUGO::BodyType::DYNAMIC);
    // });
    // scene.addComponent<PlayerArmComponent>(e1);
    // scene.addComponent<RenderComponent>(e1, [](RenderComponent& rc) {
    //     rc.setMeshName("player.mesh");
    // });
    // scene.addComponent<AnimationComponent>(e1);
    

    // EntityID camera = scene.createEntity();
    // scene.addComponent<TransformComponent>(camera, [](TransformComponent& t1) {
    //     t1.setPosition(HUGO::Vector3(0.0f, 0, 0.1f));
    // });
    // scene.addComponent<CameraComponent>(camera, [](CameraComponent& comp){
    //     comp.setMain(true);
    //     comp.setNearClip(0.01f); 
    //     comp.setFarClip(1000.0f);
    // });
    // EntityID light = scene.createEntity();
    // scene.addComponent<TransformComponent>(light, [](TransformComponent& tr){
    //     tr.setPosition(HUGO::Vector3(0.0, 0.0f, 0.1f)); });

    // scene.addComponent<LightComponent>(light, [](LightComponent& lg){
    //     lg.setLightType(HUGO::LightType::LT_SPOTLIGHT);
    //     lg.setAttenuation(HUGO::Vector4(1.0f, 0.0f, 0.01f, 20.0f));
    //     lg.setDiffuse(HUGO::Vector4(0.9f, 0.85f, 0.7f, 1.0f));
    //     lg.setSpecular(HUGO::Vector4(0.5f, 0.5f, 0.5f, 1.0f));

    //     lg.setSpotInner(HUGO::Radian(HUGO::Degree(20.0f)));
    //     lg.setSpotOuter(HUGO::Radian(HUGO::Degree(35.0f)));
    //     lg.setSpotFallOff(1.0f); 
    //     lg.setSpotNearClip(0.2f);
    //  });
    
    //  scene.addComponent<ChildrenComponent>(e1, [light, camera](ChildrenComponent& c){ 
    //     c.addChild(light);
    //     c.addChild(camera);
    // });

    HUGO::ErrorOrType<EntityID> player = scene.instantiatePrefab("player");
    if(!player.isError()) {
        scene.modifyComponent<TransformComponent>(player.getValue(), [pos](TransformComponent& tr){
            tr.setPosition(pos + HUGO::Vector3(0.0f,1.0f,0.0f));
        });
    }

}
void MapSystem::createBeastie(const HUGO::Vector3& pos){
    HUGO::ErrorOrType<EntityID> beastie = scene.instantiatePrefab("beastie");
    if(!beastie.isError()) {
        TransformComponent& beastieTr = scene.getComponent<TransformComponent>(beastie.getValue());
        beastieTr.setPosition(pos);
    }
}

void MapSystem::createRitual(const HUGO::Vector3 &pos)
{
    EntityID ritual = scene.createEntity(); 
    scene.addComponent<TransformComponent>(ritual, [pos](TransformComponent& t3) {
        t3.setPosition(pos + HUGO::Vector3(0.0f,1.0f,0.0f));
        t3.setScale(HUGO::Vector3(0.5f, 0.5f, 0.5f));
    });
    scene.addComponent<RenderComponent>(ritual, [](RenderComponent& rc3) {
        rc3.setMeshName("ritual.glb");
    });
    scene.addComponent<ColliderComponent>(ritual, [](ColliderComponent& cc3) {
        cc3.info = HUGO::BoxColliderInfo{HUGO::Vector3(0.5f, 0.5f, 0.5f)};
        cc3.positionOffset = HUGO::Vector3(0.0f,0.3f,0.0f);
        cc3.isTrigger = true;
    });

    EntityID cube = scene.createEntity();
    scene.addComponent<TransformComponent>(cube, [pos](TransformComponent& t) {
        t.setPosition(pos + HUGO::Vector3(0.0f, 0.25f, 0.0f));
        t.setScale(HUGO::Vector3(0.15f, 0.15f, 0.15f));
    });
    
    scene.addComponent<RenderComponent>(cube, [](RenderComponent& rc) {
        rc.setMeshName("cube.mesh");
        rc.visible = false;
    });

    scene.addComponent<RitualComponent>(ritual, [cube](RitualComponent& rc){
        rc.setActive(true);
        rc.associatedCubo = cube;
    });

}

void MapSystem::createFloor(const HUGO::Vector3 &pos, const HUGO::Vector3 & scale)
{
    HUGO::ErrorOrType<EntityID> floor = scene.instantiatePrefab("floor");
    if(!floor.isError()) {
        TransformComponent& floorTr = scene.getComponent<TransformComponent>(floor.getValue());
        floorTr.setPosition(pos + HUGO::Vector3(0.0f, -scale.y/2.0f, 0.0f));
        floorTr.setScale(scale);

        scene.modifyComponent<ColliderComponent>(floor.getValue(), [scale](ColliderComponent& collider){
            collider.info = HUGO::BoxColliderInfo{scale};
        });
    }
}
