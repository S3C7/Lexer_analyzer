#include "lexer.h"
#include <cctype>
#include <stdexcept>

// 初始化静态成员变量
const std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"if", TokenType::IF},
    {"then", TokenType::THEN},
    {"else", TokenType::ELSE},
    {"int", TokenType::INT},
    {"float", TokenType::FLOAT},
    {"string", TokenType::STRING},
    {"print", TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"void", TokenType::VOID},
    {"while", TokenType::WHILE},
    {"for", TokenType::FOR},
    {"main", TokenType::MAIN},
};

const std::unordered_map<std::string, TokenType> Lexer::operators = {
    {"+", TokenType::PLUS},
    {"-", TokenType::MINUS},
    {"*", TokenType::MUL},
    {"/", TokenType::DIV},
    {"=", TokenType::EQUAL},
    {"<", TokenType::LESS},
    {">", TokenType::GREATER},
    {"<=", TokenType::LEQ},
    {">=", TokenType::GEQ}
};

Lexer::Lexer(const std::string& source) : source(source), position(0), line(1), column(0) {}

Token Lexer::scanSingleCharToken() {
    char current = getCurrentChar();
    int startColumn = column;
    switch (current) {
        case '(': advance(); return Token{TokenType::LEFT_PAREN, "(", line, startColumn};
        case ')': advance(); return Token{TokenType::RIGHT_PAREN, ")", line, startColumn};
        case '{': advance(); return Token{TokenType::LEFT_BRACE, "{", line, startColumn};
        case '}': advance(); return Token{TokenType::RIGHT_BRACE, "}", line, startColumn};
        case ';': advance(); return Token{TokenType::SEMICOLON, ";", line, startColumn};
        case ',': advance(); return Token{TokenType::COMMA, ",", line, startColumn};
        default: return Token{TokenType::END_OF_FILE, "", line, startColumn}; // 表示没有匹配到单字符标记
    }
}

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

    Token singleCharToken = scanSingleCharToken();
    if (singleCharToken.type != TokenType::END_OF_FILE) {
        return singleCharToken;
    }

    // 处理 < 和 > 运算符
    switch (current) {
        case '<':
            if (peekNext() == '=') {
                advance(); advance();
                return Token{TokenType::LEQ, "<=", line, startColumn};
            } else {
                advance();
                return Token{TokenType::LESS, "<", line, startColumn};
            }
        case '>':
            if (peekNext() == '=') {
                advance(); advance();
                return Token{TokenType::GEQ, ">=", line, startColumn};
            } else {
                advance();
                return Token{TokenType::GREATER, ">", line, startColumn};
            }
    }

    // 处理其他运算符
    std::string op(1, current);
    advance();
    auto it = operators.find(op);
    if (it != operators.end()) {
        return Token{it->second, op, line, startColumn};
    }

    throw std::runtime_error("Unexpected character at line " + std::to_string(line) + 
                             ", column " + std::to_string(startColumn) + 
                             ": " + std::string(1, current));
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
        throw std::runtime_error("Unterminated string at line " + std::to_string(line) 
                + ", column " + std::to_string(startColumn));
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

char Lexer::peekNext() const {
    if (position + 1 >= source.length()) {
        return '\0';  // 如果已经到达字符串末尾，返回空字符
    }
    return source[position + 1];  // 返回下一个字符，但不改变位置
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
        case TokenType::INT: return "INT";
        case TokenType::PRINT: return "PRINT";
        case TokenType::RETURN: return "RETURN";
        case TokenType::VOID: return "VOID";
        case TokenType::WHILE: return "WHILE";
        case TokenType::FOR: return "FOR";
        case TokenType::MAIN: return "MAIN";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MUL: return "MUL";
        case TokenType::DIV: return "DIV";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::GREATER: return "GREATER";
        case TokenType::LEQ: return "LEQ";
        case TokenType::GEQ: return "GEQ";
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::STRING: return "STRING";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        default: return "UNKNOWN";
    }
}