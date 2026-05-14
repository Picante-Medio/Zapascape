#include "MovementSystem.h"

#include "MovementComponent.h"
#include "MovementRBComponent.h"
#include "WorldTransformComponent.h"
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
    for (auto& pack: scene.query<MovementComponent, WorldTransformComponent>()) {
        const MovementComponent& m = pack.getConst<MovementComponent>();
        pack.modify<WorldTransformComponent>([s, m](WorldTransformComponent& tr){
            tr.worldPosition += m.direction * m.speed * s;
        });
    }
}

void 
MovementSystem::fixedUpdate(uint32_t) {
    // movement with rigidbody
    for (auto& pack: scene.query<MovementRBComponent, RigidBodyComponent>()) {
        const MovementRBComponent& m = pack.getConst<MovementRBComponent>();
        pack.modify<RigidBodyComponent>([m](RigidBodyComponent& rb){
            rb.setAsleep(false);
            rb.applyForce(m.direction.normalized() * m.force);
            
            //this is a transform-like movement
            //HUGO::Vector3 targetVelocity = m.direction.normalized() * m.speed;
            //rb.setLinearVelocity(targetVelocity);   
        });
    }
}
