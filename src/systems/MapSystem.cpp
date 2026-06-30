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

    //esto no es un bucle ya que solo existe un map component
    for (auto& pack: scene.query<MapComponent>()) {
        
        HUGO::Vector2 startPos;
        std::vector<HUGO::Vector2> roomPositions;

        pack.modify<MapComponent>([this, &startPos, &roomPositions](MapComponent& map) {

            map.mapData = std::vector<std::vector<HUGO::Int>>(
                map.mapWidth, 
                std::vector<HUGO::Int>(
                    map.mapHeight, 
                    static_cast<HUGO::Int>(TileType::Undefined)
                )
            );

            startPos = HUGO::Vector2(map.mapWidth / 2.0f, map.mapHeight / 2.0f);

            map.mapData[static_cast<HUGO::Int>(startPos.x)][static_cast<HUGO::Int>(startPos.y)] = static_cast<HUGO::Int>(TileType::Start);
        
            HUGO::Vector2 currentPos = startPos;
            
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
        });

        const MapComponent& mapRead = pack.getConst<MapComponent>();
        mapID = mapRead.entity;
        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float minZ = std::numeric_limits<float>::max();
        float maxZ = std::numeric_limits<float>::lowest();

        //casillas de mapa vacias-> se instancian los prefabs de los muros etc a lavez que se genera el mapa
        for(HUGO::Int i =0; i<roomPositions.size(); ++i) {
            auto pos = roomPositions[i];
            // HUGO::Int off = (HUGO::Int)(roomPositions.size()/10);
            // if (i% off ==0){
            //     HUGO::Vector3 worldPos(pos.x * mapRead.getGridSize(), mapRead.getGridSize()/2 -0.5f, pos.y * mapRead.getGridSize());
            //     createRoomLight(worldPos);
            // }

            std::vector<HUGO::Vector2> freePositions = getAdjacentFreePositions(pos, mapRead.mapData);
            std::cout<<"{";
            for(const HUGO::Vector2& freePos : freePositions) {

                //HUGO::ErrorOrType<EntityID> wall = scene.instantiatePrefab(mapRead.wallPrefab);
                float wallWorldX = freePos.x * mapRead.getGridSize();
                float wallWorldZ = freePos.y * mapRead.getGridSize();
                minX = std::min(minX, wallWorldX);
                maxX = std::max(maxX, wallWorldX);
                minZ = std::min(minZ, wallWorldZ);
                maxZ = std::max(maxZ, wallWorldZ);

                /*if(!wall.isError()) {
                    scene.modifyComponent<TransformComponent>(wall.getValue(), 
                    [&freePos, &mapRead](TransformComponent& wallTransform){
                        wallTransform.position = HUGO::Vector3(freePos.x * mapRead.getGridSize(), 0, freePos.y * mapRead.getGridSize());
                    });
                }*/

                pack.modify<MapComponent>([&freePos](MapComponent& map){
                    map.mapData[freePos.x][freePos.y] = static_cast<HUGO::Int>(TileType::Wall);
                });
            }
            
        }
        
        HUGO::Vector3 start = HUGO::Vector3(startPos.x * mapRead.getGridSize(), 0, startPos.y * mapRead.getGridSize());
        HUGO::Vector2 doorDirection = roomPositions[0] - startPos; 
        createDoor(start, doorDirection);

        std::uniform_int_distribution<HUGO::Int> roomDist(0, roomPositions.size()-1);
        
        HUGO::Int playerIndex = roomDist(mt);
        HUGO::Int beastIndex;

        playerPos = HUGO::Vector3(
                roomPositions[playerIndex].x * mapRead.getGridSize(), 
                -2.5f, roomPositions[playerIndex].y * mapRead.getGridSize());

        

        int minDist = 10; 
        int cont = 10;
        do {
            beastIndex = roomDist(mt);
            beastiePos = HUGO::Vector3(
                roomPositions[beastIndex].x * mapRead.getGridSize(), 
                -2.5f, roomPositions[beastIndex].y * mapRead.getGridSize());
            cont--;
        } 
        while(
            cont >=0 && (
                beastIndex == playerIndex 
                || (beastiePos - playerPos).length() < minDist * mapRead.getGridSize()
            )
        );
        
        std::vector<HUGO::Int> ritualIndices;
        for(HUGO::Int i = 0; i < mapRead.numRituals; ++i) {
            HUGO::Int ritualIndex = roomDist(mt);
            while(ritualIndex == beastIndex || std::find(ritualIndices.begin(), ritualIndices.end(), ritualIndex) != ritualIndices.end()) {
                ritualIndex = roomDist(mt);
            }
            ritualIndices.push_back(ritualIndex);
            HUGO::ErrorOrType<EntityID> ritual = scene.instantiatePrefab("ritual");
            if(!ritual.isError()) {
                scene.modifyComponent<TransformComponent>(ritual.getValue(),
                    [&roomPositions, &ritualIndex, &mapRead](TransformComponent& ritualTr){
                    ritualTr.position = HUGO::Vector3(
                        roomPositions[ritualIndex].x * mapRead.getGridSize(), 
                        -2.48f, roomPositions[ritualIndex].y * mapRead.getGridSize()
                    );
                });

                scene.modifyComponent<ColliderComponent>(ritual.getValue(),
                    [](ColliderComponent& coll){
                        coll.isTrigger = true;
                    }
                );
                createRitual(HUGO::Vector3(
                roomPositions[ritualIndex].x * mapRead.getGridSize(), 
                -1.5f, roomPositions[ritualIndex].y * mapRead.getGridSize()), 
                ritual.getValue());
            }
        }

        float centerX = (minX + maxX) / 2.0f;
        float centerZ = (minZ + maxZ) / 2.0f;
        float widthWorld = maxX - minX + mapRead.getGridSize();
        float heightWorld = maxZ - minZ + mapRead.getGridSize();
        
        HUGO::Vector3 floorPos(centerX, -2.5f, centerZ);
        HUGO::Vector3 floorScale(widthWorld, 0.1f, heightWorld);

        /*for (int x = 0; x < mapRead.mapWidth; ++x)
        {
            for (int y = 0; y < mapRead.mapHeight; ++y)
            {
                TileType tile = static_cast<TileType>(mapRead.mapData[x][y]);

                if (tile == TileType::Empty || tile == TileType::Start)
                {
                    HUGO::Vector3 pos(x * mapRead.getGridSize(), -2.5f, y * mapRead.getGridSize());

                    createFloor(pos, HUGO::Vector3(mapRead.getGridSize(), 0.1f, mapRead.getGridSize()), "floor");
                    createFloor(HUGO::Vector3(pos.x, mapRead.getGridSize() / 2.0f, pos.z),HUGO::Vector3(mapRead.getGridSize(), 0.1f, mapRead.getGridSize()),"ceiling");
                }
                else if(tile == TileType::Wall){
                    HUGO::ErrorOrType<EntityID> wall = scene.instantiatePrefab(mapRead.wallPrefab);

                    if(!wall.isError()) {
                        scene.modifyComponent<TransformComponent>(wall.getValue(), 
                        [this, x, y](TransformComponent& wallTransform){
                            wallTransform.position = HUGO::Vector3(x * scene.getComponentConst<MapComponent>(mapID).getGridSize(), 0, y * scene.getComponentConst<MapComponent>(mapID).getGridSize());
                        });

                    }
                }
            }
        }*/
       
            createPlayer(playerPos);
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


void MapSystem::update(uint32_t) {
    if(!mapGenerated) createNextTile();
}
void MapSystem::fixedUpdate(uint32_t) {}

void MapSystem::createPlayer(const HUGO::Vector3& pos){

    HUGO::ErrorOrType<EntityID> player = scene.instantiatePrefab("player");
    if(!player.isError()) {
        scene.modifyComponent<TransformComponent>(player.getValue(), [pos](TransformComponent& tr){
            tr.position = pos + HUGO::Vector3(0.0f,1.0f,0.0f);
            //tr.rotation = HUGO::Quaternion(HUGO::Radian(HUGO::Degree(-90.0f)), HUGO::Vector3(1,0,0));
        });
        scene.modifyComponent<LightComponent>(player.getValue(), [](LightComponent& l){
            l.setLightType(HUGO::LightType::LT_SPOTLIGHT);
            l.setSpotInner(HUGO::Radian(HUGO::Degree(5.0f)));
        });
    }
    else {
        std::cout<< "[Error]: Fallo al instanciar Player, cerrando el juego\n";
        scene.exitGame();
    }
}

void MapSystem::createBeastie(const HUGO::Vector3& pos){
    HUGO::ErrorOrType<EntityID> beastie = scene.instantiatePrefab("beastie");
    if(!beastie.isError()) {
        scene.modifyComponent<TransformComponent>(beastie.getValue(), [&pos](TransformComponent& beastieTr){
            beastieTr.position = pos;
        });
    }
    else {
        std::cout<< "[Error]: Fallo al instanciar Beastie\n";
    }
}

void MapSystem::createRitual(const HUGO::Vector3 &pos, EntityID ritual)
{
    HUGO::ErrorOrType<EntityID> cube = scene.instantiatePrefab("auxiliarCube");
    if(!cube.isError()){
        EntityID ent = cube.getValue();
        scene.modifyComponent<TransformComponent>(ent, [pos](TransformComponent& t) {
            t.position.y += 1.0f;
        });
        scene.addComponent<RitualComponent>(ritual, [ent](RitualComponent& rc){
            rc.associatedCubo = ent;
            rc.isActive= true;
        });
    }
    else {
        std::cout<< "[Error]: Fallo al instanciar el prefab del ritual\n";
    }
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
    else {
        std::cout<< "[Error]: Fallo al instanciar el prefab del suelo\n";
    }
}

void MapSystem::createRoomLight(const HUGO::Vector3& pos) {
    HUGO::ErrorOrType<EntityID> light = scene.instantiatePrefab("ceilingLight");
    if(!light.isError()){
        scene.modifyComponent<TransformComponent>(light.getValue(), [pos](TransformComponent& tr){
            tr.position = pos;
            tr.rotation = HUGO::Quaternion(HUGO::Radian(HUGO::Degree(-90.0f)), HUGO::Vector3(1,0,0));
        });
        scene.modifyComponent<LightComponent>(light.getValue(), [](LightComponent& lg)
        {
            lg.setLightType(HUGO::LightType::LT_SPOTLIGHT);

            lg.setSpotInner(HUGO::Radian(HUGO::Degree(10.0f)));
            lg.setSpotOuter(HUGO::Radian(HUGO::Degree(60.0f)));

            lg.setSpotNearClip(0.1f);

            lg.setAttenuation(HUGO::Vector4(1.0f, 0.09f, 0.032f, 0.0f));

            lg.setShadowFarClipDist(200.0f);
            lg.setShadowNearClipDist(0.1f);
            
        });
    }
    else {
        std::cout<< "[Error]:Fallo al instanciar el prefab del tejado\n";
    }
}

void MapSystem::createDoor(const HUGO::Vector3 &pos, const HUGO::Vector2& dir)
{
    HUGO::ErrorOrType<EntityID> door = scene.instantiatePrefab("door");
    if(!door.isError()) {

        float rotationY = 0.0f;
        if(dir.x > 0) rotationY = 90.0f;     
        else if(dir.x < 0) rotationY = -90.0f;
        else if(dir.y > 0) rotationY = 0.0f;  
        else if(dir.y < 0) rotationY = 180.0f; 
        
        HUGO::Radian angleRad = rotationY * (HUGO::PI / 180.0f);
        HUGO::Quaternion finalRotation = HUGO::Quaternion(angleRad, HUGO::Vector3(0, -1, 0));

        scene.modifyComponent<TransformComponent>(door.getValue(), [&pos, &finalRotation](TransformComponent& doorTr){

            doorTr.position = pos;
            doorTr.rotation = finalRotation;
        });

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
    }
    else {
        std::cout<< "[Error]: Fallo al instanciar el prefab de la puerta final\n";
    }
}

void MapSystem::createNextTile(){

    const MapComponent& map = scene.getComponentConst<MapComponent>(mapID);
    TileType tile = static_cast<TileType>(map.mapData[actX][actY]);

    if (tile == TileType::Empty || tile == TileType::Start)
    {
        HUGO::Vector3 pos(actX * map.getGridSize(), -2.5f, actY * map.getGridSize());

        createFloor(pos, HUGO::Vector3(map.getGridSize(), 0.1f, map.getGridSize()), "floor");
        createFloor(HUGO::Vector3(pos.x, map.getGridSize() / 2.0f, pos.z),HUGO::Vector3(map.getGridSize(), 0.1f, map.getGridSize()),"ceiling");
        increasesIt();
    }
    else if(tile == TileType::Wall){
        HUGO::ErrorOrType<EntityID> wall = scene.instantiatePrefab(map.wallPrefab);

        if(!wall.isError()) {
            scene.modifyComponent<TransformComponent>(wall.getValue(), 
            [this](TransformComponent& wallTransform){
                wallTransform.position = HUGO::Vector3(actX * scene.getComponentConst<MapComponent>(mapID).getGridSize(), 0, actY * scene.getComponentConst<MapComponent>(mapID).getGridSize());
            });

        }
        increasesIt();
    }
    else{
        while (tile != TileType::Wall && tile != TileType::Empty && tile != TileType::Start && !mapGenerated)
        {
            increasesIt();
            if(!mapGenerated) tile = static_cast<TileType>(map.mapData[actX][actY]);
        }
        
    }
}

void MapSystem::increasesIt(){
    actY++;
    if(actY >= scene.getComponentConst<MapComponent>(mapID).mapHeight){
        actY = 0;
        actX++;
        if(actX >= scene.getComponentConst<MapComponent>(mapID).mapWidth) {
            createBeastie(beastiePos);
            mapGenerated = true;
        }
    }
}