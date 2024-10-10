#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <unordered_map>

// 词法单元类型定义
enum class TokenType {
    // 关键字
    IF, THEN, ELSE,
    // 标点符号
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    // 句子界符
    SEMICOLON,
    // 普通界符
    COMMA,
    // 保留字
    PRINT,
    // 标识符
    IDENTIFIER,
    // 运算符
    PLUS, MINUS, MUL, DIV, EQUAL,
    // 字面量
    INTEGER, FLOAT, STRING,
    // 其他
    END_OF_FILE
};

// Token 结构体定义
struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
};

class Lexer {
public:
    explicit Lexer(const std::string& source);
    Token getNextToken();

private:
    std::string source;
    size_t position;
    int line;
    int column;

    static const std::unordered_map<std::string, TokenType> keywords;
    static const std::unordered_map<std::string, TokenType> operators;

    // 辅助方法
    char getCurrentChar() const;
    char advance();
    bool isAtEnd() const;
    void skipWhitespace();

    // Token 扫描方法
    Token scanIdentifier();
    Token scanNumber();
    Token scanString();
    bool isAlpha(char c) const;
    bool isDigit(char c) const;
    bool isAlphaNumeric(char c) const;
};

// 用于调试和输出的辅助函数
std::string getTokenTypeName(TokenType type);

#endif // LEXER_H
