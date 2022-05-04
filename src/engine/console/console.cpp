#include "console.h"

Console& Console::Get() {
    static Console Console;
    return Console;
}

Console::Console() {
     visible = false;
     cmdLine = new std::string("");
}

Console::~Console() {
     cmdLine->clear();
     delete cmdLine;
}

void Console::pushChar(char c) {
     if (cmdLine == nullptr) {
          cmdLine = new std::string("");
     }
     cmdLine->push_back(c);
}

void Console::popChar() {
     if (getCmdLine() != "") {
          cmdLine->pop_back();
     }
     else if (!cmdLine->empty()) {
          cmdLine->clear();
     }
}

void Console::pushString(std::string str) {
     //trim command archive if needed
     if (archive.size() > 32) {
          archive.pop_back();
     }
     archive.push_front(str);
     setCmdLine("");
}