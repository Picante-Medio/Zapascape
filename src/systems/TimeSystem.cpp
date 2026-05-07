#include "HUGO.h"
#include "TimeSystem.h"
#include "TimeComponent.h"
#include "PanelComponent.h"
#include "TextComponent.h"
#include "BeastieComponent.h"
#include "MovementRBComponent.h"

void TimeSystem::init(){
    resetTimes();
}
void TimeSystem::onLoad(){
    resetTimes();
    for (auto& pack: scene.query<TimeComponent>()){
        pack.modify<TimeComponent>([this](TimeComponent& t){
            MAX_GAME_TIME = t.maxGameTime;
            MAX_PLAYER_STOP_TIME = t.maxStopTime;
            t.remainingGameTime = t.maxGameTime;
        });
    }
}
void TimeSystem::resetTimes()
{
    accumulatedMS=0;
    playerStoppedTime = 0;
    gameTime = 0;
}
void TimeSystem::update(uint32_t dt){
    accumulatedMS += dt;

    if(gameTime > MAX_GAME_TIME){
       for (auto& pack: scene.query<BeastieComponent, MovementRBComponent>()){
            pack.modify<BeastieComponent>([](BeastieComponent& b){
                b.isFastMove = true;
            });
       }
    }
    if(playerStoppedTime > MAX_PLAYER_STOP_TIME){
       for (auto& pack: scene.query<BeastieComponent, MovementRBComponent>()){
            pack.modify<BeastieComponent>([](BeastieComponent& b){b.isSlowMove = true;});
       }
    }

    if(accumulatedMS >= 1000) {
        if(!scene.getInput()->isActionPressed("move")){
            playerStoppedTime+= (accumulatedMS / 1000);
        }
        else {
            playerStoppedTime = 0;
        }
        gameTime += (accumulatedMS / 1000);

        for (auto& pack: scene.query<TimeComponent>()){
            pack.modify<TimeComponent>([this](TimeComponent& t){
                t.remainingGameTime = gameTime>MAX_GAME_TIME? 0: MAX_GAME_TIME - gameTime; 
                t.playerStoppedLongTime = playerStoppedTime > MAX_PLAYER_STOP_TIME;
            });
        }
        
        accumulatedMS %= 1000; 
    }
}

void TimeSystem::fixedUpdate(uint32_t) {};
