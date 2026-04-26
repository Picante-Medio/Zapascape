#include "HUDSystem.h"
#include "PanelComponent.h"
#include "TextComponent.h"
#include "TimeComponent.h"

void HUDSystem::init(){
    gameTimerText = scene.createEntity();
    
    HUGO::String aux = "Menu/Mierdas25";
    scene.addComponent<PanelComponent>(gameTimerText, [&aux](PanelComponent& panel){
        panel.overlayName = aux;
        panel.containerName = panel.overlayName + "/Container";
        aux = panel.containerName;
    });
    scene.addComponent<TextComponent>(gameTimerText, [aux](TextComponent& text){
        text.textAreaName = aux +"/Text";
        text.left = 0.35f;
        text.top = 0.06f;
        text.colourTop = HUGO::Vector4(0.0f, 0.5f, 0.63f, 1.0f);
        text.colourBottom = HUGO::Vector4(0.0f, 1.0f, 1.0f, 1.0f);
        text.characterHeight = 0.035f;
        text.spaceWidth = 0.01f;
    });

}
void HUDSystem::onLoad(){
    for (auto& pack: scene.query<TimeComponent>()){
        gameTimer = pack.getConst<TimeComponent>().entity;
        break;
    }
}
void HUDSystem::update(uint32_t dt){
    const uint32_t time = scene.getComponent<TimeComponent>(gameTimer).remainingGameTime;
    scene.modifyComponent<TextComponent>(gameTimerText, [time](TextComponent& t){t.text = std::to_string(time); });
}

void HUDSystem::fixedUpdate(uint32_t) {};