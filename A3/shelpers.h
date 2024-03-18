#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

// 定义 Command 结构体，用于存储命令执行所需的信息
struct Command {
    std::string execName; // 命令名
    std::vector<const char*> argv; // 命令参数，包括命令本身和结束的 nullptr
    int inputFd = STDIN_FILENO; // 输入文件描述符，默认为标准输入
    int outputFd = STDOUT_FILENO; // 输出文件描述符，默认为标准输出
    bool background = false; // 是否在后台执行命令

    Command() : inputFd(STDIN_FILENO), outputFd(STDOUT_FILENO), background(false) {}
};

// 解析命令行字符串，返回一个包含所有单词的 vector
std::vector<std::string> tokenize(const std::string& s);

// 根据提供的 tokens 构造 Command 对象的列表
std::vector<Command> getCommands(const std::vector<std::string>& tokens);

// 重载输出运算符，用于打印 Command 对象
std::ostream& operator<<(std::ostream& outs, const Command& c);

// 分割含有特殊符号的字符串
bool splitOnSymbol(std::vector<std::string>& words, int i, char c);
