#include "MovementSystem.h"

#include "MovementComponent.h"
#include "MovementRBComponent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"

void MovementSystem::init() {}

void 
MovementSystem::onLoad() {}

void
MovementSystem::update(uint32_t dt) 
{
    // movement with transform
    HUGO::Float s = dt /1000.0f;
    // Update movement components
    for (auto& pack: scene.query<MovementComponent, TransformComponent>()) {
        const MovementComponent& m = pack.getConst<MovementComponent>();
        scene.modifyComponent<TransformComponent>(m.entity, [s, m](TransformComponent& tr){
            tr.setWorldPosition(tr.getWorldPosition() + m.direction * m.speed * s);
        });
    }
}

void 
MovementSystem::fixedUpdate(uint32_t) {
    // movement with rigidbody
    for (auto& pack: scene.query<MovementRBComponent, RigidBodyComponent>()) {
        const MovementRBComponent& m = pack.getConst<MovementRBComponent>();
        scene.modifyComponent<RigidBodyComponent>(m.entity, [m](RigidBodyComponent& rb){
            rb.setAsleep(false);
            rb.applyForce(m.direction.normalized() * m.force);
            
            //this is a transform-like movement
            //HUGO::Vector3 targetVelocity = m.direction.normalized() * m.speed;
            //rb.setLinearVelocity(targetVelocity);   
        });
    }
}
