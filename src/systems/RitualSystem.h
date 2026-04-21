#pragma once

#include "HUGO.h"
#include "System.h"

HSYSTEM(RitualSystem)
    //test
    EntityID e1;
    EntityID e2;
    EntityID e3;

    HUGO::Int activeRituals;
    HUGO::Int maxRitualsNumber;

    std::unordered_set<EntityID> rituals;
public:
    void init() override;
    //Called every time the scene is loaded
    void onLoad() override;

    void fixedUpdate(uint32_t dt) override;
    void update(uint32_t dt) override;

    // Setters
    void setMaximunRitualsNumber(HUGO::Int newNumber);
    void setActiveRitualsNumber(HUGO::Int newNumber);
    

    // Getters for the UI
    HUGO::Int getMaximunRitualsNumber() const;
    HUGO::Int getActiveRitualsNumber() const;
END_HSYSTEM()