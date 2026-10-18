#include "damage.h"
#include "log_throw.h"

namespace ru {
    
    damage_type DamageTypeFromString(std::string_view dt_str)
    {
        if(dt_str == "physical") {
            return damage_type::physical;
        } else if(dt_str == "arcane") {
            return damage_type::arcane;
        } else if(dt_str == "fire") {
            return damage_type::fire;
        } else if(dt_str == "ice") {
            return damage_type::ice;
        } else if(dt_str == "lightning") {
            return damage_type::lightning;
        } else if(dt_str == "poison") {
            return damage_type::poison;
        } else if(dt_str == "healing") {
            return damage_type::healing;
        } else if(dt_str == "slashing") {
            return damage_type::slashing;
        } else if(dt_str == "piercing") {
            return damage_type::piercing;
        } else if(dt_str == "blunt") {
            return damage_type::blunt;
        } else {
            log_throw("Unknown damage_type string: '{}'", dt_str);
        }
    }


    bool parse_amount(std::string& amount, damage_type& type, const nlohmann::json& j)
    {
        bool have_amount = false;
        if(j.contains("amount")) {
            if(!j["amount"].is_string()) {
                log_throw("While parsing damage object, expected 'amount' field to be a string. It was of type '{}'.", j["amount"].type_name());
            }
            amount = j["amount"].get<std::string>();
            have_amount = true;
        }
        if(j.contains("type")) {
            if(!j["type"].is_string()) {
                log_throw("While parsing damage object, expected 'type' field to be a string. It was of type '{}'.", j["type"].type_name());
            }
            type = DamageTypeFromString(j["type"].get<std::string>());
        }
        return have_amount;
    }

    damage ParseDamageFromJson(const nlohmann::json& j)
    {
        damage dmg;
        if(j.is_string()) {
            // Assume a simple on-hit amount of none type damage, e.g. "damage": "1d10+3"
            dmg.on_hit_amount = j.get<std::string>();
        } else if(j.is_object()) {
            // More complex object.
            if(parse_amount(dmg.on_hit_amount, dmg.on_hit_type, j)) {
                dmg.enabled |= damage::damage_enablers::instant;
            }
            if(j.contains("instant")) {
                if(parse_amount(dmg.on_hit_amount, dmg.on_hit_type, j["instant"])) {
                    dmg.enabled |= damage::damage_enablers::instant;
                }
            }
            if(j.contains("over_time")) {
                if(parse_amount(dmg.over_time_amount, dmg.over_time_type, j["instant"])) {
                    dmg.enabled |= damage::damage_enablers::over_time;
                }
                if(j["instant"].contains("duration")) {
                    if(!j["instant"]["duration"].is_number()) {
                        log_throw("While parsing damage object, expected 'duration' field to be a number. It was of type '{}'.", j["instant"]["duration"].type_name());
                    }
                    dmg.over_time_duration = j["instant"]["duration"].get<float>();
                }
            }

            // multiplier
            if(j.contains("multiplier")) {
                if(!j["multiplier"].is_number()) {
                    log_throw("While parsing damage object, expected 'multiplier' field to be a number. It was of type '{}'.", j["multiplier"].type_name());
                }
                dmg.dmg_multiplier = j["multiplier"].get<float>();
            }
        }
        return dmg;
    }

}