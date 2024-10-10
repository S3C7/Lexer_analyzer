#include "lexer.h"
#include <cctype>
#include <stdexcept>

// ... 其他方法的实现 ...

Token Lexer::getNextToken() {
    skipWhitespace();
    
    if (isAtEnd()) {
        return Token{TokenType::END_OF_FILE, "", line, column};
    }

    int startColumn = column;
    std::string lexeme;

    while (!isAtEnd() && !std::isspace(getCurrentChar())) {
        lexeme += advance();
    }

    return identifyToken(lexeme);
}

Token Lexer::identifyToken(const std::string& lexeme) {
    // 检查是否是关键字
    auto keywordIt = keywords.find(lexeme);
    if (keywordIt != keywords.end()) {
        return Token{keywordIt->second, lexeme, line, column - static_cast<int>(lexeme.length())};
    }

    // 检查是否是操作符
    auto operatorIt = operators.find(lexeme);
    if (operatorIt != operators.end()) {
        return Token{operatorIt->second, lexeme, line, column - static_cast<int>(lexeme.length())};
    }

    // 检查是否是数字
    if (std::isdigit(lexeme[0])) {
        // 这里可以添加更复杂的数字识别逻辑，包括浮点数
        return Token{TokenType::INTEGER, lexeme, line, column - static_cast<int>(lexeme.length())};
    }

    // 如果不是关键字、操作符或数字，则视为标识符
    if (isAlpha(lexeme[0])) {
        return Token{TokenType::IDENTIFIER, lexeme, line, column - static_cast<int>(lexeme.length())};
    }

    // 如果都不是，可以抛出异常或返回一个错误token
    throw std::runtime_error("Unexpected token: " + lexeme);
}

void Lexer::skipWhitespace() {
    while (!isAtEnd() && std::isspace(getCurrentChar())) {
        advance();
    }
}

// ... 其他辅助方法的实现 ...