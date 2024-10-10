#ifndef HANDLER_H
#define HANDLER_H

#include "lexer.h"
#include <string>
#include <vector>

class Handler {
public:
    Handler();

    // 打开并读取文件内容
    bool openFile(const std::string& filename);

    // 处理文件内容
    void processContent();

    // 获取处理后的词法单元
    const std::vector<Token>& getTokens() const;



private:
    // 打印词法单元列表
    void printTokens() const;

    // 将词法单元写入文件
    void printTokensToFile() const;
    
    std::string content;  // 存储文件内容
    std::vector<Token> tokens;  // 存储词法单元
};

#endif // HANDLER_H