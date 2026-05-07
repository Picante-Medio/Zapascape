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
#include "MapComponent.h"

void BeastieSystem::init(){
}
void BeastieSystem::onLoad(){
    for (auto& pack: scene.query<BeastieComponent, AnimationComponent, AudioComponent>()){
        beastie = pack.getConst<BeastieComponent>().entity;
        pack.modify<BeastieComponent>([this](BeastieComponent& b){
            beastie = b.entity;
            b.aiEnabled = false;
            b.currentSpawnDelay = 10.0f;
        });
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
void BeastieSystem::fixedUpdate(uint32_t)
{
    for(auto& pack : scene.query<BeastieComponent, MovementRBComponent, TransformComponent, RigidBodyComponent>())
    {
        TransformComponent& beastieTr = pack.getDeprecated<TransformComponent>();
        MovementRBComponent& movement  = pack.getDeprecated<MovementRBComponent>();
        BeastieComponent& b = pack.getDeprecated<BeastieComponent>();

        //----------------------------------------
        // MOVEMENT
        //----------------------------------------
        movement.direction = b.desiredDirection;

        //----------------------------------------
        // FORCE
        //----------------------------------------
        updateForce(b);

        //----------------------------------------
        // ROTATION
        //----------------------------------------
        if(b.desiredDirection.length() > 0.001f)
        {
            HUGO::Vector3 lookDir =b.desiredDirection.normalized();

            HUGO::Float rotDir =beastieTr.getRotation().forward().cross(lookDir).y;

            HUGO::Float angularVel = std::clamp(rotDir, -1.0f, 1.0f) * 3.0f;

            pack.modify<RigidBodyComponent>([angularVel](RigidBodyComponent& rb)
            {
                rb.setAngularVelocity(HUGO::Vector3(0, angularVel, 0));
            });
        }
        else
        {
            pack.modify<RigidBodyComponent>([](RigidBodyComponent& rb)
            {
                rb.setAngularVelocity(HUGO::Vector3(0.0f));
            });
        }
    }
}
void BeastieSystem::update(uint32_t dt)
{
    auto& input = scene.getInput();

    for (auto& pack : scene.query<BeastieComponent, TransformComponent, AnimationComponent, AudioComponent, ColliderComponent>())
    {
        TransformComponent& beastieTr = pack.getDeprecated<TransformComponent>();
        BeastieComponent& b = pack.getDeprecated<BeastieComponent>();

        //----------------------------------------
        // SPAWN DELAY
        //----------------------------------------

        if(!b.aiEnabled)
        {
            b.currentSpawnDelay -= dt / 1000.0f;

            if(b.currentSpawnDelay <= 0.0f)
            {
                b.aiEnabled = true;

                pack.modify<AudioComponent>([](AudioComponent& a)
                {
                    a.isPlaying = true;
                });
            }
            else
            {
                continue;
            }
        }

        //----------------------------------------
        // PLAYER INFO
        //----------------------------------------

        const TransformComponent& playerTr = scene.getComponentConst<TransformComponent>(player);

        HUGO::Bool isMoving = input->isActionPressed("move");

        HUGO::Vector3 toPlayer = playerTr.getWorldPosition() - beastieTr.getWorldPosition();

        toPlayer.y = 0.0f;

        HUGO::Float distance = toPlayer.length();

        if(distance > 0.001f)
        {
            toPlayer.normalize();
        }

        //----------------------------------------
        // STATE MACHINE
        //----------------------------------------

        if(distance > b.maxRange)
        {
            if(b.currentState != BeastieState::WANDER)
            {
                b.currentState = BeastieState::WANDER;
                b.wanderTime = 0.0f;
            }
        }
        else if(isMoving && distance <= b.minRange)
        {
            b.currentState = BeastieState::PURSUE;
            b.isSlowMove = false;
        }
        else if(b.isSlowMove)
        {
            b.currentState = BeastieState::PURSUE;
        }
        else
        {
            b.currentState = BeastieState::STOP;
        }

        if (b.isFastMove){
            b.currentState = BeastieState::PURSUE;
            if(!rageSoundIncreased){
                pack.modify<AudioComponent>([](AudioComponent& a)
                {
                    a.pitch = 1.5f;
                });
                rageSoundIncreased = true;
                for (auto& pack1: scene.query<AudioComponent, MapComponent>()){
                    pack.modify<AudioComponent>([](AudioComponent& a)
                    {
                        a.pitch = 2.0f;
                    });
                }
            }
        }


        //----------------------------------------
        // BEHAVIOUR
        //----------------------------------------

        switch(b.currentState)
        {
            //------------------------------------
            // PURSUE
            //------------------------------------

            case BeastieState::PURSUE:
            {
                b.desiredDirection = toPlayer;

                pack.modify<AnimationComponent>([](AnimationComponent& a)
                {
                    a.setEnabled("Run", true);
                    a.setEnabled("Stop", false);
                });

                break;
            }

            //------------------------------------
            // STOP
            //------------------------------------

            case BeastieState::STOP:
            {
                b.desiredDirection =
                    HUGO::Vector3(0.0f);

                pack.modify<AnimationComponent>(
                    [](AnimationComponent& a)
                    {
                        a.setEnabled("Run", false);
                        a.setEnabled("Stop", true);
                    });

                break;
            }

            //------------------------------------
            // WANDER
            //------------------------------------

            case BeastieState::WANDER:
            {
                b.wanderTime -= dt / 1000.0f;

                if(b.wanderTime <= 0.0f)
                {
                    b.wanderTime =
                        b.wanderInterval;

                    HUGO::Degree angle(
                        rand() % 360);

                    float rad =
                        angle.valueRadians();

                    b.wanderDirection =
                        HUGO::Vector3(
                            cos(rad),
                            0.0f,
                            sin(rad));

                    b.wanderDirection.normalize();
                }

                b.desiredDirection =
                    b.wanderDirection;

                pack.modify<AnimationComponent>(
                    [](AnimationComponent& a)
                    {
                        a.setEnabled("Run", true);
                        a.setEnabled("Stop", false);
                    });

                break;
            }
        }

        //----------------------------------------
        // COLLISIONS
        //----------------------------------------

        for(auto collisionId :
            pack.getConst<ColliderComponent>()
            .getCollisionInfo())
        {
            if(scene.hasComponent<PlayerComponent>(
                collisionId))
            {
                scene.changeAndDestroyPreviousScene(
                    "DefeatScene");
            }
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
