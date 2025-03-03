#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object_3D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateParalelipiped(const std::string& name, float length, float width, float height, glm::vec3 color);
    Mesh* CreateTree(const std::string& name);
    Mesh* CreateArrow(const std::string& name);
    Mesh* CreateTerrain(const std::string& name);
    Mesh* CreateFrame(const std::string& name);
    Mesh* CreateDropzone(const std::string& name);
    Mesh* CreateArrow(const std::string& name, glm::vec3 color);

}
#pragma once
