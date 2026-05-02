#include "HUGO.h"
#include "TimeSystem.h"
#include "TimeComponent.h"
#include "BeastieComponent.h"
#include "MovementRBComponent.h"

const uint32_t MAX_GAME_TIME =120;
const uint32_t MAX_PLAYER_STOP_TIME = 10;

void TimeSystem::init(){
    timerEntity = scene.createEntity();
    scene.addComponent<TimeComponent>(timerEntity, [this](TimeComponent& t) {t.remainingGameTime = MAX_GAME_TIME; });
    resetTimes();
}
void TimeSystem::onLoad(){
    resetTimes();
    scene.modifyComponent<TimeComponent>(timerEntity, [this](TimeComponent& t) {t.remainingGameTime = MAX_GAME_TIME; });
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
            pack.getDeprecated<BeastieComponent>().isFastMove = true;
       }
    }
    if(playerStoppedTime > MAX_PLAYER_STOP_TIME){
       for (auto& pack: scene.query<BeastieComponent, MovementRBComponent>()){
            pack.getDeprecated<BeastieComponent>().isSlowMove = true;
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

        scene.modifyComponent<TimeComponent>(timerEntity, [this](TimeComponent& t) {
            t.remainingGameTime = gameTime>MAX_GAME_TIME? 0: MAX_GAME_TIME - gameTime; 
            t.playerStoppedLongTime = playerStoppedTime > MAX_PLAYER_STOP_TIME;
        });
        
        accumulatedMS %= 1000; 
    }
}

void TimeSystem::fixedUpdate(uint32_t) {};
