#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateTerrain(const std::string &name, glm::vec3 leftUpperCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateTank(const std::string& name, glm::vec3 primaryColor, glm::vec3 secondaryColor);
    Mesh* CreateSquare(const std::string& name, glm::vec3 color);
    Mesh* CreateLifebarFrame(const std::string& name, float length, float height);
    Mesh* CreateProjectile(const std::string& name);
    Mesh* CreateLine(const std::string& name, const float lineLen);

}
