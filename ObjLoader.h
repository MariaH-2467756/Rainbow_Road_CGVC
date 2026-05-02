#pragma once

#include "Mesh.h"
#include <string>
#include <memory>

class ObjLoader {
public:
    MeshData load(const std::string& objectFilepath, const char* textureFilePath = "");

};

void loadTexture(const char* textureFilePath, unsigned int& texture, bool verticalFlip = false);