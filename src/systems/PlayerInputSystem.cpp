#include "PlayerInputSystem.h"

#include "CameraComponent.h"
#include "MovementComponent.h"
#include "PlayerComponent.h"
#include "TransformComponent.h"

#include <algorithm>

// -------------------------
// Pitch limits for FPS camera
// -------------------------
const HUGO::Radian MIN_PITCH = HUGO::Degree(-89.0f);
const HUGO::Radian MAX_PITCH = HUGO::Degree(89.0f);

void PlayerInputSystem::init() {
    auto input = scene.getInput();

    // Register gameplay actions
    input->registerAction(interactAction, interactKey);
    input->registerAction(markAction, markKey);

    // Movement keys (WASD or custom mapping)
    input->registerAction(moveAction, {upKey, downKey, leftKey, rightKey});
}

void PlayerInputSystem::onLoad() {

    // Find player entity
    for (auto& pack: scene.query<PlayerComponent>()){
        player = pack.getConst<PlayerComponent>().entity;
        break;
    }

    // Initialize camera angles
    yaw = 0.0f;
    pitch = 0.0f;

    firstMouse = true;
}

void PlayerInputSystem::update(uint32_t dt)
{
    auto input = scene.getInput();
    const MovementComponent& m = scene.getComponentConst<MovementComponent>(player);
    // -------------------------
    // MOVEMENT INPUT (WASD)
    // -------------------------
    HUGO::Vector3 actDir(0.0f);

    if (input->isActionPressed(moveAction))
    {
        if (input->isKeyPressed(leftKey))
            actDir.x = -1.0f;
        else if (input->isKeyPressed(rightKey))
            actDir.x = 1.0f;

        if (input->isKeyPressed(upKey))
            actDir.z = 1.0f;
        else if (input->isKeyPressed(downKey))
            actDir.z = -1.0f;
    }

    // -------------------------
    // MOUSE LOOK (only when right mouse button is held)
    // -------------------------
    
    // Check if right mouse button is currently pressed
    if (input->isMouseButtonPressed(HUGO::MouseCode::MOUSE_RIGHT))
    {
        // Get current mouse cursor position
        auto aux = input->getMousePosition();
        HUGO::Vector2 currentMousePos(
            static_cast<HUGO::Float>(aux.x),
            static_cast<HUGO::Float>(aux.y)
        );

        // Initialize last mouse position on first frame
        if (firstMouse)
        {
            lastMousePos = currentMousePos;
            firstMouse = false;
        }

        // Calculate mouse movement delta since last frame
        HUGO::Vector2 mouseDelta = currentMousePos - lastMousePos;
        lastMousePos = currentMousePos;

        // // Invert Y axis (standard FPS behavior: moving mouse up looks down)
        // mouseDelta.y = -mouseDelta.y;

        // Update camera rotation angles with sensitivity multiplier
        yaw   += mouseDelta.x * rotationSensitivity;
        pitch += mouseDelta.y * rotationSensitivity;

        // Clamp pitch to prevent camera flipping upside down
        pitch = std::clamp(pitch, MIN_PITCH.valueRadians(),MAX_PITCH.valueRadians());
    }
    else
    {
        // When right mouse button is NOT pressed:
        // Reset last mouse position to avoid camera snap/jump when pressing again
        auto aux = input->getMousePosition();
        HUGO::Vector2 currentMousePos(static_cast<HUGO::Float>(aux.x), static_cast<HUGO::Float>(aux.y));
        lastMousePos = currentMousePos;
        firstMouse = true;  // Reset flag to prevent delta accumulation
    }

    // -------------------------
    // APPLY ROTATION TO CAMERA
    // -------------------------
    // Build quaternion for yaw
    HUGO::Quaternion rotY(HUGO::Radian(yaw), HUGO::Vector3(0, 1, 0));
    
    // Build quaternion for pitch
    HUGO::Quaternion rotX(HUGO::Radian(pitch), HUGO::Vector3(1, 0, 0));
    
    // Apply combined rotation to camera (yaw first, then pitch)
    scene.modifyComponent<TransformComponent>(player, [rotY, rotX](TransformComponent& t)
    {
        // Yaw around world Y, then pitch around local X for FPS-style camera
        t.setRotation(rotY * rotX);
    });

    actDir = actDir.normalized();

    HUGO::Float deltaTime = dt / 1000.0f;

    const TransformComponent& t = scene.getComponentConst<TransformComponent>(player);
    HUGO::Vector3 forward = t.getRotation().forward();
    HUGO::Vector3 right = t.getRotation().right();

    // Keep movement on horizontal plane
    forward.y = 0.0f;
    right.y   = 0.0f;

    forward = forward.normalized();
    right = right.normalized();

    // Compute movement direction
    HUGO::Vector3 moveDir = right * actDir.x - forward * actDir.z;

    if (moveDir.length() > 0.0f)
        moveDir = moveDir.normalized();

    // we use the camera for movement
    scene.modifyComponent<MovementComponent>(player, [moveDir](MovementComponent& m)
    {
        m.direction = moveDir; 
    });
}

void PlayerInputSystem::fixedUpdate(uint32_t) {}