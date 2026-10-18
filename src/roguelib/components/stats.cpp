#include "stats.h"

derived_stats compute_derived_stats(const stats& base_stats)
{
    derived_stats d_stats;

    // Need to pull these modifiers from configuration file.
    d_stats.physical_critical_strike_chance = base_stats.dex * 0.01f;
    d_stats.physical_critical_strike_damage = 1.5f + (base_stats.str * 0.02f);
    d_stats.physical_damage_modifier = 1.0f + (base_stats.str * 0.03f);
    d_stats.physical_evasion = base_stats.dex * 0.5f;

    d_stats.magical_critical_strike_chance = base_stats.intel * 0.01f;
    d_stats.magical_critical_strike_damage = 1.5f + (base_stats.intel * 0.02f);
    d_stats.magical_damage_modifier = 1.0f + (base_stats.intel * 0.03f);
    d_stats.magical_evasion = base_stats.wis * 0.5f;

    return d_stats;
}