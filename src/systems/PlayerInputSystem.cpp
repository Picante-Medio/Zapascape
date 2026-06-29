#include "PlayerInputSystem.h"

#include "ColliderComponent.h"
#include "CameraComponent.h"
#include "MovementRBComponent.h"
#include "PlayerComponent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "AnimationComponent.h"
#include "LightComponent.h"
#include "AudioComponent.h"
#include <algorithm>

// -------------------------
// Pitch limits for FPS camera
// -------------------------
const HUGO::Radian MIN_PITCH = HUGO::Degree(-89.0f);
const HUGO::Radian MAX_PITCH = HUGO::Degree(89.0f);

void PlayerInputSystem::init() {
    auto& input = scene.getInput();

    // Register gameplay actions
    input.registerAction(interactAction, interactKey);
    input.registerAction(markAction, markKey);

    // Movement keys (WASD or custom mapping)
    input.registerAction(moveAction, {upKey, downKey, leftKey, rightKey});
}

void PlayerInputSystem::onLoad() {
    for(auto& pack: scene.query<PlayerComponent, AnimationComponent>()){
        pack.modify<AnimationComponent>([](AnimationComponent& anim){
            anim.addAnimation("Idle", true, false);
            anim.addAnimation("Stop", true, true);
        });
    }
    firstMouse = true;
}

void PlayerInputSystem::update(uint32_t dt)
{
    auto& input = scene.getInput();

    // -------------------------
    // MOVEMENT INPUT (WASD)
    // -------------------------
    HUGO::Vector3 actDir(0.0f);

    if(input.isKeyPressed(HUGO::KeyCode::ESC)) scene.changeAndDestroyPreviousScene("IntroScene");
    if (input.isKeyPressed(leftKey)) actDir.x -= 1.0f;
    else if (input.isKeyPressed(rightKey)) actDir.x += 1.0f;

    if (input.isKeyPressed(upKey)) actDir.z -= 1.0f;
    else if (input.isKeyPressed(downKey)) actDir.z += 1.0f;

    //ANIMATION
    for(auto& pack: scene.query<PlayerComponent,AnimationComponent, AudioComponent>()){
        bool cond = input.isActionPressed("move");
        pack.modify<AnimationComponent>([cond](AnimationComponent& anim){
            anim.setEnabled("Idle",cond);
            anim.setEnabled("Stop",!cond);
        });
        pack.modify<AudioComponent>([cond](AudioComponent& a){
            a.isPlaying = cond;
        });
    }

    // -------------------------
    // MOVEMENT DIRECTION
    // -------------------------
    for (auto& pack: scene.query<PlayerComponent,TransformComponent, MovementRBComponent>()){
        const TransformComponent& t = pack.getConst<TransformComponent>();
        HUGO::Vector3 forward = t.rotation.forward().normalized();
        HUGO::Vector3 right = t.rotation.right().normalized();

        forward.y = 0.0f;
        right.y = 0.0f;

        HUGO::Vector3 moveDir = right * actDir.x + forward * actDir.z;

        if (moveDir.length() > 0.001f)
            moveDir = moveDir.normalized();
        else
            moveDir = HUGO::Vector3(0,0,0);

        pack.modify<MovementRBComponent>([moveDir](MovementRBComponent& m)
        {
            m.direction = moveDir;
        });
    }
}

// -------------------------
// MOUSE DELTA
// -------------------------
void PlayerInputSystem::fixedUpdate(uint32_t) {
    auto& input = scene.getInput();
    HUGO::Vector2 mouseDelta(0.0f);
    auto aux = input.getMousePosition();
    if (input.isMouseButtonPressed(HUGO::MouseCode::MOUSE_RIGHT))
    {
        HUGO::Vector2 currentMousePos(static_cast<float>(aux.x),static_cast<float>(aux.y));

        if (firstMouse)
        {
            lastMousePos = currentMousePos;
            firstMouse = false;
        }

        mouseDelta = currentMousePos - lastMousePos;
        lastMousePos = currentMousePos;
    }
    else
    {
        lastMousePos = HUGO::Vector2(static_cast<float>(aux.x),static_cast<float>(aux.y));
        firstMouse = true;
    }

    // -------------------------
    // RIGIDBODY ROTATION 
    // -------------------------
    float yaw = mouseDelta.x * rotationSensitivity;
    for (auto& pack: scene.query<PlayerComponent, RigidBodyComponent>()){
        pack.modify<RigidBodyComponent>([yaw](RigidBodyComponent& rb){
            rb.setAngularVelocity(HUGO::Vector3(0.0f, yaw, 0.0f));
        });
    }
}