#include "GameToOtherSystem.h"
#include "AudioComponent.h"
#include "AllChangeSceneMessages.h"

void GameToOtherSystem::init(){ }

void GameToOtherSystem::onLoad(){
    for (auto& pack : scene.query<AudioComponent>()){
        pack.modify<AudioComponent>([](AudioComponent& a){
            a.type = HUGO::MUSIC;
        });
    }
}

void GameToOtherSystem::update(uint32_t){
}

void GameToOtherSystem::fixedUpdate(uint32_t) {};
