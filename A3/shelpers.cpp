#include "shelpers.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib> // 为了使用 getenv

std::vector<std::string> tokenize(const std::string& s) {
    std::vector<std::string> ret;
    int pos = 0;
    int space;

    while ((space = s.find(' ', pos)) != std::string::npos) {
        std::string word = s.substr(pos, space - pos);
        if (!word.empty()) {
            // 检查环境变量
            if (word[0] == '$') {
                const char* envValue = getenv(word.substr(1).c_str());
                if (envValue) {
                    word = envValue;
                }
            }
            ret.push_back(word);
        }
        pos = space + 1;
    }

    // 处理最后一个词
    std::string lastWord = s.substr(pos, s.size() - pos);
    if (!lastWord.empty()) {
        // 检查环境变量
        if (lastWord[0] == '$') {
            const char* envValue = getenv(lastWord.substr(1).c_str());
            if (envValue) {
                lastWord = envValue;
            }
        }
        ret.push_back(lastWord);
    }

    // 这里是之前的代码，处理特殊符号
    for (int i = 0; i < ret.size(); ++i) {
        for (char c : {'&', '<', '>', '|'}) {
            if (splitOnSymbol(ret, i, c)) {
                --i;
                break;
            }
        }
    }
    return ret;
}


bool splitOnSymbol(std::vector<std::string>& words, int i, char c) {
    if (words[i].size() < 2) {
        return false;
    }
    int pos;
    if ((pos = words[i].find(c)) != std::string::npos) {
        if (pos == 0) {
            // Starts with symbol
            words.insert(words.begin() + i + 1, words[i].substr(1, words[i].size() - 1));
            words[i] = words[i].substr(0, 1);
        } else {
            // Symbol in middle or end
            words.insert(words.begin() + i + 1, std::string{c});
            std::string after = words[i].substr(pos + 1, words[i].size() - pos - 1);
            if (!after.empty()) {
                words.insert(words.begin() + i + 2, after);
            }
            words[i] = words[i].substr(0, pos);
        }
        return true;
    }
    return false;
}

std::vector<Command> getCommands(const std::vector<std::string>& tokens) {
    std::vector<Command> commands;
    Command currentCommand;

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == ">" || tokens[i] == "<") {
            if (i + 1 >= tokens.size()) {
                std::cerr << "Syntax error near unexpected token `" << tokens[i] << "'\n";
                return {}; // Return empty command list on error
            }
            int fd;
            if (tokens[i] == ">") {
                fd = open(tokens[i + 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (fd < 0) {
                    perror("open");
                    return {}; // Error handling
                }
                currentCommand.outputFd = fd;
            } else if (tokens[i] == "<") {
                fd = open(tokens[i + 1].c_str(), O_RDONLY);
                if (fd < 0) {
                    perror("open");
                    return {}; // Error handling
                }
                currentCommand.inputFd = fd;
            }
            i++; // Skip filename
        } else if (tokens[i] == "|") {
            if (!currentCommand.execName.empty()) {
                currentCommand.argv.push_back(nullptr);
                commands.push_back(currentCommand);
            }
            currentCommand = Command(); // Start a new command
        } else if (tokens[i] == "&") {
            currentCommand.background = true;
        } else {
            // Handle normal command and arguments
            if (currentCommand.execName.empty()) {
                currentCommand.execName = tokens[i];
            }
            currentCommand.argv.push_back(tokens[i].c_str());
        }
    }
    if (!currentCommand.execName.empty()) {
        currentCommand.argv.push_back(nullptr); // Ensure argv ends with nullptr
        commands.push_back(currentCommand);
    }
    return commands;
}

std::ostream& operator<<(std::ostream& outs, const Command& c) {
    outs << c.execName << " [argv: ";
    for (const auto& arg : c.argv) {
        if (arg) {
            outs << arg << ' ';
        }
        else {
            outs << "NULL ";
        }
    }
    outs << "] -- FD, in: " << c.inputFd << ", out: " << c.outputFd << " "
         << (c.background ? "(background)" : "(foreground)");
    return outs;
}
