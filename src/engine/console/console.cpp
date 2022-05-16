#include "console.h"

Console& Console::Get() {
    static Console Console;
    return Console;
}

void Console::pushChar(char c) {
     cmdLine.push_back(c);
}

void Console::popChar() {
     if (!this->cmdLine.empty()) {
         cmdLine.pop_back();
     }
}

void Console::pushString(const std::string& str) {
     //trim command archive if needed
     if (archive.size() > 32) {
          archive.pop_back();
     }
     archive.push_front(str);
     this->cmdLine.clear();
}