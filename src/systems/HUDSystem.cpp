#include "HUDSystem.h"
#include "PanelComponent.h"
#include "TextComponent.h"
#include "TimeComponent.h"

void HUDSystem::init(){}

void HUDSystem::onLoad(){}
void HUDSystem::update(uint32_t dt){
    for (auto& pack: scene.query<TimeComponent, TextComponent>()){
        const HUGO::Int time = pack.getConst<TimeComponent>().remainingGameTime;
        pack.modify<TextComponent>([time](TextComponent& t){
            t.text = std::to_string(time);
        });
    }
}

void HUDSystem::fixedUpdate(uint32_t) {};