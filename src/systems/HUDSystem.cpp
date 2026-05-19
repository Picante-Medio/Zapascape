#include "HUDSystem.h"
#include "Tags.h"
#include "PanelComponent.h"
#include "TextComponent.h"
#include "TimeComponent.h"
#include "AllChangeSceneMessages.h"
#include "RitualComponent.h"

void HUDSystem::init(){
    scene.subscribe<DeactiveRitualMessage>([this](std::shared_ptr<HUGO::Message> msg) {
        std::shared_ptr<DeactiveRitualMessage> m = std::static_pointer_cast<DeactiveRitualMessage>(msg);
        for (auto& pack: scene.query<RitualTextTag, TextComponent>())
            pack.modify<TextComponent>([m](TextComponent& text) {text.text = m->text;}); 
    });
}

void HUDSystem::onLoad(){}
void HUDSystem::update(uint32_t dt)
{
    for (auto& pack: scene.query<TimeComponent, TextComponent>())
    {
        const TimeComponent& timeComp = pack.getConst<TimeComponent>();

        pack.modify<TextComponent>([timeComp, this](TextComponent& text)
        {
            // -------------------------
            // TEXT
            // -------------------------
            HUGO::Int time = timeComp.remainingGameTime;
            text.text = std::to_string(time);

            HUGO::Float t01 = 0.0f;

            if(timeComp.alert)
            {
                if (!alertActivated){
                    for(auto& pack: scene.query<AlertTag, PanelComponent>()){
                        pack.modify<PanelComponent>([](PanelComponent& p){
                            p.isVisible = true;
                        });
                    
                    }
                    alertActivated = true;
                }
                HUGO::Vector4 red(1.0f, 0.0f, 0.0f, 1.0f);
                text.colourTop = red;
                text.colourBottom = red;
            }
        });
    }
}

void HUDSystem::fixedUpdate(uint32_t) {};