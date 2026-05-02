#pragma once

#include "Mesh.h"
#include <memory>
#include <string>

class ObjLoader {
public:
  MeshData load(const std::string &objectFilepath,
                const char *textureFilePath = "");
};

void loadTexture(const char *textureFilePath, unsigned int &texture,
                 bool verticalFlip = false);