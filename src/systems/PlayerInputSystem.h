#pragma once

#include "HUGO.h"

HSYSTEM(PlayerInputSystem)
    HUGO::Vector2 lastMousePos;
    bool firstMouse = true;

    float rotationSensitivity = 0.1f;

    HUGO::KeyCode upKey = HUGO::KeyCode::W;
    HUGO::KeyCode downKey = HUGO::KeyCode::S;
    HUGO::KeyCode leftKey = HUGO::KeyCode::A;
    HUGO::KeyCode rightKey = HUGO::KeyCode::D;

    HUGO::KeyCode markKey = HUGO::KeyCode::F;
    HUGO::KeyCode interactKey = HUGO::KeyCode::E;

    std::string markAction = "mark";
    std::string interactAction = "interact";
    std::string moveAction = "move";
    
    EntityID cameraID;
    float pitch = 0.0f;
public:
    void init() override;
    void onLoad() override;

    void fixedUpdate(uint32_t dt) override;
    void update(uint32_t dt) override;

END_HSYSTEM()

