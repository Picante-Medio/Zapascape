#pragma once
#include "HugoTypes.h"
#include "Message.h"

struct ChangeGameMessage : HUGO::Message {
    std::string sceneName = "Game";
};
struct DeactiveRitualMessage : HUGO::Message {
    std::string text = "";
};
