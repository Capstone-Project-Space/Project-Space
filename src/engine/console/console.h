#pragma once

#include <queue>
#include <string>

class Console {
public:
     Console();
     ~Console();

     void setVisible(bool visible) { this->visible = visible; }
     bool getVisible() { return this->visible; }

     void setCmdLine(std::string cmdLine) { this->cmdLine = cmdLine; }
     std::string getCmdLine() { return cmdLine; }

     void setArchive(std::queue<std::string> archive) { this->archive = archive; }
     std::queue<std::string> getArchive() { return this->archive; }
     int getArchiveSize() { return this->archive.size(); }

     void pushChar(char);
     void popChar();

     void pushString(std::string);

private:
     bool visible;
     std::string cmdLine;
     std::queue<std::string> archive;
};