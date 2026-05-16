#include "RitualSystem.h"
#include <iostream>

#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "AnimationComponent.h"
#include "AudioComponent.h"
#include "PlayerComponent.h"
#include "RenderComponent.h"
#include "RitualComponent.h"
#include "ParticleEmitterComponent.h"
#include "DoorComponent.h"

void
RitualSystem::init() {
    activeRituals = 0;
    player = -1;
    maxRitualsNumber = 0;
}

void
RitualSystem::onLoad() {
    sound = scene.createEntity();
    scene.addComponent<AudioComponent>(sound, [](AudioComponent& a){
        a.audioName = "deactivate_ritual";
        a.type = HUGO::SOUND2D;
        a.isPlaying = false;
        a.loop=false;
        a.volume = 0.8f;
    });
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
        
        if(!scene.getComponentConst<RitualComponent>(ritual).isActive) {
            continue;
        }

        EntityID cubo = pack.getConst<RitualComponent>().associatedCubo;
        bool playerInArea = false;
        for(auto collisionId : scene.getComponentConst<ColliderComponent>(ritual).getCollisionInfo()){
            if(player == collisionId){
                playerInArea = true;
                // activate cube
                scene.modifyComponent<RenderComponent>(cubo, [](RenderComponent& rc){ 
                    rc.isVisible = true;
                });

                auto input = scene.getInput();
                if(input->isActionPressed("interact") && scene.getComponentConst<RitualComponent>(ritual).isActive){
                    activateRitual(ritual, cubo);
                    scene.log("Ritual activated");
                    break;
                }
            }
        }

        if(!playerInArea) {
            scene.modifyComponent<RenderComponent>(cubo, [](RenderComponent& rc){ 
                rc.isVisible = false;
            });
        }
    }

    for (auto& pack : scene.query<DoorComponent>()){
        EntityID door = pack.getConst<DoorComponent>().entity;

        if(pack.getConst<DoorComponent>().isInteractive) {
            auto input = scene.getInput();
            EntityID cubo = pack.getConst<DoorComponent>().associatedCubo;

            bool playerInArea = false;
            for(auto collisionId : scene.getComponentConst<ColliderComponent>(door).getCollisionInfo()){
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
                rc.isVisible = playerInArea;
            });
            
        }
    }
}

void 
RitualSystem::activateRitual(EntityID ritual, EntityID cubo) {  
    setActiveRitualsNumber(activeRituals + 1);
    
    scene.modifyComponent<RitualComponent>(ritual, [ritual, this](RitualComponent& rt){
        rt.isActive = false;
        scene.modifyComponent<AudioComponent>(ritual, [](AudioComponent& a){
            a.isPlaying = false;
        });
    });
    
    scene.modifyComponent<RenderComponent>(ritual, [](RenderComponent& rc){ 
        rc.isVisible = false;
    });
    
    scene.modifyComponent<RenderComponent>(cubo, [](RenderComponent& rc){ 
        rc.isVisible = false;
    });
    
    scene.modifyComponent<ColliderComponent>(ritual, [](ColliderComponent& cc){
        cc.setActive(false);
    });

    scene.modifyComponent<ParticleEmitterComponent>(ritual, [](ParticleEmitterComponent& pe){
        pe.active = false;
        pe.emitting = false;
    });

    scene.modifyComponent<AudioComponent>(sound, [](AudioComponent& a){
        a.isPlaying = true;
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
        for (auto& pack : scene.query<DoorComponent, AudioComponent>()){

            pack.modify<DoorComponent>([](DoorComponent& door){
                door.isInteractive = true;
            });

            pack.modify<AudioComponent>([](AudioComponent& a){
                a.isPlaying = true;
            });
            
        }
    }
}