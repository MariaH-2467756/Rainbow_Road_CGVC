#define TINYOBJLOADER_IMPLEMENTATION
#include "includes/TinyObjLoader/tiny_obj_loader.h"
#include "ObjLoader.h"
#include <stdexcept>
#include <map>
#include <tuple>

MeshData ObjLoader::load(const std::string& filepath) {
    tinyobj::ObjReaderConfig config;
    config.mtl_search_path = "./";

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(filepath, config))
        throw std::runtime_error("ObjLoader: failed to load '" + filepath + "': " + reader.Error());

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
    return result;
}