#include "VictorySceneSystem.h"
#include "AnimationComponent.h"

void VictorySceneSystem::init(){ }

void VictorySceneSystem::onLoad(){
    EntityID beastie = scene.instantiatePrefab("tuxZapas").getValue();
    scene.addComponent<AnimationComponent>(beastie, [](AnimationComponent& a){
        a.addAnimation("Run", true, true);
    });
}

void VictorySceneSystem::update(uint32_t dt){
}

void VictorySceneSystem::fixedUpdate(uint32_t) {};
