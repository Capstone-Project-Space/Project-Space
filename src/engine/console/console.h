#pragma once

#include <deque>
#include <string>

class Console {
public:
     Console();
     ~Console();

     void setVisible(bool visible) { this->visible = visible; }
     bool getVisible() { return this->visible; }

     void setCmdLine(std::string cmdLine) {
          if (this->cmdLine == nullptr) {
               this->cmdLine = new std::string("");
          }
          this->cmdLine->assign(cmdLine);
     }
     std::string getCmdLine() { return *cmdLine; }

     void setArchive(std::deque<std::string> archive) { this->archive = archive; }
     std::deque<std::string> getArchive() { return this->archive; }
     int getArchiveSize() { return this->archive.size(); }
     std::string getArchiveAt(int i) { return this->archive.at(i); }

     void pushChar(char);
     void popChar();

     void pushString(std::string);

private:
     bool visible;
     std::string* cmdLine;
     std::deque<std::string> archive;
};