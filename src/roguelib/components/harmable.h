#pragma once

struct harmable
{
    int cur_hp{ 0 };
    int max_hp{ 0 };
    // is entity invulnerable?
    bool invulnerable{ false };
    /// time, in turns, for invulnerability to last.
    float invulnerability_timer{ 0 };
};
