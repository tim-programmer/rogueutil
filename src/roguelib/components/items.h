#pragma once

#include <map>
#include <string>
#include <string_view>

#include <nlohmann/json.hpp>
#include <log_throw.h>

#include "damage_type.h"

namespace ru {

    enum class item_type {
        weapon,
        armor,
        consumable,
        /// A light source, generally equipped by the player so they can see.
        light,
        quest,
        miscellaneous,
        // Add more as needed.
    };

    enum class item_subtype {
        // Weapon subtypes
        melee,
        ranged,
        magic,
        // Armor subtypes
        light,
        medium,
        heavy,
        // Consumable subtypes
        potion,
        scroll,
        // Add more as needed.
    };

    enum class boostable_stats {
        strength,
        dexterity,
        constitution,
        intelligence,
        wisdom,
        charisma,
        // Add more as needed.
    };

    boostable_stats StatFromString(std::string_view stat_str);

    enum class effect_type {
        damage,
        restore_health,
        heal_over_time,
        restore_mana,
        mana_over_time,
        restore_stamina,
        stamina_over_time,
        boost_health,
        boost_mana,
        boost_stamina,
        boost_stat,
        poison,
        slow,
        haste,
        burn,
        bleed,
        blindness,
        stun,
        vision,
        hearing,
        invisibility,
        light_source,
        // Add more as needed.
    };

    effect_type EffectTypeFromString(std::string_view et);

    struct Effect {
        effect_type type;
        damage_type dmg_type{ damage_type::none }; /// For damage effects.
        /// Can be a dice string or fixed value.
        /// For damage effects, this is the damage amount.
        /// For healing effects, this is the healing amount.
        /// For boost_stat effects, this is the amount to boost the stat by.
        /// Also the amount for restore mana and stamina effects.
        /// XXX make this std::any?
        std::string value;
        boostable_stats stat;       /// For boost_stat effects, the name of the stat to boost.
        int duration{ 0 };          /// Duration in ticks/turns, 0 for instant effects.
        int period{ 0 };            /// Number of ticks/turns between effect applications.
        float chance{ 1.0f };       /// Chance of effect occurring, 0.0 to 1.0.
        // area effect?
    };

    Effect ParseEffect(const nlohmann::json& j);

    struct Item {
        std::string name;
        std::string description;
        item_type type;
        item_subtype subtype;
        // Effects applied to the target of the item when it is used.
        std::vector<Effect> use_effects;
        // Effects applied to the user when the item is equipped (for weapons/armor).
        std::vector<Effect> equip_effects;
        float weight{ 0.0f };
        int value{ 0 };
        std::string rarity{ "common" };
        /// Number of uses before the item is consumed or breaks. 0 = unlimited uses.
        /// This is intended for consumables. For weapons/armor, durability would be a better mechanic.
        int uses{ 0 };
        /// Number of ticks/turns before the item can be used again.
        float cooldown{ 0.0f };
        /// Whether the item can be stacked in the inventory.
        bool stackable{ false };
        /// @brief Maximum number of items that can be stacked together.
        int stack_size{ 1 };
        // level requirement?
        // How much damage the item can take before breaking (for weapons/armor primarily).
        // 0 means the item does not degrade/unbreakable.
        int durability{ 0 };
        /// Whether the item is currently equipped, i.e. ready for use.
        bool equipped{ false };
        /// Quantity of the item (for stackable items).
        int quantity{ 1 };
        /// Fragile items are destroyed when the entity that has them in it's inventory dies.
        /// Doesn't mean they can't be stolen, pilfered, magically exchanged though.
        bool fragile{ false };
    };

    class Items
    {
    public:
        Items() = default;
        ~Items() = default;

        void load(const nlohmann::json& j)
        {
            if(!j.contains("items")) {
                log_throw("No 'items' key in items game data.");
            }
            for(auto& [key, value] : j["items"].items()) {
                Item it;
                it.name = key;

                if(value.contains("description") && value["description"].is_string()) {
                    it.description = value["description"].get<std::string>();
                }
                if(value.contains("type") && value["type"].is_string()) {
                    std::string t = value["type"].get<std::string>();
                    if(t == "weapon") {
                        it.type = item_type::weapon;
                    } else if(t == "armor") {
                        it.type = item_type::armor;
                    } else if(t == "consumable") {
                        it.type = item_type::consumable;
                    } else if(t == "light") {
                        it.type = item_type::light;
                    } else if(t == "quest") {
                        it.type = item_type::quest;
                    } else if(t == "miscellaneous") {
                        it.type = item_type::miscellaneous;
                    } else {
                        log_throw("Item '{}' has unknown 'type' value '{}'.", it.name, t);
                    }
                } else {
                    log_throw("Item '{}' missing 'type' or 'type' is not a string.", it.name);
                }

                if(value.contains("subtype") && value["subtype"].is_string()) {
                    std::string st = value["subtype"].get<std::string>();
                    if(st == "melee") {
                        it.subtype = item_subtype::melee;
                    } else if(st == "ranged") {
                        it.subtype = item_subtype::ranged;
                    } else if(st == "light") {
                        it.subtype = item_subtype::light;
                    } else if(st == "medium") {
                        it.subtype = item_subtype::medium;
                    } else if(st == "heavy") {
                        it.subtype = item_subtype::heavy;
                    } else if(st == "potion") {
                        it.subtype = item_subtype::potion;
                    } else if(st == "scroll") {
                        it.subtype = item_subtype::scroll;
                    } else {
                        log_throw("Item '{}' has unknown 'subtype' value '{}'.", it.name, st);
                    }
                }

                if(value.contains("use_effects")) {
                    if(!value["use_effects"].is_array()) {
                        log_throw("Item '{}' 'use_effects' must be an array.", it.name);
                    }

                    for(auto& ef_val : value["use_effects"]) {
                        it.use_effects.emplace_back(ParseEffect(ef_val));
                    }
                }

                if(value.contains("equip_effects")) {
                    if(!value["equip_effects"].is_array()) {
                        log_throw("Item '{}' 'equip_effects' must be an array.", it.name);
                    }

                    for(auto& ef_val : value["equip_effects"]) {
                        it.equip_effects.emplace_back(ParseEffect(ef_val));
                    }
                }

                if(value.contains("weight") && value["weight"].is_number_float()) {
                    it.weight = value["weight"].get<float>();
                }
                if(value.contains("value") && value["value"].is_number_integer()) {
                    it.value = value["value"].get<int>();
                }
                if(value.contains("rarity") && value["rarity"].is_string()) {
                    it.rarity = value["rarity"].get<std::string>();
                }
                if(value.contains("uses") && value["uses"].is_number_integer()) {
                    it.uses = value["uses"].get<int>();
                }
                if(value.contains("cooldown") && value["cooldown"].is_number_float()) {
                    it.cooldown = value["cooldown"].get<float>();
                }
                if(value.contains("stackable") && value["stackable"].is_boolean()) {
                    it.stackable = value["stackable"].get<bool>();
                }
                if(value.contains("stack_size") && value["stack_size"].is_number_integer()) {
                    it.stack_size = value["stack_size"].get<int>();
                }
                if(value.contains("durability") && value["durability"].is_number_integer()) {
                    it.durability = value["durability"].get<int>();
                }

                it.fragile = value.value<bool>("fragile", false);

                // Add the item to the map.
                addItem(it.name, it);
            }
        }

        void addItem(const std::string& name, const Item& it)
        {
            items_map[name] = it;
        }

        /// @brief Get an item by name.
        /// @param name The name of the item.
        /// @return An instance of the item.
        Item getItem(const std::string& name) const
        {
            auto it = items_map.find(name);
            if(it != items_map.end()) {
                return it->second;
            } else {
                log_throw("Item '{}' not found.", name);
            }
        }
    private:
        std::map<std::string, Item> items_map;
    };

} // namespace ru