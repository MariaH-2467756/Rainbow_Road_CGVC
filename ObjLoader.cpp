#include <string>
#define TINYOBJLOADER_IMPLEMENTATION
#include "includes/TinyObjLoader/tiny_obj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "includes/std_image/stb_image.h"
#include <iostream>
#include "ObjLoader.h"
#include <stdexcept>
#include <map>
#include <tuple>

MeshData ObjLoader::load(const std::string& objectFilepath, const char* textureFilePath) {
    tinyobj::ObjReaderConfig config;
    config.mtl_search_path = "./";

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(objectFilepath, config))
        throw std::runtime_error("ObjLoader: failed to load '" + objectFilepath + "': " + reader.Error());

    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();

    MeshData result;
    std::map<std::tuple<int,int,int>, unsigned int> vertices;

    for (const auto& shape : shapes) {
        for (const auto& idx : shape.mesh.indices) {
            auto key = std::make_tuple(idx.vertex_index, idx.normal_index, idx.texcoord_index);

            if (vertices.find(key) == vertices.end()) {
                Vertex v{};
                v.position = {
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]
                };
                if (idx.normal_index >= 0) {
                    v.normal = {
                        attrib.normals[3 * idx.normal_index + 0],
                        attrib.normals[3 * idx.normal_index + 1],
                        attrib.normals[3 * idx.normal_index + 2]
                    };
                }
                if (idx.texcoord_index >= 0) {
                    v.texCoords = {
                        attrib.texcoords[2 * idx.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]
                    };
                }
                vertices[key] = static_cast<unsigned int>(result.vertices.size());
                result.vertices.push_back(v);
            }
            result.indices.push_back(vertices[key]);
        }
    }

    loadTexture(textureFilePath, result.texture);
    return result;
}

void loadTexture(const char* textureFilePath, unsigned int& texture)
{
    // Load the texture.
    if (strlen(textureFilePath) == 0){
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        unsigned char whitePixel[] = {255, 255, 255, 255};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
    } else {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width, height, nrChannels;
        unsigned char *data = stbi_load(textureFilePath, &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cerr << "Failed to load texture";
        }
        stbi_image_free(data);
    }
}