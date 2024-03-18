#include "shelpers.h"
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <cstdlib> // 为 getenv 和 setenv 添加

int main() {
    std::string line;
    while (std::cout << "> " && std::getline(std::cin, line)) {
        if (line == "exit") {
            break;
        }

        // Check for variable assignment
        size_t eqPos = line.find('=');
        if (eqPos != std::string::npos && line[0] != '$') {
            std::string varName = line.substr(0, eqPos);
            std::string varValue = line.substr(eqPos + 1);
            // Set environment variable using setenv
            if (setenv(varName.c_str(), varValue.c_str(), 1) == -1) {
                perror("setenv");
            }
            continue;
        }

        auto tokens = tokenize(line);
        auto commands = getCommands(tokens);

        if (commands.empty()) continue; // 忽略空命令

        for (size_t i = 0; i < commands.size(); ++i) {
            auto& command = commands[i];

            // Handle built-in commands
            if (command.execName == "cd") {
                if (command.argv.size() >= 2) {
                    if (chdir(command.argv[1]) != 0) {
                        perror("cd");
                    }
                } else {
                    std::cerr << "cd: missing operand" << std::endl;
                }
                continue;
            }

            // Setup pipe
            int pipefd[2];
            if (i < commands.size() - 1) {
                if (pipe(pipefd) == -1) {
                    perror("pipe");
                    continue;
                }
                command.outputFd = pipefd[1];
                commands[i + 1].inputFd = pipefd[0];
            }

            pid_t pid = fork();
            if (pid == 0) { // 子进程
                // 输入重定向
                if (command.inputFd != STDIN_FILENO) {
                    dup2(command.inputFd, STDIN_FILENO);
                    close(command.inputFd);
                }

                // 输出重定向
                if (command.outputFd != STDOUT_FILENO) {
                    dup2(command.outputFd, STDOUT_FILENO);
                    close(command.outputFd);
                }

                // Execute command
                std::vector<char*> argv;
                for (auto& arg : command.argv) {
                    argv.push_back(const_cast<char*>(arg));
                }
                argv.push_back(nullptr); // 确保以nullptr结尾

                execvp(command.execName.c_str(), argv.data());
                perror("execvp");
                exit(EXIT_FAILURE);
            } else if (pid > 0) { // 父进程
                int status;
                if (!command.background) {
                    waitpid(pid, &status, 0);
                } else {
                    std::cout << "Started background job " << pid << std::endl;
                }

                // 关闭未使用的管道端
                if (i < commands.size() - 1) {
                    close(pipefd[1]); // 关闭写端，因为子进程已经复制了
                }
                if (i > 0) {
                    close(commands[i-1].inputFd); // 关闭前一个命令的输入端
                }
            } else {
                perror("fork");
                exit(EXIT_FAILURE);
            }
        }
    }
    return 0;
}
