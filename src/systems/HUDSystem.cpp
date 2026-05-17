#include "HUDSystem.h"
#include "AlertTag.h"
#include "PanelComponent.h"
#include "TextComponent.h"
#include "TimeComponent.h"
#include "AllChangeSceneMessages.h"
#include "RitualComponent.h"

constexpr HUGO::Float ALERT = 10.0f;
void HUDSystem::init(){
    scene.subscribe<DeactiveRitualMessage>([this](std::shared_ptr<HUGO::Message> msg) {
        std::shared_ptr<DeactiveRitualMessage> m = std::static_pointer_cast<DeactiveRitualMessage>(msg);
        for (auto& pack: scene.query<RitualTextComponent, TextComponent>())
            pack.modify<TextComponent>([m](TextComponent& text) {text.text = m->text;}); 
    });
}

void HUDSystem::onLoad(){}
void HUDSystem::update(uint32_t dt)
{
    for (auto& pack: scene.query<TimeComponent, TextComponent>())
    {
        const auto& timeComp = pack.getConst<TimeComponent>();
        HUGO::Int time = timeComp.remainingGameTime;

        pack.modify<TextComponent>([time, this](TextComponent& text)
        {
            // -------------------------
            // TEXT
            // -------------------------
            text.text = std::to_string(time);

            HUGO::Float t01 = 0.0f;

            if(time <= ALERT)
            {
                if (!alertActivated){
                    for(auto& pack: scene.query<AlertTag, PanelComponent>()){
                        pack.modify<PanelComponent>([](PanelComponent& p){
                            p.isVisible = true;
                        });
                    
                    }
                    alertActivated = true;
                }
                t01 = (ALERT - time) / ALERT;
                t01 = std::clamp(t01, 0.0f, 1.0f);

                // -------------------------
                // COLOR: red-black
                // -------------------------
                HUGO::Vector4 red(1.0f - t01, 0.0f, 0.0f, 1.0f);

                text.colourTop = red;
                text.colourBottom = red;
            }
        });
    }
}

void HUDSystem::fixedUpdate(uint32_t) {};