#pragma once
#ifndef CAMERA_H
#define	CAMERA_H
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
//#include "math_3d.h"


class Camera
{
public:

    Camera();

    Camera(const glm::vec3& Pos, const glm::vec3&Target, const glm::vec3& Up);

    bool OnKeyboard(int Key);

    const glm::vec3& GetPos() const
    {
        return m_position;
    }

    const glm::vec3& GetTarget() const
    {
        return m_target;
    }

    const glm::vec3& GetUp() const
    {
        return m_up;
    }

private:

    glm::vec3 m_position;
    glm::vec3 m_target;
    glm::vec3 m_up;
};

#endif	