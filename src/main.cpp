#include <iostream>
#include <string>
#include "handler.h"
#include "lexer.h"

int main() {
    //std::string source = "if (x + y) then z = x * y else z = x / y; print (\"hello world\");";
    // Lexer lexer(source);

    // Token token;
    // do {
    //     token = lexer.getNextToken();
    //     std::cout << "<" << getTokenTypeName(token.type) << ", " << token.lexeme << ">" << std::endl;
    // } while (token.type != TokenType::END_OF_FILE);

    Handler handler;
    handler.openFile("input.txt");
    handler.processContent();
    return 0;
}