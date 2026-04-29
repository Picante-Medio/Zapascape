#include "HUGO.h"
#include "TimeSystem.h"
#include "GameStatusMessage.h"
#include "TimeComponent.h"
const uint32_t MAX_GAME_TIME =10;
const uint32_t MAX_PLAYER_STOP_TIME = 5;

void TimeSystem::init(){
    timerEntity = scene.createEntity();
    scene.addComponent<TimeComponent>(timerEntity, [this](TimeComponent& t) {t.remainingGameTime = MAX_GAME_TIME; });
    
    resetTimes();
}
void TimeSystem::onLoad(){
    resetTimes();
    scene.modifyComponent<TimeComponent>(timerEntity, [this](TimeComponent& t) {t.remainingGameTime = MAX_GAME_TIME; });
    menssageSent = false;
}
void TimeSystem::resetTimes()
{
    accumulatedMS=0;
    playerStoppedTime = 0;
    gameTime = 0;
}
void TimeSystem::update(uint32_t dt){
    accumulatedMS += dt;

    scene.log(std::to_string(playerStoppedTime));
    if(gameTime > MAX_GAME_TIME && ! menssageSent){
        scene.sendMessage<BeastieFastMove>(BeastieFastMove{});
        menssageSent= true;
    }
    if(playerStoppedTime > MAX_PLAYER_STOP_TIME){
        scene.sendMessage<BeastieSlowMove>(BeastieSlowMove{});
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
        });
        
        accumulatedMS %= 1000; 
    }
}

void TimeSystem::fixedUpdate(uint32_t) {};
