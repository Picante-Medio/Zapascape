#include "IntroToGameSystem.h"
#include "ButtonComponent.h"
#include "AllChangeSceneMessages.h"


void IntroToGameSystem::init(){ }

void IntroToGameSystem::onLoad(){
     scene.subscribe<ChangeGameMessage>([this](std::shared_ptr<HUGO::Message> msg) {
        std::shared_ptr<ChangeGameMessage> m = std::static_pointer_cast<ChangeGameMessage>(msg);
        scene.changeAndDestroyPreviousScene(m->sceneName);
    });

    // register play button callback
    for (auto& pack: scene.query<ButtonComponent>()){
        pack.modify<ButtonComponent>([this](ButtonComponent& b){
            b.onClickCallback = [this](){
                scene.sendMessage<ChangeGameMessage>(ChangeGameMessage());
            };
        });
    }
}

void IntroToGameSystem::update(uint32_t dt){
}

void IntroToGameSystem::fixedUpdate(uint32_t) {};
