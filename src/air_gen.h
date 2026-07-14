#include "air.h"

#include <string_view>
#include <vector>
#include <set>
#include <map>
#include <format>
#include <print>

struct Arg {
    std::string name;
    std::string type;
};

class AirGenerator {
private:
    TacProgram _prog;

    std::string operand_type(const Value& v, std::map<std::string,std::string>& temp_types, std::map<std::string,std::string>& var_types);

    void type_err(std::string_view type_1, std::string_view type_2)
    {
        throw std::runtime_error(std::format("AIR: It seems to me that type '{}' differs from '{}'", type_1, type_2));
        // std::vprint_nonunicode(fmt.get(), std::make_format_args(args...));
    }

public:
    AirGenerator(TacProgram prog)
        : _prog(prog) {}

    AirProgram analyze();
    void dead_code_remove();
    void type_check();
};