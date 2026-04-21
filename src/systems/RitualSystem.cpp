#include "RitualSystem.h"

#include <iostream>

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "TweenComponent.h"
#include "ParentComponent.h"
#include "ChildrenComponent.h"
#include "PanelComponent.h"
#include "ButtonComponent.h"
#include "TextComponent.h"
#include "InputFieldComponent.h"
#include "LightComponent.h"
#include "SliderComponent.h"
#include "ParticleEmitterComponent.h"
#include "CameraComponent.h"

#include "AnimationComponent.h"
#include "AudioComponent.h"

void
RitualSystem::init() {
    activeRituals = 0;
    maxRitualsNumber = 3;

    // create rituals in valid places 
    for(int i = 0; i < maxRitualsNumber; ++i){
        EntityID ritual = scene.createEntity();
        // instantiate prefab

        // change its position
    }
}

void
RitualSystem::onLoad() {}

void
RitualSystem::update(uint32_t dt) {
   
}

void
RitualSystem::fixedUpdate(uint32_t) {}


void 
RitualSystem::setMaximunRitualsNumber(HUGO::Int newNumber){
    maxRitualsNumber = newNumber;
}

void 
RitualSystem::setActiveRitualsNumber(HUGO::Int newNumber){
    activeRituals = newNumber;

    if(activeRituals == maxRitualsNumber){
    // door activated
    }
}

HUGO::Int 
RitualSystem::getMaximunRitualsNumber() const {
    return maxRitualsNumber;
}

HUGO::Int 
RitualSystem::getActiveRitualsNumber() const{
   return activeRituals;
}