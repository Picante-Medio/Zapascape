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
        HUGO::ErrorOrType<EntityID> mark = scene.instantiatePrefab("mark");
        if(!mark.isError())q.push_back(mark.getValue());
    }
}

void
MarkSystem::update(uint32_t dt) 
{
    auto& input = scene.getInput();
    //check if we have pressed mark key
    if (input.wasActionJustPressed("mark") && !q.empty()){
        const TransformComponent& tr_player = scene.getComponentConst<TransformComponent>(player);
        EntityID id_to_use = q.front(); q.pop_front();
        scene.modifyComponent<TransformComponent>(id_to_use, [tr_player, this](TransformComponent& tr){
            tr.position = tr_player.position - HUGO::Vector3(0.0f, 1.45f, 0.0f);
            tr.rotation = tr_player.rotation;
        });
        q.push_back(id_to_use);
        scene.modifyComponent<MarkComponent>(mark, [](MarkComponent& m){
            m.currentNum++;
        });
    }
}

void
MarkSystem::fixedUpdate(uint32_t) {}

