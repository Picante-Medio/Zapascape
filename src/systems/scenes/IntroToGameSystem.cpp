#include "IntroToGameSystem.h"
#include "AnimationComponent.h"

void IntroToGameSystem::init(){ }

void IntroToGameSystem::onLoad(){
    HUGO::ErrorOrType<EntityID> beastie = scene.instantiatePrefab("beastieStatic");
    if(!beastie.isError()) {
        scene.addComponent<AnimationComponent>(beastie.getValue(), [](AnimationComponent& a){
            a.addAnimation("Idle", true, true);
        });
    }
    
    HUGO::ErrorOrType<EntityID> tux = scene.instantiatePrefab("tuxStatic");
    if(!tux.isError()) {
        scene.addComponent<AnimationComponent>(tux.getValue(), [](AnimationComponent& a){
            a.addAnimation("Idle", true, true);
        });
    }
}

void IntroToGameSystem::update(uint32_t dt){
}

void IntroToGameSystem::fixedUpdate(uint32_t) {};
