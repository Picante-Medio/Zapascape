#include "MovementSystem.h"

#include "MovementComponent.h"
#include "TransformComponent.h"

void MovementSystem::init() {}

void 
MovementSystem::onLoad() {}

void
MovementSystem::update(uint32_t dt) 
{
    HUGO::Float s = dt /1000.0f;
    // Update movement components
    for (auto& pack: scene.query<MovementComponent, TransformComponent>()) {
        const MovementComponent& m = pack.getConst<MovementComponent>();
        pack.modify<TransformComponent>([m, s](TransformComponent& tr){
            tr.setWorldPosition(tr.getWorldPosition() + HUGO::Vector3(m.direction.x, 0.0f, m.direction.y) * m.speed * s);
        });
    }
}

void
MovementSystem::fixedUpdate(uint32_t) {}
