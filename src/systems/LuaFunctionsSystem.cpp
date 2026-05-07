#include "LuaFunctionsSystem.h"
#include "LuaBinder.h"
void LuaFunctionsSystem::init(){}
void LuaFunctionsSystem::onLoad(){

    LuaBinder& luaBinder = LuaBinder::getInstance();
    luaBinder.registerMethod(*this, &LuaFunctionsSystem::changeSceneToIntro, "Intro");
}
void LuaFunctionsSystem::fixedUpdate(uint32_t){}
void LuaFunctionsSystem::update(uint32_t dt){}

void LuaFunctionsSystem::changeSceneToIntro(){
    scene.changeAndDestroyPreviousScene("IntroScene");
}