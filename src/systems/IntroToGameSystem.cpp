#include "IntroToGameSystem.h"
#include "AudioComponent.h"
#include "ButtonComponent.h"
#include "AllChangeSceneMessages.h"

void IntroToGameSystem::init(){ }

void IntroToGameSystem::onLoad(){
     scene.subscribe<ChangeGameMessage>([this](std::shared_ptr<HUGO::Message> msg) {
        std::shared_ptr<ChangeGameMessage> m = std::static_pointer_cast<ChangeGameMessage>(msg);
        scene.changeAndDestroyPreviousScene(m->sceneName);
    });

    for (auto& pack: scene.query<AudioComponent>()){
        pack.modify<AudioComponent>([this](AudioComponent& a){
            a.type = HUGO::AudioTypes::MUSIC;
        });
    }

    // register play button callback
    HUGO::ErrorOrType<EntityID> startB = scene.instantiatePrefab("startButton");
    but = startB.getValue();
    scene.modifyComponent<ButtonComponent>(but, [this](ButtonComponent& b){
        b.onClickCallback = [this](){
            scene.sendMessage<ChangeGameMessage>(ChangeGameMessage());
        };
    });
    scene.modifyComponent<AudioComponent>(but, [this](AudioComponent& a){
        a.type = HUGO::AudioTypes::SOUND2D;
    });
}

void IntroToGameSystem::update(uint32_t dt){
    for (auto& pack : scene.query<ButtonComponent, AudioComponent>()){
        const ButtonComponent& b = pack.getConst<ButtonComponent>();
        HUGO::Bool isOver = (b.state == HUGO::ButtonStateTypes::BS_OVER);

        HUGO::Bool prev = wasOver;

        if (isOver && !prev){
            pack.modify<AudioComponent>([](AudioComponent& a){
                a.isPlaying = true;
            });
        }
        wasOver = isOver;
    }
}

void IntroToGameSystem::fixedUpdate(uint32_t) {};
