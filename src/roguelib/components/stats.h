#pragma once

#include <string>

struct stats
{
    std::string name;
    float age;
    int str;
    int dex;
    int con;
    int intel;
    int wis;
    int cha;
    int experience;
    float movement_speed;
    std::string race;
};

struct derived_stats
{
    float physical_critical_strike_chance{ 0.0f };
    float physical_critical_strike_damage{ 0.0f };
    float physical_damage_modifier{ 0.0f };
    float physical_evasion{ 0.0f };
    float magical_critical_strike_chance{ 0.0f };
    float magical_critical_strike_damage{ 0.0f };
    float magical_damage_modifier{ 0.0f };
    float magical_evasion{ 0.0f };
};

derived_stats compute_derived_stats(const stats& base_stats);
