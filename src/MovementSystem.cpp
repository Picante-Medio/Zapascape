#include "MovementSystem.h"

#include "MovementComponent.h"
#include "TransformComponent.h"

void MovementSystem::init() {}

void 
MovementSystem::onLoad() {}


void
MovementSystem::update(uint32_t dt) 
{
    // Update movement components
//     for (auto& pack: scene.query<MovementComponent>()) {
//         const MovementComponent& m = pack.getConst<MovementComponent>();
//         const TransformComponent& t = pack.getConst<TransformComponent>();
//         scene.modify<TransformComponent>(pack.entity, [m, dt](TransformComponent& tr){
//             tr.setPosition(tr.getPosition() + HUGO::Vector3(m.direction.x, 0.0f, m.direction.y) * m.speed * dt);
//         });
//     }
}

void
MovementSystem::fixedUpdate(uint32_t) {}
