#pragma once

#include <map>
#include <string>

#include <nlohmann/json.hpp>

#include <log_throw.h>

#include "damage.h"
#include "healing.h"

namespace ru {

    enum class cost_type {
        none,
        mana,
        stamina,
        health,
        // Add more as needed.
    };

    struct Ability
    {
        /// @brief Name of the ability.
        std::string name;
        /// @brief Description of the ability.
        std::string description;
        /// variable cost might be a thing at some point.
        int base_cost{ 0 };
        /// Range of the effect, 0 means self-target only.
        /// Range of 1 means adjacent tile.
        int range{ 0 };
        /// Area of effect radius, 0 means no area of effect, single target or self only.
        int area_of_effect{ 0 };
        /// Minimum level required to use the ability. 0 means no requirement.
        int level_requirement{ 0 };
        /// @brief Amount of damage dealy by an ability.
        damage dmg;
        /// @brief Amount of healing done by an ability.
        healing heal;
        /// Type of resource used to pay for the ability.
        cost_type cst_type{ cost_type::none };
        /// Icon name to use for the ability
        std::string icon;
    };

    class Abilities
    {
    public:
        Abilities() = default;
        ~Abilities() = default;

        void load(const nlohmann::json& j)
        {
            if(!j.contains("abilities")) {
                log_throw("No 'abilities' key in abilities game data.");
            }
            for(auto& [key, value] : j["abilities"].items()) {
                Ability ab;
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

        void addAbility(const std::string& name, const Ability& ab)
        {
            abilities_map[name] = ab;
        }

        const Ability* getAbility(const std::string& name) const
        {
            auto it = abilities_map.find(name);
            if (it != abilities_map.end())
            {
                return &(it->second);
            }
            log_throw("Ability '{}' not found", name);
        }

    private:
        std::map<std::string, Ability> abilities_map;
    };

} // namespace ru
