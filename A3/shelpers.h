#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

// Define the Command structure to store the information required for command execution
struct Command {
    std::string execName; // command name
    std::vector<const char*> argv; // Command parameters, including the command itself and the ending nullptr
    int inputFd = STDIN_FILENO; // Input file descriptor, defaults to standard input
    int outputFd = STDOUT_FILENO; // Output file descriptor, defaults to standard output
    bool background = false; // Whether to execute the command in the background

    Command() : inputFd(STDIN_FILENO), outputFd(STDOUT_FILENO), background(false) {}
};

// Parses the command line string and returns a vector containing all words
std::vector<std::string> tokenize(const std::string& s);

// Constructs a list of Command objects based on the provided tokens
std::vector<Command> getCommands(const std::vector<std::string>& tokens);

// Overloaded output operator for printing Command objects
std::ostream& operator<<(std::ostream& outs, const Command& c);

// Split strings containing special symbols
bool splitOnSymbol(std::vector<std::string>& words, int i, char c);
