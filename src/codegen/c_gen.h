#pragma once

#include "../tac.h"
#include "../air.h"

#include <cstdio>
#include <format>
#include <fstream>
#include <print>

#include <set>

// template <>
// struct std::formatter<Param> {
//     constexpr auto parse(std::format_parse_context& ctx) {
//         return ctx.begin();
//     }

//     auto format(const Param& p, std::format_context& ctx) const {
//         if (p.hasDefault) 
//         {
//             // idk
//             return std::format_to(ctx.out(), "{} {}", p.type, p.name);
//         }
//         return std::format_to(ctx.out(), "{} {}", p.type, p.name);
//     }
// };

// template <>
// struct std::formatter<std::vector<Param>> {
//     constexpr auto parse(std::format_parse_context& ctx) {
//         return ctx.begin();
//     }

//     auto format(const std::vector<Param>& vec, std::format_context& ctx) const {
//         auto out = ctx.out();
//         std::string_view separator = "";

//         for (const auto& pt : vec) {
//             out = std::format_to(out, "{}", separator);
//             out = std::format_to(out, "{}", pt);
//             separator = ", ";
//         }
//         return out;
//     }
// };

class CGen {
private:
    FILE* file;

    std::string val_to_c(const Value& val);

public:
    void generate(const AirProgram& prog);
};