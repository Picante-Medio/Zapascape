#include "MapSystem.h"
#include "MapComponent.h"
#include "TransformComponent.h"
#include <random>
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

void MapSystem::init() {}

void 
MapSystem::onLoad() {
    for (auto& pack: scene.query<MapComponent>()) {
        MapComponent& map = pack.getDeprecated<MapComponent>();
        map.mapData = std::vector<std::vector<int>>(map.mapHeight, std::vector<int>(map.mapWidth, static_cast<int>(TileType::Undefined)));

        HUGO::Vector2 startPos(map.mapWidth / 2.0f, map.mapHeight / 2.0f);
        map.mapData[static_cast<int>(startPos.y)][static_cast<int>(startPos.x)] = static_cast<int>(TileType::Start);

        HUGO::Vector2 currentPos = startPos;
        std::vector<HUGO::Vector2> roomPositions;
        
        int nRooms = map.getNumberOfRooms();
        int currentRooms = 0;

        while(currentRooms < nRooms) {
            int randomIndex = std::rand() % directions.size();
            currentPos = currentPos + directions[randomIndex];
            if (currentPos.x < 0 || currentPos.x >= map.mapWidth ||
                currentPos.y < 0 || currentPos.y >= map.mapHeight) {
                continue;
            }
            TileType mapTile = static_cast<TileType>(map.mapData[currentPos.x][currentPos.y]);
            if(mapTile == TileType::Undefined) {
                map.mapData[currentPos.x][currentPos.y] = static_cast<int>(TileType::Empty);
                roomPositions.push_back(currentPos);
                currentRooms += 1;
            }
        }

        for(HUGO::Vector2 pos : roomPositions) {
            std::vector<HUGO::Vector2> freePositions = getAdjacentFreePositions(pos, map.mapData);

            for(HUGO::Vector2 freePos : freePositions) {

                ErrorOrType<EntityID> wall = scene.instantiatePrefab("wall");
                if(!wall.isError()) {
                    TransformComponent& wallTransform = scene.getComponent<TransformComponent>(wall.getValue());
                    wallTransform.setPosition(HUGO::Vector3(freePos.x * map.getGridSize(), 0, freePos.y * map.getGridSize()));
                }
                map.mapData[freePos.x][freePos.y] = static_cast<int>(TileType::Wall);
            }
        }
        
        int playerIndex = std::rand() % roomPositions.size();
        ErrorOrType<EntityID> player = scene.instantiatePrefab("player");
        if(!player.isError()) {
            TransformComponent& playerTr = scene.getComponent<TransformComponent>(player.getValue());
            playerTr.setPosition(HUGO::Vector3(roomPositions[playerIndex].x * map.getGridSize(), 10, roomPositions[playerIndex].y * map.getGridSize()));
        }
        //createPlayer(HUGO::Vector3(roomPositions[playerIndex].x * map.getGridSize(), 0, roomPositions[playerIndex].y * map.getGridSize()));

        int beastIndex = std::rand() % roomPositions.size();
        while(beastIndex == playerIndex) {
            beastIndex = std::rand() % roomPositions.size();
        }
        createBeastie(HUGO::Vector3( roomPositions[beastIndex].x * map.getGridSize(),0,roomPositions[beastIndex].y * map.getGridSize()));
    
        
        createFloor(HUGO::Vector3(map.mapWidth * map.getGridSize() / 2.0f, 0,map.mapHeight * map.getGridSize() / 2.0f),map.mapWidth * map.getGridSize(), map.mapHeight * map.getGridSize());
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
void MapSystem::createPlayer(const HUGO::Vector3& pos){
    EntityID e1 = scene.createEntity();
    scene.addComponent<TransformComponent>(e1, [pos](TransformComponent& t1) {
        t1.setPosition(pos);

        t1.setRotation(HUGO::Quaternion(HUGO::Radian(HUGO::PI), HUGO::Vector3(0,1,0)));
        t1.setScale(HUGO::Vector3(0.5f, 0.5f, 0.5f));
    });
    scene.addComponent<CameraComponent>(e1, [](CameraComponent& comp){
        comp.setMain(true);
        comp.setNearClip(0.01f); 
        comp.setFarClip(1000.0f);
    });
    scene.addComponent<PlayerComponent>(e1);
    scene.addComponent<MovementRBComponent>(e1);
    scene.addComponent<ColliderComponent>(e1, [](ColliderComponent& c){
        c.info = HUGO::BoxColliderInfo{(HUGO::Vector3(1.2f, 1.0f, 1.2f))};
        c.setFriction(0.6f);
        c.setPositionOffset({0.0f, 1.0f, 0.0f});
    });
    scene.addComponent<RigidBodyComponent>(e1, [](RigidBodyComponent& rb){
        rb.setGravity(HUGO::Vector3(0.0f));
        rb.setMass(5.0f);
        rb.setLinearDamping(0.95f);
        rb.setBodyType(HUGO::BodyType::DYNAMIC);
    });

    EntityID playerArm = scene.createEntity();
    scene.addComponent<TransformComponent>(playerArm, [](TransformComponent& t1) {
        t1.setRotation(HUGO::Quaternion(HUGO::Radian(HUGO::PI), HUGO::Vector3(0,1,0)));
        t1.setPosition(HUGO::Vector3(0.0f, -3, -1.5f));
        t1.setScale(HUGO::Vector3(0.5f, 0.5f, 0.5f));
    });
    scene.addComponent<PlayerArmComponent>(playerArm);
    scene.addComponent<RenderComponent>(playerArm, [](RenderComponent& rc) {
        rc.setMeshName("player.mesh");
    });
    scene.addComponent<AnimationComponent>(playerArm);
    
    EntityID light = scene.createEntity();
    scene.addComponent<TransformComponent>(light, [](TransformComponent& tr){
        tr.setPosition(HUGO::Vector3(0.2, 1.0f, 0.1f));
        tr.setRotation(HUGO::Quaternion(HUGO::Degree(-10.0f), HUGO::Vector3(1, 0, 0)));
    });

    scene.addComponent<LightComponent>(light, [](LightComponent& lg){
        lg.setLightType(HUGO::LightType::LT_SPOTLIGHT);
        lg.setAttenuation(HUGO::Vector4(1.0f, 0.0f, 0.01f, 20.0f));
        lg.setDiffuse(HUGO::Vector4(0.9f, 0.85f, 0.7f, 1.0f));
        lg.setSpecular(HUGO::Vector4(0.5f, 0.5f, 0.5f, 1.0f));

        lg.setSpotInner(HUGO::Radian(HUGO::Degree(20.0f)));
        lg.setSpotOuter(HUGO::Radian(HUGO::Degree(35.0f)));
        lg.setSpotFallOff(1.0f); 
        lg.setSpotNearClip(0.2f);
     });
    
     scene.addComponent<ChildrenComponent>(e1, [light, playerArm](ChildrenComponent& c){ 
        c.addChild(light);
        c.addChild(playerArm);
    });

}
void MapSystem::createBeastie(const HUGO::Vector3& pos){
    EntityID e2 = scene.createEntity();
    scene.addComponent<TransformComponent>(e2, [pos](TransformComponent& t2) {  
        t2.setRotation(HUGO::Quaternion(HUGO::Radian(HUGO::PI), HUGO::Vector3(0,1,0)));
        t2.setPosition(pos);
        t2.setScale(HUGO::Vector3(0.4f, 0.4f, 0.4f));
    });
    scene.addComponent<RenderComponent>(e2, [](RenderComponent& rc) {
        rc.setMeshName("beastie.mesh");
    });
    scene.addComponent<AnimationComponent>(e2);
    scene.addComponent<ColliderComponent>(e2, [](ColliderComponent& c){
        c.info = HUGO::BoxColliderInfo{(HUGO::Vector3(0.5f))};
        c.positionOffset = HUGO::Vector3(0,0.6,0);
        c.setFriction(0.1f);
    });
    scene.addComponent<MovementRBComponent>(e2);
    scene.addComponent<BeastieComponent>(e2);
    scene.addComponent<RigidBodyComponent>(e2, [](RigidBodyComponent& rb){
        rb.setGravity(HUGO::Vector3(0.0f));
        rb.setMass(5.0f);
        rb.setLinearDamping(0.95f);
        rb.setBodyType(HUGO::BodyType::DYNAMIC);
    });
    
}

void MapSystem::createFloor(const HUGO::Vector3& pos, int width, int height)
{
    EntityID wall = scene.createEntity();
    scene.addComponent<TransformComponent>(wall, [pos, width, height](TransformComponent& t){
        t.setPosition(HUGO::Vector3(pos.x, -1, pos.z));
        t.setScale(HUGO::Vector3(width, 0.1, height));
    });
    scene.addComponent<ColliderComponent>(wall, [width, height](ColliderComponent& c){
        c.info = HUGO::BoxColliderInfo{HUGO::Vector3(width / 2.0f, 0.1f, height / 2.0f)};
        c.setFriction(0.6f);
    });
    scene.addComponent<RenderComponent>(wall, [](RenderComponent& r){
        r.meshName = "cube.mesh";
    });
    scene.addComponent<RigidBodyComponent>(wall, [](RigidBodyComponent& rb){
        rb.setBodyType(HUGO::BodyType::STATIC);
    });
}
