#include "HUDSystem.h"
#include "PanelComponent.h"
#include "TextComponent.h"
#include "TimeComponent.h"

void HUDSystem::init(){}

void HUDSystem::onLoad(){
    for (auto& pack: scene.query<TimeComponent>()){
        gameTimer = pack.getConst<TimeComponent>().entity;
        break;
    }
}
void HUDSystem::update(uint32_t dt){
    const HUGO::Int time = scene.getComponent<TimeComponent>(gameTimer).remainingGameTime;
    scene.modifyComponent<TextComponent>(gameTimer, [time](TextComponent& t){t.text = std::to_string(time); });
}

void HUDSystem::fixedUpdate(uint32_t) {};