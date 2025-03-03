#pragma once

#include "components/simple_scene.h"
#include "lab_m1/DroneDelivery/lab_camera.h"
#include "lab_m1/DroneDelivery/3D_object.h"
#include "lab_m1/DroneDelivery/transform3D.h"
#include "lab_m1/DroneDelivery/drone.h"
#include "components/text_renderer.h"


#include <vector>

namespace m1
{
    class DroneDelivery : public gfxc::SimpleScene
    {
     public:
        DroneDelivery();
        ~DroneDelivery();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void MinimapUpdate();
        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        void RenderRectangle(float x, float y, float width, float height, glm::vec4 color);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RotateBack(float deltaTimeSeconds);

        void RenderDrone(float deltaTimeSeconds);
        void RenderPlatforms();
        void RenderTrees();
        void InitDropzone();
        void InitPlatforms();
        void InitTrees();
        void InitMeshes();


     protected:
        implemented::Camera *camera1, *camera2, *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        // TODO(student): If you need any other class variables, define them here.
        gfxc::TextRenderer* textRenderer;
        float fovy = 60;
        float sizex = 20;
        float sizey = 20;

        float angle = 0;
        int score = 0;
        glm::vec2 dropzonePos;
        glm::vec3 packetPos;
        bool packetFalling = false;
        glm::vec3 packetSpeed = glm::vec3(0, 0, 0);

        std::vector<glm::vec3> *treesPos = new std::vector<glm::vec3>();
        std::vector<glm::vec3>* platPos = new std::vector<glm::vec3>();

        Drone drone;
    };
}   // namespace m1
