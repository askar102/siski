#include "air.h"

#include <cstdint>
#include <string_view>
#include <vector>
#include <set>
#include <map>
#include <format>
#include <print>
#include <limits>
#include <optional>

struct Arg {
    std::string name;
    std::string type;
};

struct TypeRange {
    int64_t min_val;
    uint64_t max_val;
    bool is_signed;
};

class AirGenerator {
private:
    TacProgram _prog;

    std::string operand_type(const Value& v, std::map<std::string,std::string>& temp_types, std::map<std::string,std::string>& var_types);

    [[noreturn]]
    void type_err(std::string_view type_1, std::string_view type_2)
    {
        throw std::runtime_error(std::format("AIR: It seems to me that type '{}' differs from '{}'", type_1, type_2));
        // std::vprint_nonunicode(fmt.get(), std::make_format_args(args...));
    }

    std::optional<TypeRange> get_type_range(std::string_view data_type) 
    {
        if (data_type == "I8") return TypeRange{INT8_MIN,  (uint64_t)INT8_MAX,  true};
        if (data_type == "I16") return TypeRange{INT16_MIN, (uint64_t)INT16_MAX, true};
        if (data_type == "I32") return TypeRange{INT32_MIN, (uint64_t)INT32_MAX, true};
        if (data_type == "I64") return TypeRange{INT64_MIN, (uint64_t)INT64_MAX, true};
        
        if (data_type == "U8") return TypeRange{0, UINT8_MAX,  false};
        if (data_type == "U16") return TypeRange{0, UINT16_MAX, false};
        if (data_type == "U32") return TypeRange{0, UINT32_MAX, false};
        if (data_type == "U64") return TypeRange{0, UINT64_MAX, false};
        
        return std::nullopt;
    }

    bool can_fit(int64_t val, std::string_view data_type) 
    {
        auto range = get_type_range(data_type);
        if (!range) return false;

        if (!range->is_signed && val < 0)
        {
            return false;
        }

        if (val < range->min_val) {
            return false;
        }

        if (val >= 0 && static_cast<uint64_t>(val) > range->max_val)
        {
            return false;
        }
        
        return true;
    }

    bool const_val(const Value& v, std::map<std::string,int64_t>& const_temps, int64_t& out) {
        if (v.type == VALUE_TYPE::CONST) { 
            out = v.constVal; 
            return true; 
        }

        if (v.type == VALUE_TYPE::TEMP && const_temps.count(v.name))
        {
            out = const_temps[v.name]; 
            return true; 
        }
        return false;
    }

public:
    AirGenerator(TacProgram prog)
        : _prog(prog) {}

    AirProgram analyze();
    void dead_code_remove();
    void type_check();
};