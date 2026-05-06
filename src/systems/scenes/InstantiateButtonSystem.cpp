#include "InstantiateButtonSystem.h"
#include "RenderComponent.h"
#include "AudioComponent.h"
#include "ButtonComponent.h"
#include "AllChangeSceneMessages.h"
#include "ClickTag.h"
#include "PanelComponent.h"
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
    for (auto& pack: scene.query<PanelComponent>()){
        loading = pack.getConst<PanelComponent>().entity;
    }

    // register play button callback
    HUGO::ErrorOrType<EntityID> startB = scene.instantiatePrefab("startButton");
    EntityID but = startB.getValue();
    const AudioComponent& a = scene.getComponentConst<AudioComponent>(but);

    scene.modifyComponent<ButtonComponent>(but, [this](ButtonComponent& b){
        b.onClickCallback = [this](){
            countdownStart = true;
            scene.modifyComponent<AudioComponent>(click, [](AudioComponent& a){ a.isPlaying = true; });
            scene.modifyComponent<PanelComponent>(loading, [](PanelComponent& p){ p.isVisible = true; });
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

    for (auto& pack: scene.query<AudioComponent, ClickTag>()){
        pack.modify<AudioComponent>([this](AudioComponent& a){
            a.type = HUGO::AudioTypes::SOUND2D;
            click = a.entity;
        });
    }
}

void
InstantiateButtonSystem::update(uint32_t dt) {
for (auto& pack : scene.query<ButtonComponent, AudioComponent>()){
        const ButtonComponent& b = pack.getConst<ButtonComponent>();
        HUGO::Bool isOver = (b.state == HUGO::ButtonStateTypes::BS_OVER);

        if (!countdownStart && isOver && !b.wasOver){
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
    if (countdownStart){
        cont += dt;
        if (cont >= contMax){
            scene.changeAndDestroyPreviousScene("Game");
        }
    }
}

void
InstantiateButtonSystem::fixedUpdate(uint32_t) {}
