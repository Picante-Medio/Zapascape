#include "TestSystem.h"

#include <iostream>

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "TweenComponent.h"
#include "ParentComponent.h"
#include "ChildrenComponent.h"
#include "PanelComponent.h"
#include "ButtonComponent.h"
#include "TextComponent.h"
#include "InputFieldComponent.h"
#include "LightComponent.h"
#include "SliderComponent.h"
#include "ParticleEmitterComponent.h"
#include "CameraComponent.h"
#include "PlayerComponent.h"
#include "AnimationComponent.h"
#include "AudioComponent.h"
#include "MovementComponent.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"
#include "MovementRBComponent.h"
#include "MarkComponent.h"

#include "BeastieComponent.h"
void

TestSystem::init() {
    e1 = scene.createEntity();
    scene.addComponent<TransformComponent>(e1, [](TransformComponent& t1) {
        t1.setPosition(HUGO::Vector3(0, 0.0f, 0));

        t1.setRotation(HUGO::Quaternion(HUGO::Radian(HUGO::PI), HUGO::Vector3(0,1,0)));
        t1.setScale(HUGO::Vector3(0.5f, 0.5f, 0.5f));
    });
    scene.addComponent<CameraComponent>(e1, [](CameraComponent& comp){
        comp.setMain(true);
        comp.setNearClip(0.01f); 
        comp.setFarClip(1000.0f);
    });
    scene.addComponent<PlayerComponent>(e1);
    scene.addComponent<MovementRBComponent>(e1);
    scene.addComponent<ColliderComponent>(e1, [](ColliderComponent& c){
        c.info = HUGO::BoxColliderInfo{(HUGO::Vector3(1.2f, 1.0f, 1.2f))};
        c.setFriction(0.6f);
        c.setPositionOffset({0.0f, 1.0f, 0.0f});
    });
    scene.addComponent<RigidBodyComponent>(e1, [](RigidBodyComponent& rb){
        rb.setGravity(HUGO::Vector3(0.0f));
        rb.setMass(5.0f);
        rb.setLinearDamping(0.95f);
        rb.setBodyType(HUGO::BodyType::DYNAMIC);
    });

    EntityID playerArm = scene.createEntity();
    scene.addComponent<TransformComponent>(playerArm, [](TransformComponent& t1) {
        t1.setRotation(HUGO::Quaternion(HUGO::Radian(HUGO::PI), HUGO::Vector3(0,1,0)));
        t1.setPosition(HUGO::Vector3(0.0f, -3, -1.5f));
        t1.setScale(HUGO::Vector3(0.5f, 0.5f, 0.5f));
    });
    scene.addComponent<RenderComponent>(playerArm, [](RenderComponent& rc) {
        rc.setMeshName("player.mesh");
    });
    scene.addComponent<AnimationComponent>(playerArm, [](AnimationComponent& rc) {
        rc.addAnimation("Idle", true, true);
    });
    
    EntityID light = scene.createEntity();
    scene.addComponent<TransformComponent>(light, [](TransformComponent& tr){
        tr.setPosition(HUGO::Vector3(0.2, 1.0f, 0.5f));
        tr.setRotation(HUGO::Quaternion(HUGO::Degree(-10.0f), HUGO::Vector3(1, 0, 0)));
    });

    scene.addComponent<LightComponent>(light, [](LightComponent& lg){
        lg.setLightType(HUGO::LightType::LT_SPOTLIGHT);
        lg.setAttenuation(HUGO::Vector4(1.0f, 0.0f, 0.01f, 20.0f));
        lg.setDiffuse(HUGO::Vector4(0.9f, 0.85f, 0.7f, 1.0f));
        lg.setSpecular(HUGO::Vector4(0.5f, 0.5f, 0.5f, 1.0f));

        lg.setSpotInner(HUGO::Radian(HUGO::Degree(20.0f)));
        lg.setSpotOuter(HUGO::Radian(HUGO::Degree(35.0f)));
        lg.setSpotFallOff(1.0f); 
        lg.setSpotNearClip(0.2f);
     });
    
     scene.addComponent<ChildrenComponent>(e1, [light, playerArm](ChildrenComponent& c){ 
        c.addChild(light);
        c.addChild(playerArm);
    });

    //ErrorOrType<EntityID> prefab = scene.instantiatePrefab("DynPlayer");
    //beastie

    e2 = scene.createEntity();
    scene.addComponent<TransformComponent>(e2, [](TransformComponent& t2) {  
        t2.setRotation(HUGO::Quaternion(HUGO::Radian(HUGO::PI), HUGO::Vector3(0,1,0)));
        t2.setPosition(HUGO::Vector3(0, 0, -5));
        t2.setScale(HUGO::Vector3(0.4f, 0.4f, 0.4f));
    });
    scene.addComponent<RenderComponent>(e2, [](RenderComponent& rc) {
        rc.setMeshName("beastie.mesh");
    });
    scene.addComponent<AnimationComponent>(e2);
    scene.addComponent<ColliderComponent>(e2, [](ColliderComponent& c){
        c.info = HUGO::BoxColliderInfo{(HUGO::Vector3(0.5f))};
        c.positionOffset = HUGO::Vector3(0,0.6,0);
        c.setFriction(0.1f);
    });
    scene.addComponent<MovementRBComponent>(e2);
    scene.addComponent<BeastieComponent>(e2);
    scene.addComponent<RigidBodyComponent>(e2, [](RigidBodyComponent& rb){
        rb.setGravity(HUGO::Vector3(0.0f));
        rb.setMass(5.0f);
        rb.setLinearDamping(0.95f);
        rb.setBodyType(HUGO::BodyType::DYNAMIC);
    });
    
    
    // e3 = scene.createEntity(); 
    // scene.addComponent<TransformComponent>(e3, [](TransformComponent& t3) {
    //     t3.setPosition(HUGO::Vector3(-2, -1, 0));
    //     t3.setScale(HUGO::Vector3(0.5f, 0.5f, 0.5f));
    // });
    // scene.addComponent<RenderComponent>(e3, [](RenderComponent& rc3) {
    //     rc3.setMeshName("door.glb");
    // });

    // EntityID e4 = scene.createEntity(); 
    // scene.addComponent<TransformComponent>(e4, [](TransformComponent& t3) {
    //     t3.setPosition(HUGO::Vector3(2, -1, 0));
    //     t3.setScale(HUGO::Vector3(0.5f, 0.5f, 0.5f));
    // });
    // scene.addComponent<RenderComponent>(e4, [](RenderComponent& rc3) {
    //     rc3.setMeshName("ritual.glb");
    // });

    EntityID marks = scene.createEntity();
    scene.addComponent<MarkComponent>(marks);

    // ErrorOrType<EntityID> prefab = scene.instantiatePrefab("Wall");
}


void
TestSystem::onLoad() {}


void
TestSystem::update(uint32_t dt) {
}

void
TestSystem::fixedUpdate(uint32_t) {}
