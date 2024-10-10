#include "handler.h"
#include "lexer.h"
#include <fstream>
#include <iostream>
#include <sstream>

Handler::Handler() {}

bool Handler::openFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
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
        std::cerr << "Lexical analysis error: " << e.what() << std::endl;
        // 可以选择在这里清空tokens或者保留已经识别的部分
        // tokens.clear();
    }
    printTokens();
    printTokensToFile();
}

const std::vector<Token>& Handler::getTokens() const {
    return tokens;
}

void Handler::printTokens() const {
    for (const auto& token : tokens) {
        std::cout << "Type: " << getTokenTypeName(token.type)
                  << ", Lexeme: " << token.lexeme
                  << ", Line: " << token.line
                  << ", Column: " << token.column << std::endl;
    }

}

void Handler::printTokensToFile() const {
    std::ofstream outFile("output/tokens.txt");
    if (!outFile.is_open()) {
        std::cerr << "Unable to open output file: tokens.txt" << std::endl;
        return;
    }

    for (const auto& token : tokens) {
        outFile << "<" << getTokenTypeName(token.type) << ", "
                << token.lexeme << ">" << std::endl;
    }

    outFile.close();
}   

