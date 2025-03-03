#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    // Sets the resolution of the small viewport
    resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);
}

void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Lab4::RenderTree(glm::mat4 matrix, int level, double myx, double myy) {
    if (level == 2)
        return;

    RenderMesh(meshes["box"], shaders["VertexNormal"], matrix * transform3D::Translate(myx, myy, 0));

    glm::mat4 matrix1 = matrix;
    //matrix1 *= transform3D::Scale(0.5, 0.5, 0.5);
    matrix1 *= transform3D::RotateOZ(-AI_MATH_PI / 6);
    RenderTree(matrix1, level + 1, myx - 0.5, myy + 1.5);

    glm::mat4 matrix2 = matrix;
    //matrix2 *= transform3D::Scale(0.5, 0.5, 0.5);
    matrix2 *= transform3D::RotateOZ(AI_MATH_PI / 6);
    RenderTree(matrix2, level + 1, myx + 0.5, myy + 1.5);

}

void Lab4::RenderScene() {
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Scale(1, 2, 1);
    RenderTree(modelMatrix, 0, 0, 0.5);
}

void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    RenderScene();
    DrawCoordinateSystem();

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    // TODO(student): render the scene again, in the new viewport
    RenderScene();
    DrawCoordinateSystem();
}

void Lab4::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic
    if (window->KeyHold(GLFW_KEY_W))
        translateZ -= 0.4 * deltaTime;
    if (window->KeyHold(GLFW_KEY_S))
        translateZ += 0.4 * deltaTime;
    if (window->KeyHold(GLFW_KEY_A))
        translateX -= 0.4 * deltaTime;
    if (window->KeyHold(GLFW_KEY_D))
        translateX += 0.4 * deltaTime;
    if (window->KeyHold(GLFW_KEY_R))
        translateY += 0.4 * deltaTime;
    if (window->KeyHold(GLFW_KEY_F))
        translateY -= 0.4 * deltaTime;

    if (window->KeyHold(GLFW_KEY_1)) {
        scaleX -= 0.2 * deltaTime;
        scaleY -= 0.2 * deltaTime;
        scaleZ -= 0.2 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_2)) {
        scaleX += 0.2 * deltaTime;
        scaleY += 0.2 * deltaTime;
        scaleZ += 0.2 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_3))
        angularStepOX -= 0.4 * deltaTime;
    if (window->KeyHold(GLFW_KEY_4))
        angularStepOX += 0.4 * deltaTime;
    if (window->KeyHold(GLFW_KEY_5))
        angularStepOY -= 0.4 * deltaTime;
    if (window->KeyHold(GLFW_KEY_6))
        angularStepOY += 0.4 * deltaTime;
    if (window->KeyHold(GLFW_KEY_7))
        angularStepOZ -= 0.4 * deltaTime;
    if (window->KeyHold(GLFW_KEY_8))
        angularStepOZ += 0.4 * deltaTime;
}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
    
    // TODO(student): Add viewport movement and scaling logic
    if (key == GLFW_KEY_I) {
        miniViewportArea.x = 50;
        miniViewportArea.y = 50;
    }
    if (key == GLFW_KEY_J) {
        miniViewportArea.x = resolution.x - miniViewportArea.width - 50;
        miniViewportArea.y = 50;
    }
    if (key == GLFW_KEY_K) {
        miniViewportArea.x = 50;
        miniViewportArea.y = resolution.y - miniViewportArea.height - 50;
    }
    if (key == GLFW_KEY_L) {
        miniViewportArea.x = resolution.x - miniViewportArea.width - 50;
        miniViewportArea.y = resolution.y - miniViewportArea.height - 50;
    }

    if (key == GLFW_KEY_U && ok == 0) {
        miniViewportArea.height *= 2.4;
        miniViewportArea.width *= 2.4;
        ok = 1;
    }
    if (key == GLFW_KEY_O && ok == 1) {
        miniViewportArea.height /= 2.4;
        miniViewportArea.width /= 2.4;
        ok = 0;
    }

}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
