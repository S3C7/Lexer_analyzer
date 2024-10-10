#include "handler.h"
#include "lexer.h"
#include <fstream>
#include <iostream>
#include <sstream>

Handler::Handler() {}

bool Handler::openFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    content = buffer.str();
    return true;
}

void Handler::processContent() {
    Lexer lexer(content);
    tokens.clear();

    try {
        while (true) {
            Token token = lexer.getNextToken();
            tokens.push_back(token);

            if (token.type == TokenType::END_OF_FILE) {
                break;
            }
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "词法分析错误: " << e.what() << std::endl;
        // 可以选择在这里清空tokens或者保留已经识别的部分
        // tokens.clear();
    }

    printTokens();
}

const std::vector<Token>& Handler::getTokens() const {
    return tokens;
}

void Handler::printTokens() const {
    for (const auto& token : tokens) {
        std::cout << "类型: " << getTokenTypeName(token.type)
                  << ", 词素: " << token.lexeme
                  << ", 行: " << token.line
                  << ", 列: " << token.column << std::endl;
    }
}
