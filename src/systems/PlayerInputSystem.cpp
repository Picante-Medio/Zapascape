#include "PlayerInputSystem.h"

#include "CameraComponent.h"
#include "MovementComponent.h"
#include "PlayerComponent.h"
#include "TransformComponent.h"

#include <algorithm>

const HUGO::Radian MIN_PITCH = HUGO::Degree(-89.0f);
const HUGO::Radian MAX_PITCH = HUGO::Degree(89.0f);

void 
PlayerInputSystem::init() {
    auto input = scene.getInput();
    input->registerAction("interact", interactKey);
    input->registerAction("mark", markKey);
}

void
PlayerInputSystem::onLoad() {

    // find player entity id every time we load (bcs the previous game scene might be erased)
    for (auto& pack: scene.query<PlayerComponent>()){
        player = pack.getConst<PlayerComponent>().entity;
        break;
    }

    for (auto& pack: scene.query<CameraComponent>()){
        const CameraComponent& c = pack.getConst<CameraComponent>();
        if (c.isMain()){
            camera = c.entity;
            break;
        }
    }
}


void
PlayerInputSystem::update(uint32_t) 
{
    // direction for the next movement
    HUGO::Vector2 actDir = HUGO::Vector2(0.0f);
    auto input = scene.getInput();

    #pragma region horizontal axis
    if (input->isKeyPressed(leftKey)){
        actDir.x = -1.0f;
    }
    else if (input->isKeyPressed(rightKey)){
        actDir.x = 1.0f;
    }
    #pragma endregion

    #pragma region vertical axis
    if (input->isKeyPressed(downKey)){
        actDir.y = -1.0f;
    }
    else if (input->isKeyPressed(upKey)){
        actDir.y = 1.0f;
    }
    #pragma endregion

    // apply movement
    if (actDir != HUGO::Vector2(0.0f)){
        scene.modifyComponent<MovementComponent>(player, [actDir](MovementComponent& m){
            m.direction = actDir;
        });
    }
}

void
PlayerInputSystem::fixedUpdate(uint32_t) {}
