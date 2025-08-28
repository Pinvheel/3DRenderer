#pragma once

#include "Triangle.hpp"
#include <vector>

struct Mesh { 
    std::vector<Triangle> tris; 

    bool loadObjectFromFile(std::string fileName);
};