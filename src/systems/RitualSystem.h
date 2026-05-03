#pragma once

#include "HUGO.h"
#include "System.h"

HSYSTEM(RitualSystem)
    EntityID player;
    EntityID puertaFinal;

    HUGO::Int activeRituals;
    HUGO::Int maxRitualsNumber;

    void activateRitual(EntityID ritual, EntityID cubo);
public:
    void init() override;
    //Called every time the scene is loaded
    void onLoad() override;

    void fixedUpdate(uint32_t dt) override;
    void update(uint32_t dt) override;

    // Setters
    void setMaximunRitualsNumber(HUGO::Int newNumber);
    void setActiveRitualsNumber(HUGO::Int newNumber);
    
END_HSYSTEM()