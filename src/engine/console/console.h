#pragma once

#include <deque>
#include <string>

class Console {
public:
    static Console& Get();

     void setVisible(bool visible) { this->visible = visible; }
     bool getVisible() { return this->visible; }

     void setCmdLine(const std::string& cmdLine) {
          this->cmdLine.assign(cmdLine);
     }
     const std::string& getCmdLine() { return cmdLine; }

     void setArchive(const std::deque<std::string>& archive) { this->archive = archive; }
     const std::deque<std::string>& getArchive() { return this->archive; }
     int getArchiveSize() { return this->archive.size(); }
     const std::string& getArchiveAt(int i) { return this->archive.at(i); }

     void pushChar(char);
     void popChar();

     void pushString(const std::string&);

private:
     bool visible = false;
     std::string cmdLine;
     std::deque<std::string> archive;
};