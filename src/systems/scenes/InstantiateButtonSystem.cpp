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

    for (auto& pack: scene.query<PanelComponent>()){
        loading = pack.getConst<PanelComponent>().entity;
        break;
    }

    // register play button callback
    HUGO::ErrorOrType<EntityID> startB = scene.instantiatePrefab("startButton");
    EntityID but = startB.getValue();
    const AudioComponent& a = scene.getComponentConst<AudioComponent>(but);

    scene.modifyComponent<ButtonComponent>(but, [this](ButtonComponent& b){
        b.onClickCallback = [this](){
            countdownStart = true;
            scene.modifyComponent<AudioComponent>(click, [](AudioComponent& a){ a.isPlaying = true; });
            scene.modifyComponent<PanelComponent>(loading, [](PanelComponent& p){ 
            p.isVisible = true; });
        };
    });

    // register exit button callback
    HUGO::ErrorOrType<EntityID> exitB = scene.instantiatePrefab("exitButton");
    but = exitB.getValue();
    scene.modifyComponent<ButtonComponent>(but, [this](ButtonComponent& b){
        b.onClickCallback = [this](){
            scene.exitGame();
        };
    });

    for (auto& pack: scene.query<AudioComponent, ClickTag>()){
        click = pack.getConst<AudioComponent>().entity;
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
