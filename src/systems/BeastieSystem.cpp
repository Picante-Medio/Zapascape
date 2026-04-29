#include "BeastieSystem.h"
#include "BeastieComponent.h"
#include "PlayerComponent.h"
#include "TransformComponent.h"
#include "MovementRBComponent.h"
#include "GameStatusMessage.h"
#include "AnimationComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"

const float MIN_DETECT_RANGE = 6.0f;
const float MAX_DETECT_RANGE = 10.0f;

const float NORMAL_MOVE_FORCE = 20.0f;
const float FAST_MOVE_FORCE = 30.0f;
const float SLOW_MOVE_FORCE = 1.0f; 

void BeastieSystem::init(){
    currentState= BeastieState::STOP;

    scene.subscribe<BeastieFastMove>([this](std::shared_ptr<HUGO::Message> msg) {
        scene.modifyComponent<MovementRBComponent>(beastie, [this](MovementRBComponent& m)
        {
            m.force = FAST_MOVE_FORCE;
        });
    });

    scene.subscribe<BeastieSlowMove>([this](std::shared_ptr<HUGO::Message> msg) {
        scene.modifyComponent<MovementRBComponent>(beastie, [this](MovementRBComponent& m)
        {
            m.force = SLOW_MOVE_FORCE;
        });
        currentState = BeastieState::SLOW_MOVE;
        isSlowMoving= true;
    });

    for (auto& pack: scene.query<BeastieComponent, TransformComponent, AnimationComponent>()){
        TransformComponent& beastieTr = pack.getDeprecated<TransformComponent>();
        AnimationComponent& anim = pack.getDeprecated<AnimationComponent>();
        beastie = pack.getConst<BeastieComponent>().entity;
        anim.addAnimation("Idle", true, true);
        anim.addAnimation("Run", true, false);
        anim.addAnimation("Stop", true, false);
        break;
    }

}
void BeastieSystem::onLoad(){
    currentState= BeastieState::STOP;
    isSlowMoving = false;
    for (auto& pack: scene.query<BeastieComponent, MovementRBComponent>()){
        beastie = pack.getConst<BeastieComponent>().entity;
        pack.modify<MovementRBComponent>([this](MovementRBComponent& move)
        {
            move.force = NORMAL_MOVE_FORCE;
        });
        break;
    }
    if(!scene.hasComponent<BeastieComponent>(beastie)) scene.log("NO hay enemigo");

    for (auto& pack: scene.query<PlayerComponent>()){
        player = pack.getConst<PlayerComponent>().entity;
        break;
    }
}
void BeastieSystem::fixedUpdate(uint32_t){}
void BeastieSystem::update(uint32_t dt){

    auto& input = scene.getInput();

    for(auto& pack: scene.query<BeastieComponent, MovementRBComponent, TransformComponent, RigidBodyComponent, AnimationComponent>()){
        TransformComponent& beastieTr = pack.getDeprecated<TransformComponent>();
        MovementRBComponent& beastieMovement = pack.getDeprecated<MovementRBComponent>();

        bool isMoving = input->isActionPressed("move");
        const TransformComponent& playerTr = scene.getComponentConst<TransformComponent>(player);
        
        if(isMoving)
        {
            if(isSlowMoving)
            {
                isSlowMoving = false;
                beastieMovement.force = NORMAL_MOVE_FORCE;
            }
            else if (isPlayerInRange(MIN_DETECT_RANGE, beastieTr, playerTr) && currentState != BeastieState::PURSUE) 
            {
                currentState = BeastieState::PURSUE;
            }  
        }
        else
        {
            if(!isSlowMoving) 
                currentState = BeastieState::STOP;
        } 
       
        HUGO::Vector3 toPlayer = (playerTr.getWorldPosition() - beastieTr.getWorldPosition());
        toPlayer.y = 0.0f; 
        toPlayer.normalized();
        const HUGO::Vector3 forward = beastieTr.getRotation().forward();
        HUGO::Vector3 dir(0.0f);
        float angularVel = 0.0f;

        switch (currentState)
        {
            case BeastieState::PURSUE:
            {
                dir = toPlayer;
                angularVel = forward.cross(dir).y * 3.5f;
                pack.modify<AnimationComponent>([](AnimationComponent& a)
                {
                    a.setEnabled("Run",true);
                    a.setEnabled("Idle",false);
                    a.setEnabled("Stop",false);
                });
                break;
            }
            case BeastieState:: SLOW_MOVE:
            {
                dir = toPlayer;
                pack.modify<AnimationComponent>([](AnimationComponent& a)
                {
                    a.setEnabled("Run",false);
                    a.setEnabled("Idle",true);
                    a.setEnabled("Stop",false);
                });
                break;
            }
            case BeastieState::STOP:
            {
                dir = HUGO::Vector3(0.0f);
                float rotDir = forward.cross(toPlayer).y;
                angularVel = std::clamp(rotDir, -1.0f, 1.0f) * 2.0f;
                pack.modify<AnimationComponent>([](AnimationComponent& a)
                {
                    a.setEnabled("Run",false);
                    a.setEnabled("Idle",false);
                    a.setEnabled("Stop",true);
                });
                break;
            }
        }

        beastieMovement.direction = dir.normalized();

        pack.modify<RigidBodyComponent>([angularVel](RigidBodyComponent& rb)
        {
            rb.setAngularVelocity(HUGO::Vector3(0, angularVel, 0));
        });
    }

    for(auto collisionId : scene.getComponent<ColliderComponent>(beastie).getCollisionInfo()){
        if(scene.hasComponent<PlayerComponent>(collisionId)){
            scene.log("GAME OVER");
        }
    }
}
bool BeastieSystem::isPlayerInRange(float dist, const TransformComponent& beastieTr, const TransformComponent& playerTr){
    float distance = (playerTr.getWorldPosition() - beastieTr.getWorldPosition()).length();
    return distance<dist;
}