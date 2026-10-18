#pragma once

#include <flecs.h>
#include "abilities.h"
#include "resolved_ability.h"

struct combat_intent
{
    flecs::entity attacker;
    const ru::Ability* ability;
    resolved_ability result;
};
