#include "object.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object::CreateTerrain(
    const std::string &name,
    glm::vec3 leftUpperCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftUpperCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(0, -length, 0), color),
        VertexFormat(corner + glm::vec3(length, -length, 0), color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color)
    };

    Mesh* terrain = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 2, 0 };

    terrain->InitFromData(vertices, indices);
    return terrain;
}

Mesh* object::CreateTank(
    const std::string& name,
    glm::vec3 primaryColor,
    glm::vec3 secondaryColor)
{
    float radius = 15;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(30, 10, 0), secondaryColor), // 0
        VertexFormat(glm::vec3(-30, 10, 0), secondaryColor), // 1
        VertexFormat(glm::vec3(-16, 0, 0), secondaryColor), // 2
        VertexFormat(glm::vec3(16, 0, 0), secondaryColor), // 3

        VertexFormat(glm::vec3(40, 10, 0), primaryColor), // 4
        VertexFormat(glm::vec3(-40, 10, 0), primaryColor), // 5
        VertexFormat(glm::vec3(-31, 20, 0), primaryColor), // 6
        VertexFormat(glm::vec3(31, 20, 0), primaryColor) // 7
    };

    vertices.push_back(VertexFormat(glm::vec3(0, 20, 0), primaryColor));
    for (int i = 0; i <= 50; i++) {
        float angle = i * AI_MATH_PI / 50;
        vertices.push_back(VertexFormat(glm::vec3(radius * cos(angle), 20 + radius * sin(angle), 0), primaryColor));
    }

    Mesh* tank = new Mesh(name);
    std::vector<unsigned int> indices = { 
        0, 1, 2, 
        2, 3, 0,
        7, 6, 5,
        5, 4, 7
    };

    for (int i = 0; i < 50; i++) {
        indices.push_back(8);
        indices.push_back(9 + i);
        indices.push_back(10 + i);
    }

    tank->SetDrawMode(GL_TRIANGLES);
    tank->InitFromData(vertices, indices);
    return tank;
}

Mesh* object::CreateSquare(
    const std::string& name,
    glm::vec3 color)
{

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0.5, 1, 0), color), // 0
        VertexFormat(glm::vec3(-0.5, 1, 0), color), // 1
        VertexFormat(glm::vec3(-0.5, 0, 0), color), // 2
        VertexFormat(glm::vec3(0.5, 0, 0), color), // 3
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    square->SetDrawMode(GL_TRIANGLES);
    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object::CreateLifebarFrame(
    const std::string& name,
    float length,
    float height)
{
    float width = 4;
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(length / 2 + width / 2, height, 0), glm::vec3(1, 1, 1)), 
        VertexFormat(glm::vec3(-length / 2 - width / 2, height, 0), glm::vec3(1, 1, 1)),
        VertexFormat(glm::vec3(-length / 2, height, 0), glm::vec3(1, 1, 1)),
        VertexFormat(glm::vec3(-length / 2, 0, 0), glm::vec3(1, 1, 1)),
        VertexFormat(glm::vec3(-length / 2 - width / 2, 0, 0), glm::vec3(1, 1, 1)),
        VertexFormat(glm::vec3(length / 2 + width / 2, 0, 0), glm::vec3(1, 1, 1)),
        VertexFormat(glm::vec3(length / 2, 0, 0), glm::vec3(1, 1, 1)),
        VertexFormat(glm::vec3(length / 2, height, 0), glm::vec3(1, 1, 1)),
    };

    Mesh* frame = new Mesh(name);
    std::vector<unsigned int> indices = {
        0, 1,
        2, 3,
        4, 5,
        6, 7,
    };

    frame->SetDrawMode(GL_LINES);
    frame->InitFromData(vertices, indices);

    glLineWidth(width);

    return frame;
}

Mesh* object::CreateProjectile(
    const std::string& name)
{
    float radius = 5;
    std::vector<VertexFormat> vertices = { 
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1))
    };
    std::vector<unsigned int> indices = { 0 };

    for (int i = 0; i <= 100; i++) {
        float angle = i * AI_MATH_TWO_PI / 100;
        vertices.push_back(VertexFormat(glm::vec3(radius * cos(angle), radius * sin(angle), 0), glm::vec3(1, 1, 1)));
        indices.push_back(i + 1);
    }

    Mesh* projectile = new Mesh(name);

    projectile->SetDrawMode(GL_TRIANGLE_FAN);
    projectile->InitFromData(vertices, indices);

    return projectile;
}

Mesh* object::CreateLine(
    const std::string& name,
    const float lineLen)
{
    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)),
        VertexFormat(glm::vec3(lineLen, 0, 0), glm::vec3(0, 0, 0))
    };

    std::vector<unsigned int> indices = { 0, 1 };
    
    Mesh* line = new Mesh(name);

    line->SetDrawMode(GL_LINES);
    line->InitFromData(vertices, indices);

    glLineWidth(4);

    return line;
}
