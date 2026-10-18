#pragma once

#include <string>
#include <string_view>
#include <type_traits>

#include <nlohmann/json.hpp>

namespace ru {

    enum class damage_type {
        none,
        physical,
        arcane,
        fire,
        ice,
        lightning,
        poison,
        healing,
        slashing,
        piercing,
        blunt,
        // Add more as needed.
    };

    damage_type DamageTypeFromString(std::string_view dt_str);

    struct damage
    {
        /// @brief What we have enabled for damage types.
        enum damage_enablers {
            none,
            /// @brief Instant damage is enabled when this bit is set.
            instant             = 1u << 0,
            /// @brief Over time damage is enabled when this bit is set.
            over_time           = 1u << 1,
            /// @brief Weapon damage should be used for the calculation.
            /// if weapon and multiplier are both set the the multiplier applies to weapon damage.
            weapon              = 1u << 2,
            /// @brief The damage multiplier is enabled when this bit is set.
            multiplier          = 1u << 3,
        };

        /// @brief Various flags that effect how damage is calculated.
        damage_enablers enabled{ none };

        /// @brief The amount of on-hit damage to apply.
        std::string on_hit_amount;
        /// @brief The type of on-hit damage.
        damage_type on_hit_type{ damage_type::none };

        /// @brief The damage over time to apply.
        std::string over_time_amount;
        /// @brief The duration of the over time damage.
        float over_time_duration{ 0.0f };
        /// @brief The type of over time damage.
        damage_type over_time_type{ damage_type::none };

        /// @brief A damage multiplier applied to all related calculations
        float dmg_multiplier{ 1.0f };
    };

    inline damage::damage_enablers operator|=(damage::damage_enablers& a, damage::damage_enablers b) {
        a = static_cast<damage::damage_enablers>(std::to_underlying<damage::damage_enablers>(a) | std::to_underlying<damage::damage_enablers>(b));
        return a;
    }

    inline damage::damage_enablers operator&(damage::damage_enablers a, damage::damage_enablers b) {
        return static_cast<damage::damage_enablers>(std::to_underlying<damage::damage_enablers>(a) & std::to_underlying<damage::damage_enablers>(b));
    }

    damage ParseDamageFromJson(const nlohmann::json& j);

} // namespace ru