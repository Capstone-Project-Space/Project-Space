#pragma once
#ifndef BODY_SYSTEM_H
#define BODY_SYSTEM_H

#include <vector>

#include "body.h"
#include "../randgen/randomgen.h"

class body_system {
public:
     std::string         name;
     std::vector<body*>  suns;
     std::vector<body*>  bodies;
     int                 body_count;
     int                 sun_count;
     bool                centralSun;
     float               systemRange;

     body_system(std::string name, int sun_count, int body_count);
     ~body_system();

     void GenerateSystem();
     void Restart();
};

std::string intToNumeral(int x);
char* StrToChar(std::string str);
#endif