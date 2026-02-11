#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @class Camera
 * @brief FPS (First Person Shooter) style camera
 *
 * Features:
 * - Position, pitch (up/down), yaw (left/right) rotation
 * - WASD movement (forward, backward, strafe left, strafe right)
 * - Mouse/arrow controls for rotation
 * - Generates view and projection matrices
 */
class Camera
{
private:
    // Position
    glm::vec3 position;

    // Rotation in degrees
    float yaw;      // Left/right rotation (around Y axis)
    float pitch;    // Up/down rotation (around X axis)

    // Camera vectors
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 worldUp;

    // Camera settings
    float sensitivity;
    float speed;
    float fov;
    float aspect;
    float nearPlane;
    float farPlane;

    /**
     * Recalculate camera vectors based on pitch and yaw
     */
    void updateCameraVectors();

public:
    /**
     * Constructor: Initialize FPS camera
     * @param pos Initial position
     * @param worldUp World up vector (typically (0, 1, 0))
     */
    Camera(const glm::vec3& pos = glm::vec3(0.0f, 2.0f, 5.0f),
        const glm::vec3& worldUp = glm::vec3(0.0f, 1.0f, 0.0f));

    // ===== Movement =====
    /**
     * Move camera forward/backward
     * @param distance Distance to move (positive = forward, negative = backward)
     */
    void moveForward(float distance);

    /**
     * Move camera left/right (strafe)
     * @param distance Distance to move (positive = right, negative = left)
     */
    void moveRight(float distance);

    /**
     * Move camera up/down
     * @param distance Distance to move (positive = up, negative = down)
     */
    void moveUp(float distance);

    // ===== Rotation =====
    /**
     * Rotate camera based on mouse movement (like in FPS games)
     * @param xOffset Mouse X offset
     * @param yOffset Mouse Y offset
     */
    void rotate(float xOffset, float yOffset);

    /**
     * Set rotation directly via euler angles
     * @param yawAngle Yaw in degrees
     * @param pitchAngle Pitch in degrees
     */
    void setRotation(float yawAngle, float pitchAngle);

    // ===== Getters =====
    /**
     * Get camera position
     * @return Position vector
     */
    glm::vec3 getPosition() const { return position; }

    /**
     * Get camera front vector (where camera is looking)
     * @return Front vector
     */
    glm::vec3 getFront() const { return front; }

    /**
     * Get view matrix
     * @return 4x4 view matrix
     */
    glm::mat4 getViewMatrix() const;

    /**
     * Get projection matrix
     * @return 4x4 projection matrix
     */
    glm::mat4 getProjectionMatrix() const;

    // ===== Setters =====
    /**
     * Set camera position
     * @param pos New position
     */
    void setPosition(const glm::vec3& pos) { position = pos; }

    /**
     * Set movement speed
     * @param spd Speed value
     */
    void setSpeed(float spd) { speed = spd; }

    /**
     * Set mouse sensitivity
     * @param sens Sensitivity value
     */
    void setSensitivity(float sens) { sensitivity = sens; }

    /**
     * Set field of view
     * @param fovDegrees FOV in degrees
     */
    void setFOV(float fovDegrees) { fov = fovDegrees; }

    /**
     * Set aspect ratio
     * @param aspectRatio Width / Height
     */
    void setAspectRatio(float aspectRatio) { aspect = aspectRatio; }
};