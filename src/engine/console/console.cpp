#include "console.h"

Console::Console() {
     visible = false;
     cmdLine = "";
     archive.empty();
}

Console::~Console() {
     cmdLine.clear();
}

void Console::pushChar(char c) {
     if (cmdLine.empty()) {
          cmdLine = "";
     }
     setCmdLine(getCmdLine() + c);
}

void Console::popChar() {
     if (getCmdLine() != "") {
          cmdLine.pop_back();
     }
     else if (!cmdLine.empty()) {
          cmdLine.clear();
     }
}

void Console::pushString(std::string str) {
     //trim command archive if needed
     if (archive.size() > 32) {
          archive.pop();
     }
     archive.push(str);
     setCmdLine("");
}