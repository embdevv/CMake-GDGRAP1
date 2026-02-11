#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>

/**
 * @class Model3D
 * @brief Handles 3D model transformation and rendering
 *
 * This class manages:
 * - Position (X, Y, Z)
 * - Rotation (X, Y, Z) in degrees for each axis
 * - Scale (X, Y, Z)
 * - Uses a shared VAO/VBO/EBO (set once via static method)
 */
class Model3D
{
private:
    // Transform properties
    glm::vec3 position;
    glm::vec3 rotation;      // In degrees (X, Y, Z)
    glm::vec3 scale;

    // Static shared mesh data (all models use the same mesh)
    static GLuint s_VAO;
    static GLuint s_VBO;
    static GLuint s_EBO;
    static GLuint s_indexCount;

public:
    /**
     * Constructor: Initialize model with default values
     */
    Model3D();

    /**
     * Destructor: Clean up (static resources handled separately)
     */
    ~Model3D();

    // ===== Transform Setters =====
    void setPosition(const glm::vec3& pos) { position = pos; }
    void setRotation(const glm::vec3& rot) { rotation = rot; }
    void setScale(const glm::vec3& scl) { scale = scl; }

    // ===== Transform Getters =====
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getRotation() const { return rotation; }
    glm::vec3 getScale() const { return scale; }

    /**
     * Calculate and return the transformation matrix
     * Combines translation, rotation (X, Y, Z), and scale
     * @return 4x4 transformation matrix
     */
    glm::mat4 getTransformMatrix() const;

    /**
     * Static method: Initialize shared mesh data (call once before creating models)
     * @param vertices Vertex position data
     * @param vertexCount Number of vertices
     * @param indices Mesh indices for rendering
     * @param indexCount Number of indices
     */
    static void initializeSharedMesh(const float* vertices, unsigned int vertexCount,
        const unsigned int* indices, unsigned int indexCount);

    /**
     * Draw the model using the provided shader program
     * Uses the shared VAO/VBO/EBO
     * @param shaderProgram OpenGL shader program ID
     * @param transformLoc Uniform location for transformation matrix
     */
    void draw(GLuint shaderProgram, GLint transformLoc) const;

    /**
     * Static method: Clean up all shared OpenGL resources
     * Call this once at the end of the program
     */
    static void cleanupSharedMesh();
};