#include "healing.h"
#include "log_throw.h"

namespace ru {
    
    healing_type HealingTypeFromString(std::string_view ht_str)
    {
        if(ht_str == "none") {
            return healing_type::none;
        } else if(ht_str == "physical") {
            return healing_type::physical;
        } else if(ht_str == "holy") {
            return healing_type::holy;
        } else if(ht_str == "nature") {
            return healing_type::nature;
        } else if(ht_str == "arcane") {
            return healing_type::arcane;
        } else {
            log_throw("Unknown healing_type string: '{}'", ht_str);
        }
    }

}
