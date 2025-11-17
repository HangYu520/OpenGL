#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tuple>

/*
* --------------
* 摄像机类
* --------------
*/
class Camera
{
public:
    // * 相机移动方式
    enum Movement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

private:
    glm::vec3 cameraPos; // 摄像机位置
    glm::vec3 cameraFront; // 摄像机朝向
    glm::vec3 cameraUp; // 摄像机上方向
    float yaw; // 摄像机初始角度
    float pitch; // 摄像机初始角度

public:
    // 构造函数
    Camera(const glm::vec3& pos)
    : cameraPos(pos), cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)), yaw(-90.0f), pitch(0.0f) {}
    Camera() = default;
    ~Camera() = default;

    Camera operator=(const Camera& otherCamera) // 重载赋值运算符
    {
        if (this != &otherCamera)
        {
            cameraPos = otherCamera.cameraPos;
            cameraFront = otherCamera.cameraFront;
            cameraUp = otherCamera.cameraUp;
            yaw = otherCamera.yaw;
            pitch = otherCamera.pitch;
        }
        return *this;
    }

    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
    
    // 设置摄像机初始参数
    void setCameraPos(const glm::vec3& pos) { cameraPos = pos; }
    void setCameraFront(const glm::vec3& front) { cameraFront = front; }
    void setCameraUp(const glm::vec3& up) { cameraUp = up; }
    void setYaw(float y) { yaw = y; }
    void setPitch(float p) { pitch = p; }

    // 调整相机参数
    void adjustCameraPos(Movement direction, float offset);
    
    void adjustCameraFront();
    
    void adjustYaw(float offset);
    
    void adjustPitch(float offset);
};