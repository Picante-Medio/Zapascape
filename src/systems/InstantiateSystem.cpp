#include "InstantiateSystem.h"

void InstantiateSystem::init() 
{

}

void 
InstantiateSystem::onLoad() 
{
    // // find player and mark controller
    // for (auto& pack: scene.query<InstantiateComponent>()){
    //     InstantiateComponent ic = pack.getConst<InstantiateComponent>();
    //     ErrorOrType<EntityID> entity = scene.instantiatePrefab(ic.getPrefabName());

    //     if(!entity.isError()) {
    //         // Do something with the instantiated entity if needed
    //     }
}

void
InstantiateSystem::update(uint32_t dt) {
}

void
InstantiateSystem::fixedUpdate(uint32_t) {

}
