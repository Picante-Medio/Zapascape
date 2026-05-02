#include "MarkSystem.h"
#include "MarkComponent.h"
#include "PlayerComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"

void MarkSystem::init() 
{

}

void 
MarkSystem::onLoad() 
{
    // find player and mark controller
    for (auto& pack: scene.query<PlayerComponent>()){
        player = pack.getConst<PlayerComponent>().entity;
        break;
    }
    for (auto& pack: scene.query<MarkComponent>()){
        mark = pack.getConst<MarkComponent>().entity;
        maxMarks = pack.getConst<MarkComponent>().maxNum;
        break;
    }

    // create mark pool
    for (HUGO::Int i =0; i <maxMarks; ++i){
        EntityID id = scene.createEntity();
        
        //put them in a invisible zone
        scene.addComponent<TransformComponent>(id, [](TransformComponent& t){
            t.setPosition({0.0f, -100.0f, 0.0f});
            t.setScale(HUGO::Vector3(0.15f, 0.15f, 0.15f));
            t.setRotation(HUGO::Quaternion(HUGO::Radian(HUGO::PI), HUGO::Vector3(0,1,0)));
        });
        scene.addComponent<RenderComponent>(id, [](RenderComponent& rc) {
            rc.setMeshName("mark.glb");
        });

        //optional if we want some animation or particles 

        q.push_back(id);
    }
}

void
MarkSystem::update(uint32_t dt) 
{
    auto input = scene.getInput();
    //check if we have pressed mark key
    if (input->wasActionJustPressed("mark")){
        const TransformComponent& tr_player = scene.getComponentConst<TransformComponent>(player);
        EntityID id_to_use = q.front(); q.pop_front();
        scene.modifyComponent<TransformComponent>(id_to_use, [tr_player, this](TransformComponent& tr){
            tr.setPosition(tr_player.getPosition()-HUGO::Vector3(0.0f, 1.0f, 0.0f));
            tr.setRotation(tr_player.getRotation());
        });
        q.push_back(id_to_use);
        scene.modifyComponent<MarkComponent>(mark, [](MarkComponent& m){
            m.currentNum++;
        });
    }
}

void
MarkSystem::fixedUpdate(uint32_t) {}

