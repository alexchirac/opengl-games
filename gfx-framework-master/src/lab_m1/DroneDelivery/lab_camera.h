#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace implemented
{
    class Camera
    {
     public:
        Camera()
        {
            position    = glm::vec3(0, 0, 0);
            forward     = glm::vec3(0, 0, -1);
            up          = glm::vec3(0, 1, 0);
            right       = glm::vec3(1, 0, 0);
            distanceToTarget = 4;
        }

        Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            Set(position, center, up);
        }

        ~Camera()
        { }

        // Update camera
        void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            this->position = position;
            forward     = glm::normalize(center - position);
            right       = glm::cross(forward, up);
            this->up    = glm::cross(right, forward);
        }

        void MoveForward(float distance)
        {
            // Translates the camera using the `dir` vector computed from
            // `forward`. Movement will always keep the camera at the same
            // height. For example, if you rotate your head up/down, and then
            // walk forward, then you will still keep the same relative
            // distance (height) to the ground!
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            // Translate the camera using the forward vector
            position += distance * forward;
        }

        void TranslateUpward(float distance)
        {
            // Translate the camera using the up vector
            position += distance * up;
        }

        void TranslateRight(float distance)
        {
            // Use the projected right vector (onto ground plane) to maintain
            // consistent distance from ground even when camera is rotated
            glm::vec3 vec = glm::normalize(glm::vec3(right.x, 0, right.z));
            position += distance * vec;

        }

        void RotateFirstPerson_OX(float angle)
        {
            // Rotate around the right axis (pitch)
            forward = glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 1);
            forward = glm::normalize(forward);
            up = glm::cross(right, forward);
            up = glm::normalize(up);

        }

        void RotateFirstPerson_OY(float angle)
        {
            // Rotate around the world Y axis (yaw)
            glm::vec4 newVector = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1);
            forward = glm::normalize(glm::vec3(newVector));

            newVector = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
            right = glm::normalize(glm::vec3(newVector));

            up = glm::cross(right, forward);
            up = glm::normalize(up);
        }

        void RotateFirstPerson_OZ(float angle)
        {
            // Rotate around the forward axis (roll)
            right = glm::rotate(glm::mat4(1.0f), angle, forward) * glm::vec4(right, 1);
            right = glm::normalize(right);
            up = glm::cross(right, forward);
            up = glm::normalize(up);

        }

        void RotateThirdPerson_OX(float angle)
        {
            // Third-person rotation around OX axis
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OY(float angle)
        {
            // Third-person rotation around OY axis
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(float angle)
        {
            // Third-person rotation around OZ axis
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);
        }

        glm::mat4 GetViewMatrix()
        {
            // Returns the view matrix
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

     public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}   // namespace implemented
