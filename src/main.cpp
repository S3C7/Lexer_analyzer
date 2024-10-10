#include <iostream>
#include <string>
#include "handler.h"
#include "lexer.h"

int main() {
    Handler handler;
    handler.openFile("input.txt");
    handler.processContent();
    return 0;
}