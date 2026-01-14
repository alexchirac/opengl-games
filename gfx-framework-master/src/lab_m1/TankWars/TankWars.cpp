#include "lab_m1/TankWars/TankWars.h"

#include <vector>
#include <iostream>

#include "lab_m1/TankWars/transform2D.h"
#include "lab_m1/TankWars/object.h"
#include "lab_m1/TankWars/entities.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


TankWars::TankWars()
{
}


TankWars::~TankWars()
{
}

float TankWars::terrainFunction(float x)
{
    float sum = 0;
    for (int i = 0; i < redimensionSize; i++)
        sum += as[i] * sin(omegas[i] * x);

    return sum + 400;
}

void TankWars::Init()
{
    resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    rectSize = 1 / rectsPerUnit;

    for (int i = 0; i <= (int)(resolution.x * rectsPerUnit); i++) {
        float x = (float)i / rectsPerUnit;
        ys.push_back(terrainFunction(x));
    }

    maxX = resolution.x - 30;
    tankA = new entities::Tank(100, - AI_MATH_PI / 4, ys, rectsPerUnit, maxX);
    tankB = new entities::Tank(400, AI_MATH_PI / 4, ys, rectsPerUnit, maxX);

    glm::vec3 center = glm::vec3(0, 0, 0);
    float squareSide = 100;

    Mesh* terrainObject = object::CreateTerrain("terrainObject", center, 1, glm::vec3(0.298, 0.431, 0.188), true);
    AddMeshToList(terrainObject);

    Mesh* tank = object::CreateTank("tank", glm::vec3(0.835, 0.071, 0), glm::vec3(0.114, 0.125, 0.212));
    AddMeshToList(tank);

    Mesh* square1 = object::CreateSquare("square1", glm::vec3(0, 0, 0));
    AddMeshToList(square1);

    Mesh* square2 = object::CreateSquare("square2", glm::vec3(1, 1, 1));
    AddMeshToList(square2);

    Mesh* lifebarFrame = object::CreateLifebarFrame("frame", lifebarLength, lifebarHeight);
    AddMeshToList(lifebarFrame);

    Mesh* projectile = object::CreateProjectile("projectile");
    AddMeshToList(projectile);

    Mesh* line = object::CreateLine("line", lineLen);
    AddMeshToList(line);
}


void TankWars::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.224, 0.604, 0.729, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void TankWars::renderTerrain()
{
    for (int i = 0; i < ys.size() - 1; i++) {
        float x = (float)i / rectsPerUnit;
        float x1 = (float)(i + 1) / rectsPerUnit;
        float y = ys[i];
        float y1 = ys[i + 1];

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x, y);
        modelMatrix *= transform2D::Cut(x1 - x, y1 - y);
        modelMatrix *= transform2D::Scale(rectSize, 800);
        RenderMesh2D(meshes["terrainObject"], shaders["VertexColor"], modelMatrix);
    }
}

void TankWars::renderTrajectory(entities::Tank* tank) {
    if (tank->health == 0)
        return;
    float angle = tank->turretAngle + AI_MATH_HALF_PI;
    float x = tank->centerx + TURRET_LEN * cos(angle);
    float y = tank->centery + TURRET_LEN * sin(angle);

    float vx = magnitude * cos(angle);
    float vy = magnitude * sin(angle);

    float dt = 0.05;

    while (1) {
        if (x < 0 || x > resolution.x || y < 0)
            break;
        
        float dx = 2 * vx * dt;
        float dy = 2 * vy * dt;

        float len = sqrt(dx * dx + dy * dy);
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x, y);
        modelMatrix *= transform2D::Rotate(atan2(dy, dx));
        modelMatrix *= transform2D::Rotate(-AI_MATH_HALF_PI);
        modelMatrix *= transform2D::Scale(1, len);
        RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

        x = x + dx;
        y = y + dy;
        vy -= GRAVITY * dt;
    }
}

void TankWars::renderTank(entities::Tank* tank)
{
    if (tank->health == 0)
        return;
    tank->calculateYAngleAndCenter();
    // Lifebar
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(tank->posx - lifebarLength / 2, tank->posy + 60);
    modelMatrix *= transform2D::Scale(tank->health / tank->maxHealth * lifebarLength, lifebarHeight);
    modelMatrix *= transform2D::Translate(0.5, 0);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    // Lifebar Frame
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(tank->posx, tank->posy + 60);
    RenderMesh2D(meshes["frame"], shaders["VertexColor"], modelMatrix);

    // Tank Body
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(tank->posx, tank->posy);
    modelMatrix *= transform2D::Rotate(tank->angle);
    RenderMesh2D(meshes["tank"], shaders["VertexColor"], modelMatrix);

    // Tank Turret
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(tank->centerx, tank->centery);
    modelMatrix *= transform2D::Rotate(tank->turretAngle);
    modelMatrix *= transform2D::Scale(5, TURRET_LEN);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
}

bool TankWars::projectileIntersectsTank(entities::Projectile* projectile, entities::Tank* tank)
{
    if (tank->health == 0)
        return false;
    float dx = projectile->posx - tank->posx;
    float dy = projectile->posy - tank->posy - TANK_HEIGHT;

    return dx * dx + dy * dy < 30 * 30;
}

void TankWars::destroyTerrain(float posx, float posy)
{
    int i = floor(posx * rectsPerUnit);
    for (int j = i; j >= 0; j--) {
        float x = (float)j / rectsPerUnit;
        if (x < posx - blastRadius)
            break;
        float dx = posx - x;
        float angle = acos(dx / blastRadius);

        float targetY = posy - blastRadius * sin(angle);

        if (targetY < 0)
            targetY = 0;
        if (ys[j] > targetY)
            ys[j] = targetY;
    }
    for (int j = i + 1; j < ys.size(); j++) {
        float x = (float)j / rectsPerUnit;
        if (x > posx + blastRadius)
            break;
        float dx = x - posx;
        float angle = acos(dx / blastRadius);

        float targetY = posy - blastRadius * sin(angle);

        if (targetY < 0)
            targetY = 0;
        if (ys[j] > targetY)
            ys[j] = targetY;
    }
}

void TankWars::updateAndRenderProjectiles(float deltaTimeSeconds)
{
    for (int i = 0; i < projectiles.size(); i++) {
        entities::Projectile *projectile = projectiles[i];
        projectile->update(deltaTimeSeconds);

        if (projectile->posx < 0 || projectile->posx >= resolution.x) {
            projectile->~Projectile();
            projectiles.erase(projectiles.begin() + i);
            i--;
            continue;
        }

        if (projectile->posy - projectile->getProjectionY() < EPSILON) {
            destroyTerrain(projectile->posx, projectile->posy);
            projectile->~Projectile();
            projectiles.erase(projectiles.begin() + i);
            i--;
            continue;
        }

        if (projectileIntersectsTank(projectile, tankA)) {
            tankA->loseHealth();
            projectile->~Projectile();
            projectiles.erase(projectiles.begin() + i);
            i--;
            continue;
        }

        if (projectileIntersectsTank(projectile, tankB)) {
            tankB->loseHealth();
            projectile->~Projectile();
            projectiles.erase(projectiles.begin() + i);
            i--;
            continue;
        }

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(projectile->posx, projectile->posy);
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
    }
}

void TankWars::landslide() {
    float last = ys[0];
    for (int i = 1; i < ys.size() - 1; i++) {
        float aux = ys[i];
        ys[i] = (last + ys[i] + ys[i + 1]) / 3;
        last = aux;
    }
    ys[0] = ys[1];
    ys[ys.size() - 1] = ys[ys.size() - 2];
}

void TankWars::Update(float deltaTimeSeconds)
{
    // Update game state
    updateAndRenderProjectiles(deltaTimeSeconds);

    renderTank(tankA);
    renderTank(tankB);

    landslide();

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(100, 100);
    RenderMesh2D(meshes["line"], shaders["VertexColor"], modelMatrix);

    renderTerrain();

    renderTrajectory(tankA);
    renderTrajectory(tankB);
}


void TankWars::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void TankWars::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_D)) {
        tankA->move(100 * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        tankA->move(-100 * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        tankA->moveTurret(deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        tankA->moveTurret(-deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        tankB->move(100 * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        tankB->move(-100 * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_UP)) {
        tankB->moveTurret(deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        tankB->moveTurret(-deltaTime);
    }
}


void TankWars::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE && tankA->health > 0) {
        float angle = tankA->turretAngle + AI_MATH_HALF_PI;
        float posx = tankA->centerx + TURRET_LEN * cos(angle);
        float posy = tankA->centery + TURRET_LEN * sin(angle);
        projectiles.push_back(new entities::Projectile(posx, 
            posy, magnitude * cos(angle), magnitude * sin(angle), ys, rectsPerUnit, maxX));
    }
    if (key == GLFW_KEY_ENTER && tankB->health > 0) {
        float angle = tankB->turretAngle + AI_MATH_HALF_PI;
        float posx = tankB->centerx + TURRET_LEN * cos(angle);
        float posy = tankB->centery + TURRET_LEN * sin(angle);
        projectiles.push_back(new entities::Projectile(posx, 
            posy, magnitude * cos(angle), magnitude * sin(angle), ys, rectsPerUnit, maxX));
    }
}


void TankWars::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void TankWars::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void TankWars::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void TankWars::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event

}


void TankWars::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void TankWars::OnWindowResize(int width, int height)
{
}
