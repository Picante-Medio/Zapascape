#include "RitualSystem.h"
#include <iostream>

#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "AnimationComponent.h"
#include "AudioComponent.h"
#include "PlayerComponent.h"
#include "RenderComponent.h"
#include "RitualComponent.h"
#include "ChildrenComponent.h"
#include "ParticleEmitterComponent.h"
#include "DoorComponent.h"
#include "DoorTag.h"

void
RitualSystem::init() {
    activeRituals = 0;
    player = -1;
    maxRitualsNumber = 0;
}

void
RitualSystem::onLoad() {
}

void
RitualSystem::update(uint32_t dt) {
    if(player == -1) {
        for (auto& pack: scene.query<PlayerComponent>()){
            player = pack.getConst<PlayerComponent>().entity;
            break;
        }
    }

    for (auto& pack: scene.query<RitualComponent, ColliderComponent, RenderComponent>()){
        if(maxRitualsNumber == 0) {
            maxRitualsNumber = scene.query<RitualComponent>().size();
        }
        EntityID ritual = pack.getConst<RitualComponent>().entity;
        
        if(!scene.getComponent<RitualComponent>(ritual).getActive()) {
            continue;
        }

        EntityID cubo = pack.getConst<RitualComponent>().associatedCubo;
        bool playerInArea = false;
        for(auto collisionId : scene.getComponent<ColliderComponent>(ritual).getCollisionInfo()){
            if(player == collisionId){
                playerInArea = true;
                // activate cube
                scene.modifyComponent<RenderComponent>(cubo, [](RenderComponent& rc){ 
                    rc.visible = true;
                });

                auto input = scene.getInput();
                if(input->isActionPressed("interact") && scene.getComponent<RitualComponent>(ritual).getActive()){
                    activateRitual(ritual, cubo);
                    scene.log("Ritual activated");
                    break;
                }
            }
        }

        if(!playerInArea) {
            scene.modifyComponent<RenderComponent>(cubo, [](RenderComponent& rc){ 
                rc.visible = false;
            });
        }
    }

    for (auto& pack : scene.query<DoorComponent>()){
        EntityID door = pack.getConst<DoorComponent>().entity;

        if(pack.getConst<DoorComponent>().isInteractive) {
            auto input = scene.getInput();
            EntityID cubo = pack.getConst<DoorComponent>().associatedCubo;

            bool playerInArea = false;
            for(auto collisionId : scene.getComponent<ColliderComponent>(door).getCollisionInfo()){
                if(collisionId == player){
                    playerInArea = true;
                    if(input->isActionPressed("interact") ){ 
                        scene.log("Door activated");
                        scene.changeAndDestroyPreviousScene("VictoryScene");
                        break;
                    }
                }
            }

            scene.modifyComponent<RenderComponent>(cubo, [playerInArea](RenderComponent& rc){ 
                rc.visible = playerInArea;
            });
            
        }
    }
}

void 
RitualSystem::activateRitual(EntityID ritual, EntityID cubo) {  
    setActiveRitualsNumber(activeRituals + 1);
    
    scene.modifyComponent<RitualComponent>(ritual, [](RitualComponent& rt){
        rt.setActive(false);
    });
    
    scene.modifyComponent<RenderComponent>(ritual, [](RenderComponent& rc){ 
        rc.visible = false;
    });
    
    scene.modifyComponent<RenderComponent>(cubo, [](RenderComponent& rc){ 
        rc.visible = false;
    });
    
    scene.modifyComponent<ColliderComponent>(ritual, [](ColliderComponent& cc){
        cc.setActive(false);
    });

    scene.modifyComponent<ParticleEmitterComponent>(ritual, [](ParticleEmitterComponent& pe){
        pe.active = false;
        pe.emitting = false;
    });
    
    scene.log("Ritual activado. Progreso: " + std::to_string(activeRituals) + "/" + std::to_string(maxRitualsNumber));
} 

void
RitualSystem::fixedUpdate(uint32_t) {
}  

void 
RitualSystem::setMaximunRitualsNumber(HUGO::Int newNumber){
    maxRitualsNumber = newNumber;
} 

void 
RitualSystem::setActiveRitualsNumber(HUGO::Int newNumber){
    activeRituals = newNumber;

    if(activeRituals == maxRitualsNumber){
        // door activated
        //de momento va directo a victoria para tener un flujo
        for (auto& pack : scene.query<DoorComponent>()){
            pack.getDeprecated<DoorComponent>().isInteractive = true;
        }
    }
}