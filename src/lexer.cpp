#include "lexer.h"
#include <cctype>
#include <stdexcept>

// 初始化静态成员变量
const std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"if", TokenType::IF},
    {"then", TokenType::THEN},
    {"else", TokenType::ELSE}
};

const std::unordered_map<std::string, TokenType> Lexer::operators = {
    {"+", TokenType::PLUS},
    {"-", TokenType::MINUS},
    {"*", TokenType::MUL},
    {"/", TokenType::DIV},
    {"=", TokenType::EQUAL}
};

Lexer::Lexer(const std::string& source) : source(source), position(0), line(1), column(0) {}

Token Lexer::getNextToken() {
    skipWhitespace();
    
    if (isAtEnd()) {
        return Token{TokenType::END_OF_FILE, "", line, column};
    }

    char current = getCurrentChar();
    int startColumn = column;

    if (current == '"') {
        return scanString();
    }

    if (isAlpha(current)) {
        return scanIdentifier();
    }

    if (isDigit(current)) {
        return scanNumber();
    }

    switch (current) {
        case '(': advance(); return Token{TokenType::LEFT_PAREN, "(", line, startColumn};
        case ')': advance(); return Token{TokenType::RIGHT_PAREN, ")", line, startColumn};
        case '{': advance(); return Token{TokenType::LEFT_BRACE, "{", line, startColumn};
        case '}': advance(); return Token{TokenType::RIGHT_BRACE, "}", line, startColumn};
        case ';': advance(); return Token{TokenType::SEMICOLON, ";", line, startColumn};
        case ',': advance(); return Token{TokenType::COMMA, ",", line, startColumn};
    }

    std::string op(1, current);
    advance();
    auto it = operators.find(op);
    if (it != operators.end()) {
        return Token{it->second, op, line, startColumn};
    }

    throw std::runtime_error("Unexpected character: " + std::string(1, current));
}

Token Lexer::scanIdentifier() {
    int startColumn = column;
    std::string lexeme;
    while (!isAtEnd() && isAlphaNumeric(getCurrentChar())) {
        lexeme += advance();
    }

    auto it = keywords.find(lexeme);
    if (it != keywords.end()) {
        return Token{it->second, lexeme, line, startColumn};
    }

    return Token{TokenType::IDENTIFIER, lexeme, line, startColumn};
}

Token Lexer::scanNumber() {
    int startColumn = column;
    std::string lexeme;
    bool isFloat = false;

    while (!isAtEnd() && (isDigit(getCurrentChar()) || getCurrentChar() == '.')) {
        if (getCurrentChar() == '.') {
            if (isFloat) {
                throw std::runtime_error("Invalid number format: multiple decimal points");
            }
            isFloat = true;
        }
        lexeme += advance();
    }

    TokenType type = isFloat ? TokenType::FLOAT : TokenType::INTEGER;
    return Token{type, lexeme, line, startColumn};
}

Token Lexer::scanString() {
    int startColumn = column;
    std::string lexeme = "\"";
    advance(); // 跳过开始的双引号

    while (!isAtEnd() && getCurrentChar() != '"') {
        if (getCurrentChar() == '\\') {
            lexeme += advance(); // 添加转义字符
        }
        lexeme += advance();
    }

    if (isAtEnd()) {
        throw std::runtime_error("Unterminated string at line " + std::to_string(line) + ", column " + std::to_string(startColumn));
    }

    lexeme += advance(); // 添加结束的双引号

    return Token{TokenType::STRING, lexeme, line, startColumn};
}

char Lexer::getCurrentChar() const {
    return isAtEnd() ? '\0' : source[position];
}

char Lexer::advance() {
    char current = getCurrentChar();
    position++;
    column++;
    if (current == '\n') {
        line++;
        column = 0;
    }
    return current;
}

bool Lexer::isAtEnd() const {
    return position >= source.length();
}

void Lexer::skipWhitespace() {
    while (!isAtEnd() && std::isspace(getCurrentChar())) {
        advance();
    }
}

bool Lexer::isAlpha(char c) const {
    return std::isalpha(c) || c == '_';
}

bool Lexer::isDigit(char c) const {
    return std::isdigit(c);
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

std::string getTokenTypeName(TokenType type) {
    switch (type) {
        case TokenType::IF: return "IF";
        case TokenType::THEN: return "THEN";
        case TokenType::ELSE: return "ELSE";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::PRINT: return "PRINT";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MUL: return "MUL";
        case TokenType::DIV: return "DIV";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::STRING: return "STRING";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        default: return "UNKNOWN";
    }
}