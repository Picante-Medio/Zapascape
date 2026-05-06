#pragma once
#include "HugoTypes.h"
#include "Message.h"

struct ChangeGameMessage : HUGO::Message {
    HUGO::String sceneName = "Game";
};
