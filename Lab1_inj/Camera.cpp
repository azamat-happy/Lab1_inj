#include <GL/freeglut.h>

#include "Camera.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
const static float StepScale = 0.1f;

Camera::Camera()
{
    m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_target = glm::vec3(0.0f, 0.0f, 1.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
}


Camera::Camera(const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
{
    m_position = Pos;
    m_target = Target;
    glm::normalize(m_target);
    m_up = Up;
    glm::normalize(m_up);
}


bool Camera::OnKeyboard(int Key)
{
    bool IsKeyReturn = false;

    switch (Key) {

    case GLUT_KEY_UP:
    {
     

        m_position += (m_target * StepScale);
        IsKeyReturn = true;
    }
    break;

    case GLUT_KEY_DOWN:
    {
        m_position -= (m_target * StepScale);
        IsKeyReturn = true;
    }
    break;

    case GLUT_KEY_LEFT:
    {
       
        glm::vec3 Left = glm::cross(m_target, m_up);
        glm::normalize(Left);
        Left *= StepScale;
        m_position += Left;
        IsKeyReturn = true;
    }
    break;

    case GLUT_KEY_RIGHT:
    {
        glm::vec3 Right = glm::cross(m_up, m_target);
        glm::normalize(Right);
        Right *= StepScale;
        m_position += Right;
        IsKeyReturn = true;
    }
    break;
    }

    return IsKeyReturn;
}