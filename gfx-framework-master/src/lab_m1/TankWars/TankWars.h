#pragma once

#include "components/simple_scene.h"
#include "lab_m1/TankWars/entities.h"

namespace m1
{
    class TankWars : public gfxc::SimpleScene
    {
     public:
        TankWars();
        ~TankWars();

        void Init() override;

     private:
        void renderTerrain();
        void renderTank(entities::Tank* tank);
        void updateAndRenderProjectiles(float deltaTimeSeconds);
        bool projectileIntersectsTank(entities::Projectile *projectile, entities::Tank *tank);
        void destroyTerrain(float posx, float posy);
        void landslide();
        void renderTrajectory(entities::Tank* tank);


        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        float terrainFunction(float x);

     protected:
        glm::ivec2 resolution;
        
        entities::Tank *tankA, *tankB;
        std::vector<entities::Projectile*> projectiles;

        float maxX;
        std::vector<float> ys;
        float rectSize;
        float rectsPerUnit = 0.5;
        float lineLen = 1;
        int redimensionSize = 4;
        float magnitude = 400;
        std::vector<float> as = { 20, 40, 50, 20 }, omegas = {0.023, 0.014, 0.008, 0.007};

        glm::mat3 modelMatrix;

        float lifebarLength = 80;
        float lifebarHeight = 10;

        float blastRadius = 50;
        float maxDif = 0.2;
    };
}   // namespace m1
