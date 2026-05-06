#include "DefeatSceneSystem.h"
#include "AnimationComponent.h"

void DefeatSceneSystem::init(){ }

void DefeatSceneSystem::onLoad(){
    HUGO::ErrorOrType<EntityID> beastie = scene.instantiatePrefab("beastieZapas");
    if(!beastie.isError()) {
        scene.addComponent<AnimationComponent>(beastie.getValue(), [](AnimationComponent& a){
            a.addAnimation("Run", true, true);
        });
    }
}

void DefeatSceneSystem::update(uint32_t dt){
}

void DefeatSceneSystem::fixedUpdate(uint32_t) {};
