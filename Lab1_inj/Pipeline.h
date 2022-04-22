#pragma once
#include "glm/glm.hpp"
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

#define _USE_MATH_DEFINES
#include <math.h> // ����� pi
// ����� ��� ��������������
class Pipeline
{
private:

    glm::vec3 m_scale;
    glm::vec3 m_worldPos;
    glm::vec3 m_rotateInfo;
    glm::mat4 m_transformation;

    struct {
        float FOV;
        float Width;
        float Height;
        float zNear;
        float zFar;
    } m_persProj;

    struct {
        glm::vec3 Pos;
        glm::vec3 Target;
        glm::vec3 Up;
    } m_camera;
public:
    Pipeline():
    
        /*m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_worldPos = glm::vec3(0.0f, 0.0f, 0.0f);
        m_rotateInfo = glm::vec3(0.0f, 0.0f, 0.0f);*/
        m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
            m_worldPos(glm::vec3(0.0f, 0.0f, 0.0f)),
            m_rotateInfo(glm::vec3(0.0f, 0.0f, 0.0f)),
            m_persProj(),
            m_transformation(glm::mat4()) {}
        
    

    //������� ������� ��������� ��������
    void Scale(float ScaleX, float ScaleY, float ScaleZ)
    {
        m_scale.x = ScaleX;
        m_scale.y = ScaleY;
        m_scale.z = ScaleZ;
    }

    // ������� ������� ��������� ���������
    void WorldPos(float x, float y, float z)
    {
        m_worldPos.x = x;
        m_worldPos.y = y;
        m_worldPos.z = z;
    }

    // ������� ������� ��������
    void Rotate(float RotateX, float RotateY, float RotateZ)
    {
        m_rotateInfo.x = RotateX;
        m_rotateInfo.y = RotateY;
        m_rotateInfo.z = RotateZ;
    }

    const glm::mat4* getTransformation()
    {
        glm::mat4 ScaleTrans, RotateTrans, TranslationTrans, PersProjTrans, CameraTranslationTrans, CameraRotateTrans;

       
        InitScaleTransform(ScaleTrans);
        InitRotateTransform(RotateTrans);
        InitTranslationTransform(TranslationTrans);
        InitPerspectiveProj(PersProjTrans);
        InitTranslationTransform(CameraTranslationTrans);
        InitCameraTransform(CameraRotateTrans);
        m_transformation = PersProjTrans * CameraRotateTrans * CameraTranslationTrans * TranslationTrans * RotateTrans * ScaleTrans;
        return &m_transformation;
    }
   
    void InitScaleTransform(glm::mat4& m) const
    {
        m[0][0] = m_scale.x; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
        m[1][0] = 0.0f;      m[1][1] = m_scale.y; m[1][2] = 0.0f; m[1][3] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = m_scale.z; m[2][3] = 0.0f;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }

    void InitRotateTransform(glm::mat4& m) const
    {
        glm::mat4 rx, ry, rz;

        const float x = ToRadian(m_rotateInfo.x);
        const float y = ToRadian(m_rotateInfo.y);
        const float z = ToRadian(m_rotateInfo.z);

        rx[0][0] = 1.0f; rx[0][1] = 0.0f; rx[0][2] = 0.0f; rx[0][3] = 0.0f;
        rx[1][0] = 0.0f; rx[1][1] = cosf(x); rx[1][2] = -sinf(x); rx[1][3] = 0.0f;
        rx[2][0] = 0.0f; rx[2][1] = sinf(x); rx[2][2] = cosf(x); rx[2][3] = 0.0f;
        rx[3][0] = 0.0f; rx[3][1] = 0.0f; rx[3][2] = 0.0f; rx[3][3] = 1.0f;

        ry[0][0] = cosf(y); ry[0][1] = 0.0f; ry[0][2] = -sinf(y); ry[0][3] = 0.0f;
        ry[1][0] = 0.0f; ry[1][1] = 1.0f; ry[1][2] = 0.0f; ry[1][3] = 0.0f;
        ry[2][0] = sinf(y); ry[2][1] = 0.0f; ry[2][2] = cosf(y); ry[2][3] = 0.0f;
        ry[3][0] = 0.0f; ry[3][1] = 0.0f; ry[3][2] = 0.0f; ry[3][3] = 1.0f;

        rz[0][0] = cosf(z); rz[0][1] = -sinf(z); rz[0][2] = 0.0f; rz[0][3] = 0.0f;
        rz[1][0] = sinf(z); rz[1][1] = cosf(z); rz[1][2] = 0.0f; rz[1][3] = 0.0f;
        rz[2][0] = 0.0f; rz[2][1] = 0.0f; rz[2][2] = 1.0f; rz[2][3] = 0.0f;
        rz[3][0] = 0.0f; rz[3][1] = 0.0f; rz[3][2] = 0.0f; rz[3][3] = 1.0f;

        m = rz * ry * rx;
    }

    void InitTranslationTransform(glm::mat4& m) const
    {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;m[0][3] = m_worldPos.x;
        m[1][0] = 0.0f; m[1][1] = 1.0f;m[1][2] = 0.0f;m[1][3] = m_worldPos.y;
        m[2][0] = 0.0f; m[2][1] = 0.0f;m[2][2] = 1.0f; m[2][3] = m_worldPos.z;
        m[3][0] = 0.0f; m[3][1] = 0.0f;m[3][2] = 0.0f; m[3][3] = 1.0f;
    }
    void SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar)
    {
        m_persProj.FOV = FOV;
        m_persProj.Width = Width;
        m_persProj.Height = Height;
        m_persProj.zNear = zNear;
        m_persProj.zFar = zFar;
    }
    void InitPerspectiveProj(glm::mat4& m) const 
    {
        const float ar = m_persProj.Width / m_persProj.Height;
        const float zNear = m_persProj.zNear;
        const float zFar = m_persProj.zFar;
        const float zRange = zNear - zFar;
        const float tanHalfFOV = tanf(ToRadian(m_persProj.FOV / 2.0));

        m[0][0] = 1.0f / (tanHalfFOV * ar);
        m[0][1] = 0.0f;
        m[0][2] = 0.0f;
        m[0][3] = 0.0f;

        m[1][0] = 0.0f;
        m[1][1] = 1.0f / tanHalfFOV;
        m[1][2] = 0.0f;
        m[1][3] = 0.0f;

        m[2][0] = 0.0f;
        m[2][1] = 0.0f;
        m[2][2] = (-zNear - zFar) / zRange;
        m[2][3] = 2.0f * zFar * zNear / zRange;

        m[3][0] = 0.0f;
       m[3][1] = 0.0f;
        m[3][2] = 1.0f;
        m[3][3] = 0.0f; 
    }
    void SetCamera(const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
    {
        m_camera.Pos = Pos;
        m_camera.Target = Target;
        m_camera.Up = Up;
    }
    void InitTranslationTransform(glm::mat4& m, float x, float y, float z)
    {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }

    glm::vec3 Cross(const glm::vec3& v) const
    {
        const float _x = y * v.z - z * v.y;
        const float _y = z * v.x - x * v.z;
        const float _z = x * v.y - y * v.x;

        return glm::vec3(_x, _y, _z);
    }
    glm::vec3& Normalize()
    {
        const float Length = sqrtf(x * x + y * y + z * z);

        x /= Length;
        y /= Length;
        z /= Length;

        return (glm::vec3)this;
    }
    void InitCameraTransform(const glm::vec3& Target, const glm::vec3& Up)
    {
        glm::vec3 N = Target;
        N.Normalize();
        glm::vec3 U = Up;
        U.Normalize();
        U = U.Cross(N);
        glm::vec3 V = N.Cross(U);

        m[0][0] = U.x;   m[0][1] = U.y;   m[0][2] = U.z;   m[0][3] = 0.0f;
        m[1][0] = V.x;   m[1][1] = V.y;   m[1][2] = V.z;   m[1][3] = 0.0f;
        m[2][0] = N.x;   m[2][1] = N.y;   m[2][2] = N.z;   m[2][3] = 0.0f;
        m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;
    }
};
