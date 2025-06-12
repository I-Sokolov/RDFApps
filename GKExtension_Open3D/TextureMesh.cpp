
#include "pch.h"

extern void ReadFile(const char* filePathPLY)
{
    auto mesh = std::make_shared<open3d::geometry::TriangleMesh>();
    open3d::io::ReadTriangleMeshOptions opts;
    opts.enable_post_processing = true;

    if (!open3d::io::ReadTriangleMesh(filePathPLY, *mesh, opts)) {
        MessageBox (0, filePathPLY, "Failed to read PLY", MB_ICONSTOP);
        return;
    }

    std::cout << "Loaded: " << mesh->vertices_.size() << " verticies, "
        << mesh->triangles_.size() << " triangles" << std::endl;

    if (!mesh->triangle_uvs_.empty()) {
        std::cout << "UV coordinates are here" << std::endl;
    }
    else {
        std::cout << "No UV coordinates" << std::endl;
    }

    if (!mesh->textures_.empty()) {
        std::cout << "Loaded " << mesh->textures_.size() << " textures" << std::endl;
    }

}