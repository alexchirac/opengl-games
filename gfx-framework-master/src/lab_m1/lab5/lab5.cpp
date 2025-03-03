#include "lab_m1/lab5/lab5.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab5::Lab5()
{
}


Lab5::~Lab5()
{
}


void Lab5::Init()
{
    renderCamera1Target = false;

    Camera1 = new implemented::Camera1();
    Camera1->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    projectionMatrix = ok ? glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f) : glm::ortho(-1, 1, -1, 1, 0, 200);

}


void Lab5::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    projectionMatrix = ok ? glm::perspective(RADIANS(fovy), window->props.aspectRatio, 0.01f, 200.0f) 
                          : glm::ortho(-float(sizex / 2), float(sizex / 2), -float(sizey / 2), float(sizey / 2), 0.01f, 200.00f);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab5::Update(float deltaTimeSeconds)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));

        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
        RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
    }

    // TODO(student): Draw more objects with different model matrices.
    // Attention! The `RenderMesh()` function overrides the usual
    // `RenderMesh()` that we've been using up until now. This new
    // function uses the view matrix from the Camera1 that you just
    // implemented, and the local projection matrix.

    // Render the Camera1 target. This is useful for understanding where
    // the rotation point is, when moving in third-person Camera1 mode.
    if (renderCamera1Target)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, Camera1->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }
}


void Lab5::FrameEnd()
{
    DrawCoordinateSystem(Camera1->GetViewMatrix(), projectionMatrix);
}


void Lab5::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(Camera1->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab5::OnInputUpdate(float deltaTime, int mods)
{
    // move the Camera1 only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float Camera1Speed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the Camera1 forward
            Camera1->TranslateForward(Camera1Speed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the Camera1 to the left
            Camera1->TranslateRight(-Camera1Speed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the Camera1 backward
            Camera1->TranslateForward(-Camera1Speed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the Camera1 to the right
            Camera1->TranslateRight(Camera1Speed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the Camera1 downward
            Camera1->TranslateUpward(Camera1Speed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the Camera1 upward
            Camera1->TranslateUpward(-Camera1Speed * deltaTime);

        }
    }

    // TODO(student): Change projection parameters. Declare any extra
    // variables you might need in the class header. Inspect this file
    // for any hardcoded projection arguments (can you find any?) and
    // replace them with those extra variables.
    if (window->KeyHold(GLFW_KEY_N))
        fovy += deltaTime * 10;
    if (window->KeyHold(GLFW_KEY_M))
        fovy -= deltaTime * 10;

    if (window->KeyHold(GLFW_KEY_1))
        sizex += deltaTime;
    if (window->KeyHold(GLFW_KEY_2))
        sizex -= deltaTime;

    if (window->KeyHold(GLFW_KEY_9))
        sizey += deltaTime;
    if (window->KeyHold(GLFW_KEY_0))
        sizey -= deltaTime;
}


void Lab5::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCamera1Target = !renderCamera1Target;
    }
    // TODO(student): Switch projections
    if (key == GLFW_KEY_O) {
        ok = true;
        cout << "X";
    }
    if (key == GLFW_KEY_P) {
        ok = false;
        cout << "Y";
    }
}


void Lab5::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCamera1Target = false;
            // TODO(student): Rotate the Camera1 in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            Camera1->RotateFirstPerson_OY(-deltaX * sensivityOX);
            Camera1->RotateFirstPerson_OX(-deltaY * sensivityOX);

        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCamera1Target = true;
            // TODO(student): Rotate the Camera1 in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            Camera1->RotateThirdPerson_OX(deltaX * sensivityOX);
            Camera1->RotateThirdPerson_OY(deltaY * sensivityOY);
        }
    }
}


void Lab5::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab5::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab5::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab5::OnWindowResize(int width, int height)
{
}
