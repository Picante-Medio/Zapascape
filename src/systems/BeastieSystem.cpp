#include "BeastieSystem.h"
#include "BeastieComponent.h"
#include "PlayerComponent.h"
#include "TransformComponent.h"
#include "MovementRBComponent.h"
#include "AnimationComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "RenderComponent.h"
#include "AudioComponent.h"

void BeastieSystem::init(){
}
void BeastieSystem::onLoad(){
    for (auto& pack: scene.query<BeastieComponent, AnimationComponent, AudioComponent>()){
        beastie = pack.getConst<BeastieComponent>().entity;
        pack.modify<AnimationComponent>([](AnimationComponent& anim){
            anim.addAnimation("Run", true, false);
            anim.addAnimation("Stop", true, true);
        });
    }
    for (auto& pack: scene.query<PlayerComponent, TransformComponent>()){
        player = pack.getConst<PlayerComponent>().entity;
        break;
    }
}
void BeastieSystem::fixedUpdate(uint32_t){}
void BeastieSystem::update(uint32_t dt){

    auto& input = scene.getInput();

    for(auto& pack: scene.query<BeastieComponent, MovementRBComponent, TransformComponent, RigidBodyComponent, AnimationComponent, AudioComponent>()){
        TransformComponent& beastieTr = pack.getDeprecated<TransformComponent>();
        MovementRBComponent& beastieMovement = pack.getDeprecated<MovementRBComponent>();
        BeastieComponent& b = pack.getDeprecated<BeastieComponent>();

        if(!b.aiEnabled)
        {
            b.currentSpawnDelay -= dt;

            if(b.currentSpawnDelay <= 0.0f)
            {
                b.aiEnabled = true;
                pack.modify<AudioComponent>([](AudioComponent& a){
                    a.isPlaying =true;
                });

            }
            else
            {
                continue;
            }
        }
        const HUGO::Bool isMoving = input->isActionPressed("move");
        const TransformComponent& playerTr = scene.getComponentConst<TransformComponent>(player);
    
        //si esta moviendo player
        if(isMoving)
        {
            b.isSlowMove = false;
            if (isPlayerInRange(b.minRange, beastieTr, playerTr) && b.currentState != BeastieState::PURSUE) 
                b.currentState = BeastieState::PURSUE;

            
        }
        else if(b.isSlowMove){
            b.currentState = BeastieState::PURSUE;
        }
        else {
            b.currentState= BeastieState::STOP;
        }

        //fuera del rango maximo
        if (!isPlayerInRange(b.maxRange, beastieTr, playerTr) && b.currentState != BeastieState::STOP)
            b.currentState= BeastieState::STOP;
        
       
        //movemento direction
        HUGO::Vector3 toPlayer = (playerTr.getWorldPosition() - beastieTr.getWorldPosition());
        toPlayer.y = 0.0f; 
        toPlayer.normalize();
        HUGO::Vector3 dir(0.0f);

        switch (b.currentState)
        {
            case BeastieState::PURSUE:
            {
                dir = toPlayer;
                if(!b.isSlowMove){
                    pack.modify<AnimationComponent>([](AnimationComponent& a)
                    {
                        a.setEnabled("Run",true);
                        a.setEnabled("Stop",false);
                    });
                }
                break;
            }
            case BeastieState::STOP:
            {
                dir = HUGO::Vector3(0.0f);
                pack.modify<AnimationComponent>([](AnimationComponent& a)
                {
                    a.setEnabled("Run",false);
                    a.setEnabled("Stop",true);
                });
                break;
            }
        }
        beastieMovement.direction = dir;
        HUGO::Bool audioCond = dir != HUGO::Vector3(0.0f);
        updateForce(b);
        //rotation
        HUGO::Float rotDir = beastieTr.getRotation().forward().cross(toPlayer).y;
        HUGO::Float angularVel = std::clamp(rotDir, -1.0f, 1.0f) * 3.0f;
        pack.modify<RigidBodyComponent>([angularVel](RigidBodyComponent& rb)
        {
            rb.setAngularVelocity(HUGO::Vector3(0, angularVel, 0));
        });
    }

    //on collision
    for(auto collisionId : scene.getComponent<ColliderComponent>(beastie).getCollisionInfo()){
        if(scene.hasComponent<PlayerComponent>(collisionId)){
            scene.changeAndDestroyPreviousScene("DefeatScene");
        }
    }
}
HUGO::Bool BeastieSystem::isPlayerInRange(HUGO::Float dist, const TransformComponent& beastieTr, const TransformComponent& playerTr){
    float distance = (playerTr.getWorldPosition() - beastieTr.getWorldPosition()).length();
    return distance<dist;
}

void BeastieSystem::updateForce(BeastieComponent &b)
{
    scene.modifyComponent<MovementRBComponent>(beastie, [b](MovementRBComponent& m){
        if(b.isFastMove) m.force = b.fastForce;
        else if(b.isSlowMove) m.force = b.slowForce;
        else m.force = b.normalForce;
    });
}
