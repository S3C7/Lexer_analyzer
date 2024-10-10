#include <iostream>
#include <string>
#include "lexer.h"  

int main() {
    std::string source = "if (x + y) then z = x * y else z = x / y";
    Lexer lexer(source);

    Token token;
    do {
        token = lexer.getNextToken();
        std::cout << "<" << getTokenTypeName(token.type) << ", " << token.lexeme << ">" << std::endl;
    } while (token.type != TokenType::END_OF_FILE);

    return 0;
}