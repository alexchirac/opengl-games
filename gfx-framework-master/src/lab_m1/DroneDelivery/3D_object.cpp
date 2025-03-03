#include "3D_object.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object_3D::CreateParalelipiped(
    const std::string& name,
    float length,
    float width,
    float height,
    glm::vec3 color)
{
    float half_len = length / 2;
    float half_width = width / 2;
    float half_height = height / 2;
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(-half_len, -half_height, -half_width), color),
        VertexFormat(glm::vec3(half_len, -half_height, -half_width), color),
        VertexFormat(glm::vec3(-half_len, half_height, -half_width), color),
        VertexFormat(glm::vec3(half_len, half_height, -half_width), color),
        VertexFormat(glm::vec3(-half_len, -half_height, half_width), color),
        VertexFormat(glm::vec3(half_len, -half_height, half_width), color),
        VertexFormat(glm::vec3(-half_len, half_height, half_width), color),
        VertexFormat(glm::vec3(half_len, half_height, half_width), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 
        0, 1, 2, 
        1, 3, 2,
        2, 3, 7,
        2, 7, 6,
        1, 7, 3,
        1, 5, 7,
        6, 7, 4,
        7, 5, 4,
        0, 4, 1,
        1, 4, 5,
        2, 6, 4,
        0, 2, 4,
    };

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object_3D::CreateTree(
    const std::string& name)
{
    glm::vec3 trunkColor = glm::vec3(0.239, 0.251, 0.125);
    glm::vec3 leafColor = glm::vec3(0.271, 0.612, 0.227);

    std::vector<VertexFormat> vertices;
    float trunkRadius = 0.3;
    float firstLevelRadius = 1.5;
    float secondLevelRadius = 1.2;
    float thirdLevelRadius = 0.8;


    for (int i = 0; i < 100; i++) {
        vertices.push_back(VertexFormat(glm::vec3(trunkRadius * cos(AI_MATH_TWO_PI * i / 100), 0, trunkRadius * sin(AI_MATH_TWO_PI * i / 100)), trunkColor));
    }
    for (int i = 0; i < 100; i++) {
        vertices.push_back(VertexFormat(glm::vec3(trunkRadius * cos(AI_MATH_TWO_PI * i / 100), -20, trunkRadius * sin(AI_MATH_TWO_PI * i / 100)), trunkColor));
    }
    for (int i = 0; i < 100; i++) {
        vertices.push_back(VertexFormat(glm::vec3(firstLevelRadius * cos(AI_MATH_TWO_PI * i / 100), 0, firstLevelRadius * sin(AI_MATH_TWO_PI * i / 100)), leafColor));
    }
    for (int i = 0; i < 100; i++) {
        vertices.push_back(VertexFormat(glm::vec3(secondLevelRadius * cos(AI_MATH_TWO_PI * i / 100), 0.5, secondLevelRadius * sin(AI_MATH_TWO_PI * i / 100)), leafColor));
    }
    for (int i = 0; i < 100; i++) {
        vertices.push_back(VertexFormat(glm::vec3(thirdLevelRadius * cos(AI_MATH_TWO_PI * i / 100), 1.4, thirdLevelRadius * sin(AI_MATH_TWO_PI * i / 100)), leafColor));
    }

    vertices.push_back(VertexFormat(glm::vec3(0, 3, 0), leafColor));
    vertices.push_back(VertexFormat(glm::vec3(0, 1.4, 0), leafColor));

    vertices.push_back(VertexFormat(glm::vec3(0, 2, 0), leafColor));
    vertices.push_back(VertexFormat(glm::vec3(0, 0.5, 0), leafColor));

    vertices.push_back(VertexFormat(glm::vec3(0, 1, 0), leafColor));
    vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), leafColor));

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices;

    // Generate Trunk
    for (int i = 0; i < 99; i++) {
        indices.push_back(i + 1);
        indices.push_back(i);
        indices.push_back(101 + i);

        indices.push_back(i);
        indices.push_back(100 + i);
        indices.push_back(101 + i);

    }

    indices.push_back(0);
    indices.push_back(99);
    indices.push_back(100);

    indices.push_back(99);
    indices.push_back(199);
    indices.push_back(100);

    // Generate first leaves

    for (int i = 0; i < 99; i++) {
        indices.push_back(201 + i);
        indices.push_back(200 + i);
        indices.push_back(vertices.size() - 1);
    }
    indices.push_back(299);
    indices.push_back(200);
    indices.push_back(vertices.size() - 1);

    for (int i = 0; i < 99; i++) {
        indices.push_back(201 + i);
        indices.push_back(200 + i);
        indices.push_back(vertices.size() - 2);
    }
    indices.push_back(299);
    indices.push_back(200);
    indices.push_back(vertices.size() - 2);

    // Generate second leaves

    for (int i = 0; i < 99; i++) {
        indices.push_back(301 + i);
        indices.push_back(300 + i);
        indices.push_back(vertices.size() - 3);
    }
    indices.push_back(399);
    indices.push_back(300);
    indices.push_back(vertices.size() - 3);

    for (int i = 0; i < 99; i++) {
        indices.push_back(301 + i);
        indices.push_back(300 + i);
        indices.push_back(vertices.size() - 4);
    }
    indices.push_back(399);
    indices.push_back(300);
    indices.push_back(vertices.size() - 4);

    // Generate third leaves

    for (int i = 0; i < 99; i++) {
        indices.push_back(401 + i);
        indices.push_back(400 + i);
        indices.push_back(vertices.size() - 5);
    }
    indices.push_back(499);
    indices.push_back(400);
    indices.push_back(vertices.size() - 5);

    for (int i = 0; i < 99; i++) {
        indices.push_back(401 + i);
        indices.push_back(400 + i);
        indices.push_back(vertices.size() - 6);
    }
    indices.push_back(499);
    indices.push_back(400);
    indices.push_back(vertices.size() - 6);

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object_3D::CreateArrow(const std::string& name, glm::vec3 color) 
{

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(2, 0, 0), color),
        VertexFormat(glm::vec3(-0.8, 0, -1), color),
        VertexFormat(glm::vec3(-0.8, 0, 1), color)
    };

    Mesh* arrow = new Mesh(name);
    std::vector<unsigned int> indices = {
        0, 1, 2
    };

    arrow->InitFromData(vertices, indices);
    return arrow;
}

Mesh* object_3D::CreateTerrain(const std::string& name) {
    glm::vec3 color = glm::vec3(0.2, 0.196, 0.024);

    std::vector<VertexFormat> vertices;
    for (int i = -100; i < 101; i++) {
        for (int j = -100; j < 101; j++) {
            vertices.push_back(VertexFormat(glm::vec3(i, 0, j), color));
        }
    }
    Mesh* terrain = new Mesh(name);
    std::vector<unsigned int> indices;
    for (int i = 0; i < 200; i++) {
        for (int j = 0; j < 200; j++) {
            indices.push_back(i * 201 + j + 1);
            indices.push_back(i * 201 + j);
            indices.push_back(i * 201 + 201 + j + 1);

            indices.push_back(i * 201 + j);

            indices.push_back(i * 201 + 201 + j);
            indices.push_back(i * 201 + 201 + j + 1);

        }
    }

    terrain->InitFromData(vertices, indices);
    return terrain;
}

Mesh* object_3D::CreateFrame(const std::string& name) {

    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0, 0, 1)),

    };
    Mesh* terrain = new Mesh(name);
    std::vector<unsigned int> indices = {
        1, 0, 2,
        2, 3, 1
    };

    terrain->InitFromData(vertices, indices);
    return terrain;
}

Mesh* object_3D::CreateDropzone(const std::string& name) {

    std::vector<VertexFormat> vertices;

    glm::vec4 color = glm::vec4(0, 1, 0.741, 0.5);

    for (int i = 0; i < 100; i++) {
        vertices.push_back(VertexFormat(glm::vec3(5 * cos(AI_MATH_TWO_PI * i / 100), 40, 5 * sin(AI_MATH_TWO_PI * i / 100)), color));
    }
    for (int i = 0; i < 100; i++) {
        vertices.push_back(VertexFormat(glm::vec3(5 * cos(AI_MATH_TWO_PI * i / 100), -20, 5 * sin(AI_MATH_TWO_PI * i / 100)), color));
    }

    std::vector<unsigned int> indices;

    for (int i = 0; i < 99; i++) {
        indices.push_back(i + 1);
        indices.push_back(i);
        indices.push_back(101 + i);

        indices.push_back(i);
        indices.push_back(100 + i);
        indices.push_back(101 + i);

    }

    indices.push_back(0);
    indices.push_back(99);
    indices.push_back(100);

    indices.push_back(99);
    indices.push_back(199);
    indices.push_back(100);

    Mesh* dropzone = new Mesh(name);

    dropzone->InitFromData(vertices, indices);
    return dropzone;
}