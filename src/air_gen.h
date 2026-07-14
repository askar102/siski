#include "air.h"

#include <vector>
#include <set>
#include <map>

class AirGenerator {
private:
    TacProgram _prog;

    std::string operand_type(const Value& v, std::map<std::string,std::string>& temp_types, std::map<std::string,std::string>& var_types);

public:
    AirGenerator(TacProgram prog)
        : _prog(prog) {}

    AirProgram analyze(TacProgram prog);
    void dead_code_remove();
    void type_check();
};