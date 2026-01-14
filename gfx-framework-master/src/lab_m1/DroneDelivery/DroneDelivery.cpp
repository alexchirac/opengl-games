#include "lab_m1/DroneDelivery/DroneDelivery.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


DroneDelivery::DroneDelivery()
{
}


DroneDelivery::~DroneDelivery()
{
}

float random(float min1, float max1)
{
    return min1 + (float)rand() / ((float)RAND_MAX / (max1 - min1));
}

void DroneDelivery::InitDropzone() {
    int idx = rand() % 10;

    packetPos = (*platPos)[idx];
    packetPos.y += 0.35;

    dropzonePos.x = random(-90, 90);
    dropzonePos.y = random(-90, 90);
}

void DroneDelivery::InitPlatforms()
{
    for (int i = 0; i < 10; i++) {
        glm::vec3 newPos = glm::vec3(random(-90, 90), random(0.5, 3), random(-90, 90));
        bool ok = true;
        if (distance(newPos, glm::vec3(0, 0, 0)) < 3) {
            ok = false;
        }
        for (const auto pos : *platPos) {
            if (distance(glm::vec3(newPos.x, 0, newPos.z), glm::vec3(pos.x, 0, pos.z)) < 10) {
                ok = false;
                break;
            }
        }
        if (ok) {
            platPos->push_back(newPos);
        }
        else {
            i--;
        }
    }
}

void DroneDelivery::InitTrees()
{
    for (int i = 0; i < 500; i++) {
        glm::vec3 newPos = glm::vec3(random(-100, 100), random(0.5, 3), random(-100, 100));
        bool ok = true;
        if (distance(newPos, glm::vec3(0, 0, 0)) < 3) {
            ok = false;
        }

        for (const auto pos : *platPos) {
            if (distance(glm::vec3(newPos.x, 0, newPos.z), glm::vec3(pos.x, 0, pos.z)) < 5) {
                ok = false;
                break;
            }
        }

        for (const auto pos : *treesPos) {
            if (distance(glm::vec3(newPos.x, 0, newPos.z), glm::vec3(pos.x, 0, pos.z)) < 3) {
                ok = false;
                break;
            }
        }
        if (ok) {
            treesPos->push_back(newPos);
        }
        else {
            i--;
        }
    }
}

void DroneDelivery::InitMeshes()
{
    Mesh* body = object_3D::CreateParalelipiped("body", 0.7, 0.1, 0.1, glm::vec3(0.482, 0.49, 0.388));
    meshes["body"] = body;

    Mesh* ending = object_3D::CreateParalelipiped("ending", 0.1, 0.1, 0.15, glm::vec3(0.482, 0.49, 0.388));
    meshes["ending"] = ending;

    Mesh* elice = object_3D::CreateParalelipiped("elice", 0.3, 0.1, 0.001, glm::vec3(1, 1, 1));
    meshes["elice"] = elice;

    Mesh* platform = object_3D::CreateParalelipiped("platform", 1.5, 1.5, 0.2, glm::vec3(0.231, 0.227, 0.157));
    meshes["platform"] = platform;

    Mesh* platformLeg = object_3D::CreateParalelipiped("platformLeg", 0.1, 0.1, 10, glm::vec3(0.231, 0.227, 0.157));
    meshes["platformLeg"] = platformLeg;

    Mesh* package = object_3D::CreateParalelipiped("package", 0.7, 0.7, 0.7, glm::vec3(1, 1, 0));
    meshes["package"] = package;

    Mesh* tree = object_3D::CreateTree("tree");
    meshes["tree"] = tree;

    Mesh* arrow1 = object_3D::CreateArrow("arrow1", glm::vec3(0.675, 0, 1));
    meshes["arrow1"] = arrow1;

    Mesh* arrow2 = object_3D::CreateArrow("arrow2", glm::vec3(0, 1, 0.741));
    meshes["arrow2"] = arrow2;

    Mesh* arrow3 = object_3D::CreateArrow("arrow3", glm::vec3(1, 1, 0));
    meshes["arrow3"] = arrow3;

    Mesh* terrain = object_3D::CreateTerrain("terrain");
    meshes["terrain"] = terrain;

    Mesh* dropzone = object_3D::CreateDropzone("dropzone");
    meshes["dropzone"] = dropzone;
}

void DroneDelivery::Init()
{
    renderCameraTarget = false;

    camera1 = new implemented::Camera();
    camera1->Set(glm::vec3(-3.5, 2, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    camera2 = new implemented::Camera();
    camera2->Set(glm::vec3(0, 2, 0), glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));

    glm::ivec2 resolution = window->GetResolution();
    textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 30);


    drone.treesPos = treesPos;
    drone.platPos = platPos;
    InitPlatforms();
    InitTrees();
    InitMeshes();
    InitDropzone();

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    {
        Shader* shader = new Shader("TerrainShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "DroneDelivery", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "DroneDelivery", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader* shader = new Shader("TransparentShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "DroneDelivery", "shaders", "TransparentVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "DroneDelivery", "shaders", "TransparentFS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}

void DroneDelivery::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    camera = camera1;
    glClearColor(0.012, 0.827, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    projectionMatrix = glm::perspective(RADIANS(fovy), window->props.aspectRatio, 0.01f, 200.0f);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

}

glm::mat4 AlignDroneToNormal(const glm::vec3& targetNormal) {
    glm::mat4 modelMatrix(1);

    glm::vec3 defaultNormal(0, 1, 0);

    glm::vec3 normalizedNormal = glm::normalize(targetNormal);

    glm::vec3 rotationAxis = glm::cross(defaultNormal, normalizedNormal);
    float dotProduct = glm::dot(defaultNormal, normalizedNormal);
    float rotationAngle = glm::acos(glm::clamp(dotProduct, -1.0f, 1.0f));

    if (glm::length(rotationAxis) < EPSILON) {
        if (dotProduct < 0.0f) {
            rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
            rotationAngle = glm::pi<float>();
        }
        else {
            return modelMatrix;
        }
    }

    modelMatrix = glm::rotate(modelMatrix, rotationAngle, glm::normalize(rotationAxis));

    return modelMatrix;
}

void DroneDelivery::RenderDrone(float deltaTimeSeconds)
{
    angle += 4 * deltaTimeSeconds;

    glm::mat4 modelMatrix;
    glm::mat4 droneMatrix = glm::mat4(1);
    droneMatrix *= transform3D::Translate(drone.pos.x, drone.pos.y, drone.pos.z);
    droneMatrix *= AlignDroneToNormal(drone.up);
    droneMatrix *= transform3D::RotateOY(drone.angle);
    droneMatrix *= transform3D::RotateOY(AI_MATH_PI / 4);

    modelMatrix = droneMatrix;
    RenderMesh(meshes["body"], shaders["VertexColor"], modelMatrix);

    modelMatrix = droneMatrix;
    modelMatrix *= transform3D::RotateOY(AI_MATH_HALF_PI);
    RenderMesh(meshes["body"], shaders["VertexColor"], modelMatrix);

    modelMatrix = droneMatrix;
    modelMatrix *= transform3D::Translate(0.3, 0.075, 0);
    RenderMesh(meshes["ending"], shaders["VertexColor"], modelMatrix);

    modelMatrix = droneMatrix;
    modelMatrix *= transform3D::Translate(-0.3, 0.075, 0);
    RenderMesh(meshes["ending"], shaders["VertexColor"], modelMatrix);

    modelMatrix = droneMatrix;
    modelMatrix *= transform3D::Translate(0, 0.075, 0.3);
    RenderMesh(meshes["ending"], shaders["VertexColor"], modelMatrix);

    modelMatrix = droneMatrix;
    modelMatrix *= transform3D::Translate(0, 0.075, -0.3);
    RenderMesh(meshes["ending"], shaders["VertexColor"], modelMatrix);

    modelMatrix = droneMatrix;
    modelMatrix *= transform3D::Translate(0.33, 0.15, 0);
    modelMatrix *= transform3D::RotateOY(angle);
    RenderMesh(meshes["elice"], shaders["VertexColor"], modelMatrix);

    modelMatrix = droneMatrix;
    modelMatrix *= transform3D::Translate(-0.33, 0.15, 0);
    modelMatrix *= transform3D::RotateOY(angle);
    RenderMesh(meshes["elice"], shaders["VertexColor"], modelMatrix);

    modelMatrix = droneMatrix;
    modelMatrix *= transform3D::Translate(0, 0.15, 0.33);
    modelMatrix *= transform3D::RotateOY(angle);
    RenderMesh(meshes["elice"], shaders["VertexColor"], modelMatrix);

    modelMatrix = droneMatrix;
    modelMatrix *= transform3D::Translate(0, 0.15, -0.33);
    modelMatrix *= transform3D::RotateOY(angle);
    RenderMesh(meshes["elice"], shaders["VertexColor"], modelMatrix);

    if (drone.packageAtached) {
        modelMatrix = droneMatrix;
        modelMatrix *= transform3D::Translate(0, -0.4, 0);
        RenderMesh(meshes["package"], shaders["VertexColor"], modelMatrix);
    }
}

void DroneDelivery::RotateBack(float deltaTimeSeconds) {
    if (!window->KeyHold(GLFW_KEY_W) && drone.angleFront > 0) {
        drone.angleFront -= 2 * deltaTimeSeconds;
        if (abs(drone.angleFront) < EPSILON) {
            drone.angleFront = 0;
        }
    }
    if (!window->KeyHold(GLFW_KEY_S) && drone.angleFront < 0) {
        drone.angleFront += 2 * deltaTimeSeconds;
        if (abs(drone.angleFront) < EPSILON) {
            drone.angleFront= 0;
        }
    }
    if (!window->KeyHold(GLFW_KEY_D) && drone.angleRight > 0) {
        drone.angleRight -= 2 * deltaTimeSeconds;
        if (abs(drone.angleRight) < EPSILON) {
            drone.angleRight = 0;
        }
    }
    if (!window->KeyHold(GLFW_KEY_A) && drone.angleRight < 0) {
        drone.angleRight += 2 * deltaTimeSeconds;
        if (abs(drone.angleRight) < EPSILON) {
            drone.angleRight = 0;
        }
    }
}

void DroneDelivery::RenderRectangle(float x, float y, float width, float height, glm::vec4 color)
{
    glClearColor(color[0], color[1], color[2], color[3]);

    glEnable(GL_SCISSOR_TEST);

    glScissor(x, y, width, height);

    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_SCISSOR_TEST);
}

void DroneDelivery::RenderPlatforms()
{
    glm::mat4 modelMatrix;

    for (const auto& pos : *platPos) {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(pos.x, pos.y, pos.z);
        RenderMesh(meshes["platform"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(pos.x + 0.7, pos.y - 5, pos.z + 0.7);
        RenderMesh(meshes["platformLeg"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(pos.x - 0.7, pos.y - 5, pos.z + 0.7);
        RenderMesh(meshes["platformLeg"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(pos.x + 0.7, pos.y - 5, pos.z - 0.7);
        RenderMesh(meshes["platformLeg"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(pos.x - 0.7, pos.y - 5, pos.z - 0.7);
        RenderMesh(meshes["platformLeg"], shaders["VertexColor"], modelMatrix);
    }

    if (!drone.packageAtached) {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(packetPos.x, packetPos.y, packetPos.z);
        RenderMesh(meshes["package"], shaders["VertexColor"], modelMatrix);
    }
}

void DroneDelivery::RenderTrees()
{
    glm::mat4 modelMatrix = glm::mat4(1);

    for (const auto& pos : *treesPos) {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(pos.x, pos.y, pos.z);
        RenderMesh(meshes["tree"], shaders["VertexColor"], modelMatrix);
    }
}

void DroneDelivery::Update(float deltaTimeSeconds)
{
    drone.updatePos(deltaTimeSeconds, window->KeyHold(GLFW_KEY_W) ^ window->KeyHold(GLFW_KEY_S), window->KeyHold(GLFW_KEY_A) ^ window->KeyHold(GLFW_KEY_D));
    
    if (glm::distance(drone.pos, packetPos) < 1 && !packetFalling) {
        drone.packageAtached = true;
    }

    if (packetFalling) {
        packetSpeed.y -= deltaTimeSeconds;
        packetPos += packetSpeed;

        if (packetPos.y < 0.5) {
            packetPos.y = 0.5;
            if (glm::distance(packetPos, glm::vec3(dropzonePos[0], 0.5, dropzonePos[1])) < 5) {
                InitDropzone();
                score += 100;
            }
            packetFalling = false;
        }
    }

    camera1->position = drone.pos - camera1->distanceToTarget * camera1->forward;
    camera = camera1;

    RotateBack(deltaTimeSeconds);
    drone.computeUp();
    RenderDrone(deltaTimeSeconds);

    RenderPlatforms();
    RenderTrees();

    glm::mat4 modelMatrix = glm::mat4(1);
    RenderSimpleMesh(meshes["terrain"], shaders["TerrainShader"], modelMatrix);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(dropzonePos[0], 0, dropzonePos[1]);
    RenderMesh(meshes["dropzone"], shaders["TransparentShader"], modelMatrix);
    glDisable(GL_BLEND);

    glm::vec2 res = window->GetResolution();
    
    RenderRectangle(0, 0, res.x / 5 + 10, res.x / 5 + 10, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    RenderRectangle(5, 5, res.x / 5, res.x / 5, glm::vec4(0, 0, 0, 1));

    textRenderer->RenderText("Score: " + to_string(score), res.x - 200, res.y - 40, 1, glm::vec4(1, 1, 1, 1));

    MinimapUpdate();
}

void DroneDelivery::MinimapUpdate()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_DEPTH_BUFFER_BIT);
    projectionMatrix = glm::ortho(-sizex, sizex, -sizey, sizey, 0.01f, 200.00f);
    //projectionMatrix = glm::perspective(RADIANS(fovy), window->props.aspectRatio, 0.01f, 200.0f);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(5, 5, resolution.x / 5, resolution.x / 5);

    camera2->position = glm::vec3(drone.pos.x, 45, drone.pos.z);
    camera = camera2;

    glm::mat4 droneMatrix = glm::mat4(1);
    droneMatrix *= transform3D::Translate(drone.pos.x, 40, drone.pos.z);
    droneMatrix *= transform3D::RotateOY(drone.angle);
    droneMatrix *= transform3D::Scale(1.5, 1, 1.5);
    RenderMesh(meshes["arrow1"], shaders["VertexColor"], droneMatrix);

    glm::mat4 modelMatrix;

    if (drone.packageAtached) {
        // Render dropzone arrow
        glm::vec3 dir = glm::vec3(dropzonePos[0] - drone.pos.x, 0, dropzonePos[1] - drone.pos.z);
        float rotationAngle = acos(glm::clamp(glm::dot(glm::vec3(1, 0, 0), glm::normalize(dir)), -1.0f, 1.0f));
        if (dir.z < 0)
            rotationAngle *= -1;

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(drone.pos.x, 40, drone.pos.z);
        modelMatrix *= transform3D::RotateOY(-rotationAngle);
        modelMatrix *= transform3D::Translate(3, 0, 0);
        RenderMesh(meshes["arrow2"], shaders["VertexColor"], modelMatrix);
    }
    else {
        // Render package arrow
        glm::vec3 dir = glm::vec3(packetPos[0] - drone.pos.x, 0, packetPos[2] - drone.pos.z);
        float rotationAngle = acos(glm::clamp(glm::dot(glm::vec3(1, 0, 0), glm::normalize(dir)), -1.0f, 1.0f));
        if (dir.z < 0)
            rotationAngle *= -1;

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(drone.pos.x, 40, drone.pos.z);
        modelMatrix *= transform3D::RotateOY(-rotationAngle);
        modelMatrix *= transform3D::Translate(3, 0, 0);
        RenderMesh(meshes["arrow3"], shaders["VertexColor"], modelMatrix);
    }

    modelMatrix = glm::mat4(1);
    RenderSimpleMesh(meshes["terrain"], shaders["TerrainShader"], modelMatrix);

    RenderTrees();
    if (!drone.packageAtached) {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(packetPos.x, packetPos.y, packetPos.z);
        modelMatrix *= transform3D::Scale(5, 1, 5);
        RenderMesh(meshes["package"], shaders["VertexColor"], modelMatrix);
    }
}

void DroneDelivery::FrameEnd()
{
    
}

void DroneDelivery::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    mesh->Render();
}

void DroneDelivery::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set Model matrix uniform
    auto location = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Set View matrix uniform
    location = glGetUniformLocation(shader->program, "View");


    // Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Set Projection matrix uniform
    location = glGetUniformLocation(shader->program, "Projection");

    glm::mat4 projectionMatrix = this->projectionMatrix;
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */
void DroneDelivery::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            camera1->MoveForward(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera1->TranslateRight(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera1->MoveForward(-cameraSpeed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera1->TranslateRight(cameraSpeed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            camera1->TranslateUpward(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            camera1->TranslateUpward(-cameraSpeed * deltaTime);

        }
    }
    else {
        if (window->KeyHold(GLFW_KEY_SPACE)) {
            drone.accelerate(deltaTime);
            angle += 20 * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_W)) {
            if (drone.angleFront < RADIANS(40)) {
                drone.angleFront += 2 * deltaTime;
            }
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            if (drone.angleFront > - RADIANS(40)) {
                drone.angleFront -= 2 * deltaTime;
            }
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            if (drone.angleRight < RADIANS(40)) {
                drone.angleRight += 2 * deltaTime;
            }
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            if (drone.angleRight > - RADIANS(40)) {
                drone.angleRight -= 2 * deltaTime;
            }
        }
        if (window->KeyHold(GLFW_KEY_RIGHT)) {
            camera1->RotateThirdPerson_OX(-deltaTime);
            drone.rotateOY(-deltaTime);
            drone.angle -= deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_LEFT)) {
            camera1->RotateThirdPerson_OX(deltaTime);
            drone.rotateOY(deltaTime);
            drone.angle += deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_UP)) {
            camera1->RotateThirdPerson_OY(-deltaTime);
        }
        if (window->KeyHold(GLFW_KEY_DOWN)) {
            camera1->RotateThirdPerson_OY(deltaTime);
        }
    }
}


void DroneDelivery::OnKeyPress(int key, int mods)
{
    if (drone.packageAtached == true && key == GLFW_KEY_G) {
        drone.packageAtached = false;
        packetPos = drone.pos;
        packetPos.y -= 0.5;
        packetSpeed = glm::vec3(0, 0, 0);
        packetFalling = true;
    }
}


void DroneDelivery::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void DroneDelivery::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void DroneDelivery::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void DroneDelivery::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void DroneDelivery::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void DroneDelivery::OnWindowResize(int width, int height)
{
}
