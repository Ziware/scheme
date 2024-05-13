#include "scheme.h"
#include "error.h"
#include "help.h"
#include "constants.h"

#include <cctype>
#include <cstdio>
#include <chrono>
#include <exception>
#include <iostream>
#include <string>

std::string RemoveSpaces(const std::string& s) {
    std::string ans;
    for (const auto& c : s) {
        if (!isspace(c)) {
            ans.push_back(tolower(c));
        }
    }
    return ans;
}

BeautyHelp help;

void HelpMessage() {
    std::cout << "\n";
    std::cout << "Type \'!benchon\' to turn in benching mode and \'!benchoff\' to disable it\n";
    std::cout << "Type \'!help\' to see help message\n";
    std::cout << "Type \'!exit\' to end interpreter work\n";
    std::cout << "\n";
    std::cout << help.GetHelp();
    std::cout << "\n\n";
}

void Init() {
    help = BeautyHelp();
}

bool benching = false;

int main() {
    Init();
    std::cout << "Scheme console interpreter implementation\nType \'!help\' to learn more\n\n";
    Interpreter scheme;
    std::string line;
    std::cout << "=> ";
    while (std::getline(std::cin, line)) {
        if (RemoveSpaces(line)[0] == '!') {
            if (RemoveSpaces(line) == kHelp) {
                HelpMessage();
            } else if (RemoveSpaces(line) == kBenchOn) {
                benching = true;
                std::cout << "Benching mode: ON\n";
            } else if (RemoveSpaces(line) == kBenchOff) {
                benching = false;
                std::cout << "Benching mode: OFF\n";
            } else if (RemoveSpaces(line) == kExit) {
                exit(0);
            }
            std::cout << "=> ";
            continue;
        }
        const auto start = std::chrono::steady_clock::now();
        try {
            std::cout << scheme.Run(line) << '\n';
        } catch (SyntaxError& ex) {
            std::cout << "Syntax Error: " << ex.what() << '\n';
        } catch (RuntimeError& ex) {
            std::cout << "Runtime Error: " << ex.what() << '\n';
        } catch (NameError& ex) {
            std::cout << "Name Error: " << ex.what() << '\n';
        } catch (HelpError& ex) {
            std::cout << "Help Error: " << ex.what() << '\n';
        } catch(std::exception& ex) {
            std::cout << "Unexpected Error: " << ex.what() << '\n';
        } catch(...) {
            std::cout << "Unexpected Error\n";
        }
        const auto end = std::chrono::steady_clock::now();
        if (benching) {
            std::cout << "Estimated time is : " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << '\n';
        }

        std::cout << "=> ";
    }
    return 0;
}
