#include "InstantiateButtonSystem.h"
#include "RenderComponent.h"
#include "AudioComponent.h"
#include "ButtonComponent.h"
#include "AllChangeSceneMessages.h"
void InstantiateButtonSystem::init() {}

void 
InstantiateButtonSystem::onLoad() 
{
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
    EntityID but = startB.getValue();
    scene.modifyComponent<ButtonComponent>(but, [this](ButtonComponent& b){
        b.onClickCallback = [this](){
            //scene.sendMessage<ChangeGameMessage>(ChangeGameMessage());
            scene.changeAndDestroyPreviousScene("Game");
        };
    });
    scene.modifyComponent<AudioComponent>(but, [this](AudioComponent& a){
        a.type = HUGO::AudioTypes::SOUND2D;
    });
    
    // register exit button callback
    HUGO::ErrorOrType<EntityID> exitB = scene.instantiatePrefab("exitButton");
    but = exitB.getValue();
    scene.modifyComponent<ButtonComponent>(but, [this](ButtonComponent& b){
        b.onClickCallback = [this](){
            scene.exitGame();
        };
    });
    scene.modifyComponent<AudioComponent>(but, [this](AudioComponent& a){
        a.type = HUGO::AudioTypes::SOUND2D;
    });
}

void
InstantiateButtonSystem::update(uint32_t dt) {
for (auto& pack : scene.query<ButtonComponent, AudioComponent>()){
        const ButtonComponent& b = pack.getConst<ButtonComponent>();
        HUGO::Bool isOver = (b.state == HUGO::ButtonStateTypes::BS_OVER);

        if (isOver && !b.wasOver){
            pack.modify<AudioComponent>([](AudioComponent& a){
                a.isPlaying = true;
                a.startTime =0.1;
            });
        }
        if (isOver != b.wasOver){
            pack.modify<ButtonComponent>([isOver](ButtonComponent& but){
                but.wasOver = isOver;
            });
        }
    }
}

void
InstantiateButtonSystem::fixedUpdate(uint32_t) {}
