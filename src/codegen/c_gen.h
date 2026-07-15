#pragma once

#include "../tac.h"
#include "../air.h"

#include <cstdio>
#include <format>
#include <fstream>
#include <print>
#include <map>

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

    template<typename... Args>
    void puts_ins(FILE* file, std::format_string<Args...> fmt, Args&& ...args)
    {   
        // const std::basic_string<char, std::char_traits<char>, std::allocator<char>> & str = "{}";
        if (file) {
            // std::fputs("\t", file);
            // fmt.get() ретурнит стринг виев по константе
            // vprint_non... не юзает промежуточный string для чеканья UTF-8, если че то пойдет не так будем юзать
            // std::vprint_unicode 
            std::vprint_nonunicode(file, fmt.get(), std::make_format_args(args...));

            // std::fputs("\n", file);
            // std::format аллоцирует basic_string, нам такая хуйня не нужна
            // std::print(file, "    {}", std::format(fmt, std::forward<Args>(args)...));
        }
    }

    /* Returns: [ INT func_name(int a, int b   ]    
    ..................................................................^^^
     WITHOUT END. BE DANGEROUS! 
    */
    void puts_func(FILE* file, const TacFunc& fn, std::string end_with)
    {
        puts_ins(file, "{} {}(", to_c_type(fn.retType), fn.name);
            for (size_t k = 0; k < fn.params.size(); ++k)
            {
                puts_ins(file, "{}{} {}", k ? ", " : "", to_c_type(fn.params[k].type), fn.params[k].name);
            }
        puts_ins(file, "){}\n", end_with);
    }

    constexpr std::string_view to_c_type(const std::string_view t)
    {
        if (t == "I8")  return "int8_t";
        if (t == "I16") return "int16_t";
        if (t == "I32") return "int32_t";
        if (t == "I64") return "int64_t";
        if (t == "U8")  return "uint8_t";
        if (t == "U16") return "uint16_t";
        if (t == "U32") return "uint32_t";
        if (t == "U64") return "uint64_t";
        if (t == "U0") return "void";

        if (t == "int") return "int";
        else return t;
    }

public:
    void generate(const AirProgram& prog);
};