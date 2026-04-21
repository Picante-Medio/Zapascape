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
    input->registerAction(interactAction, interactKey);
    input->registerAction(markAction, markKey);

    input->registerAction(moveAction, {upKey, downKey, leftKey, rightKey});
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

void PlayerInputSystem::update(uint32_t)
{
    HUGO::Vector2 actDir(0.0f);
    auto input = scene.getInput();

    if (input->isActionPressed(moveAction))
    {
        if (input->isKeyPressed(leftKey))
            actDir.x = -1.0f;
        else if (input->isKeyPressed(rightKey))
            actDir.x = 1.0f;

        if (input->isKeyPressed(upKey))
            actDir.y = 1.0f;
        else if (input->isKeyPressed(downKey))
            actDir.y = -1.0f;
    }

    scene.modifyComponent<MovementComponent>(player,
    [actDir](MovementComponent& m)
    {
        m.direction = actDir;
    });
}
void PlayerInputSystem::fixedUpdate(uint32_t dt)
{
    const MovementComponent& m =
        scene.getComponentConst<MovementComponent>(player);

    HUGO::Vector2 dir = m.direction;

    if (dir.x == 0.0f && dir.y == 0.0f)
        return;

    // Normalizar para evitar speed boost en diagonal
    dir = dir.normalized();

    float deltaTime = dt / 1000.0f;

    // -------------------------
    // MOVIMIENTO (Z INVERTIDO → TU MOTOR)
    // -------------------------
    HUGO::Vector3 worldDir(dir.x, 0.0f, -dir.y);

    scene.modifyComponent<TransformComponent>(player,
    [worldDir, m, deltaTime](TransformComponent& t)
    {
        t.setPosition(
            t.getPosition() +
            worldDir * m.speed * deltaTime
        );
    });

    // -------------------------
    // ROTACIÓN (coherente con Z invertido)
    // -------------------------
    float angle = std::atan2(dir.x, -dir.y);

    HUGO::Quaternion rot(
        HUGO::Radian(angle),
        HUGO::Vector3(0.0f, 1.0f, 0.0f)
    );

    scene.modifyComponent<TransformComponent>(player,
    [rot](TransformComponent& t)
    {
        t.setRotation(rot);
    });
}
