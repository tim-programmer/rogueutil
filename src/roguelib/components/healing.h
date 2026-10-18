#pragma once

#include <string>
#include <string_view>

namespace ru {

    enum class healing_type {
        none,
        physical,
        holy,
        nature,
        arcane,
    };

    struct healing
    {
        enum healing_enablers {
            none,
            instant     = 1u << 0,
            over_time   = 1u << 1,
        };
        std::string name;
        std::string description;

        std::string instant_amount;
        healing_type instant_type{ healing_type::none };

        std::string over_time_amount;
        float over_time_duration{ 0.0f };
        healing_type over_time_type{ healing_type::none };
    };

    healing_type HealingTypeFromString(std::string_view ht_str);
}
