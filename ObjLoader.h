#pragma once

#include "Mesh.h"
#include <string>

class ObjLoader {
public:
    static MeshData load(const std::string& filepath);
};