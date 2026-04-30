#include "InstantiateSystem.h"
#include "InstantiateComponent.h"
#include "RenderComponent.h"

void InstantiateSystem::init() 
{

}

void 
InstantiateSystem::onLoad() 
{
    for (auto& pack: scene.query<InstantiateComponent>()){
        InstantiateComponent ic = pack.getConst<InstantiateComponent>();
        ErrorOrType<EntityID> entity = scene.instantiatePrefab(ic.getPrefabName());

        if(entity.isError()) {
            std::cout << "ERROR_DORI " << entity.getError().description();
        }
        else {
            std::cout << "ENTITY INSTANTIATED WITH ID " << entity.getValue() << "\n";
        }
    }
}

void
InstantiateSystem::update(uint32_t dt) {
    for (auto& pack: scene.query<InstantiateComponent>()){
        InstantiateComponent ic = pack.getConst<InstantiateComponent>();
        EntityID entity = ic.entity;
        // std::cout << "ENTITY ID IS " << entity << "\n";
    }
}

void
InstantiateSystem::fixedUpdate(uint32_t) {

}
