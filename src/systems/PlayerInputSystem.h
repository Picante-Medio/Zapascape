#pragma once

#include "HUGO.h"

HSYSTEM(PlayerInputSystem)
    EntityID player;
    EntityID camera;

    HUGO::Vector2 lastMousePos;
    HUGO::Bool firstMouse = true;

    HUGO::Float yaw = 0.0f;
    HUGO::Float pitch = 0.0f;

    HUGO::Float rotationSensitivity = 0.002f;

    HUGO::KeyCode upKey = HUGO::KeyCode::W;
    HUGO::KeyCode downKey = HUGO::KeyCode::S;
    HUGO::KeyCode leftKey = HUGO::KeyCode::A;
    HUGO::KeyCode rightKey = HUGO::KeyCode::D;

    HUGO::KeyCode markKey = HUGO::KeyCode::F;
    HUGO::KeyCode interactKey = HUGO::KeyCode::E;

    HUGO::String markAction = "mark";
    HUGO::String interactAction = "interact";
    HUGO::String moveAction = "move";
    
public:
    void init() override;
    void onLoad() override;

    void fixedUpdate(uint32_t dt) override;
    void update(uint32_t dt) override;

END_HSYSTEM()

