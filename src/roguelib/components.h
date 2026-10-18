#pragma once

#include <string>

#include <flecs.h>
#include <nlohmann/json.hpp>
#include <log_throw.h>

#include "damage_type.h"

namespace ru {

    enum class cost_type {
        none,
        mana,
        stamina,
        health,
        // Add more as needed.
    };

    struct ability
    {
        std::string name;
        std::string description;
        /// variable cost might be a thing at some point.
        int base_cost{ 0 };
        /// Range of the effect, 0 means self-target only.
        /// Range of 1 means adjacent tile.
        int range{ 0 };
        /// Area of effect radius, 0 means no area of effect, single target or self only.
        int area_of_effect{ 0 };
        /// Minimum level required to use the ability.
        int level_requirement{ 1 };
        /// Damage or healing amount. A healing ability would have positive damage, but a \c dmg_type of healing.
        std::string damage;
        /// Type of damage or healing.
        damage_type dmg_type{ damage_type::none };
        /// Type of resource used to pay for the ability.
        cost_type cst_type{ cost_type::none };
        /// Icon name to use for the ability
        std::string icon;
    };

    class abilities
    {
    public:
        abilities() = default;
        ~abilities() = default;

        void load(const nlohmann::json& j)
        {
            if(!j.contains("abilities")) {
                log_throw("No 'abilities' key in abilities game data.");
            }
            for(auto& [key, value] : j["abilities"].items()) {
                ability ab;
                ab.name = key;

                if(value.contains("description") && value["description"].is_string()) {
                    ab.description = value["description"].get<std::string>();
                }
                if(value.contains("cost") && value["cost"].is_number_integer()) {
                    ab.base_cost = value["cost"].get<int>();
                }
                if(value.contains("range") && value["range"].is_number_integer()) {
                    ab.range = value["range"].get<int>();
                }
                if(value.contains("area_of_effect") && value["area_of_effect"].is_number_integer()) {
                    ab.area_of_effect = value["area_of_effect"].get<int>();
                }
                if(value.contains("level_requirement") && value["level_requirement"].is_number_integer()) {
                    ab.level_requirement = value["level_requirement"].get<int>();
                }
                if(value.contains("damage")) {
                    if(value["damage"].is_string()) {
                        ab.damage = value["damage"].get<std::string>();
                    } else if (value["damage"].is_number_integer()) {
                        ab.damage = std::to_string(value["damage"].get<int>());
                    }
                }
                if(value.contains("damage_type") && value["damage_type"].is_string()) {
                    std::string dt = value["damage_type"].get<std::string>();
                    ab.dmg_type = DamageTypeFromString(dt);
                }
                if(value.contains("cost_type") && value["cost_type"].is_string()) {
                    std::string ct = value["cost_type"].get<std::string>();
                    if(ct == "mana") {
                        ab.cst_type = cost_type::mana;
                    } else if(ct == "stamina") {
                        ab.cst_type = cost_type::stamina;
                    } else if(ct == "health") {
                        ab.cst_type = cost_type::health;
                    } else {
                        log_throw("Ability '{}' has unknown 'cost_type' value '{}'.", ab.name, ct);
                    }
                } else {
                    log_throw("Ability '{}' missing 'cost_type' or 'cost_type' is not a string.", ab.name);
                }
                if(!value.contains("icon") || !value["icon"].is_string()) {
                    log_throw("Ability '{}' missing 'icon' or 'icon' is not a string.", ab.name);
                } else {
                    ab.icon = value["icon"].get<std::string>();
                }

                // Add the ability to the map.
                addAbility(ab.name, ab);
            }
        }

        void addAbility(const std::string& name, const ability& ab)
        {
            abilities_map[name] = ab;
        }

        const ability* getAbility(const std::string& name) const
        {
            auto it = abilities_map.find(name);
            if (it != abilities_map.end())
            {
                return &(it->second);
            }
            log_throw("Ability '{}' not found", name);
        }

    private:
        std::map<std::string, ability> abilities_map;
    };

    namespace components {

        struct Position
        {
            int x;
            int y;
        };

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

        struct mana
        {
            int cur_mana;
            int max_mana;
        };

        struct stamina
        {
            int cur_stamina;
            int max_stamina;
        };

        struct harmable
        {
            int cur_hp{ 0 };
            int max_hp{ 0 };
            // is entity invulnerable?
            bool invulnerable{ false };
            /// time, in turns, for invulnerability to last.
            float invulnerability_timer{ 0 };
        };

        struct resolved_ability
        {
            int damage;
        };

        struct combat_intent
        {
            flecs::entity attacker;
            const ru::ability* ability;
            resolved_ability result;
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

        struct inventory {
            std::vector<ru::item> item_list;
        };


        derived_stats compute_derived_stats(const stats& base_stats);

    }
}