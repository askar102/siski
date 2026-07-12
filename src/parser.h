#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

#include "ast.h"

#include "lexer.h"

#include "log.h"

// И так, у нас есть парсер.
// Получается мы должны сделать вот что: 
// 1. У него есть метод под каждую конструкцию (почти)
// 2. Так же мы создадим наши AST ноды
// 3. Эти методы будут собирать AST ноды вьедино
// 4. Старт программы у нас будет с Root ноды


class Parser {
protected:
    const std::vector<Token>& _tokens;
    size_t _curr_token_pos = 0;

    /* Посмотреть текущий токен */
    Token Peek() const;

    /* Взять текущий токен и сдвинуть позицию на один вперед. Возвращает токен который он только что "сьел" */
    Token Advance();

    /* Проверить того ли типа текущий токен. Ничего не трогает */
    bool Check(TokenType type);

    /* Если текущий токен нужного типа, сьесть его и вернуть true. Иначе не трогать и вернуть false */
    bool Match(TokenType type);

    /* Если текущий токен нужного типа, то сьедаем. Иначе возвращаем ошибку. */
    /* Возвращает токен - тот, который проверил и пропустил вперед */
    Token Except(TokenType type);   

public:
    Parser(const std::vector<Token>& tokens)
        : _tokens(tokens) {}

    void ParseProgram();
};