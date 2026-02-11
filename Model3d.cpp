#include "Model3D.h"
#include <glm/gtc/type_ptr.hpp>

// Initialize static members
GLuint Model3D::s_VAO = 0;
GLuint Model3D::s_VBO = 0;
GLuint Model3D::s_EBO = 0;
GLuint Model3D::s_indexCount = 0;

Model3D::Model3D()
    : position(0.0f, 0.0f, 0.0f),
    rotation(0.0f, 0.0f, 0.0f),
    scale(1.0f, 1.0f, 1.0f)
{
}

Model3D::~Model3D()
{
    // No cleanup needed - shared resources handled by static method
}

glm::mat4 Model3D::getTransformMatrix() const
{
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, position);
    transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, scale);
    return transform;
}

void Model3D::initializeSharedMesh(const float* vertices, unsigned int vertexCount,
    const unsigned int* indices, unsigned int indexCount)
{
    if (vertices == nullptr || indices == nullptr || vertexCount == 0 || indexCount == 0)
        return;

    s_indexCount = indexCount;

    // Generate VAO, VBO, EBO
    glGenVertexArrays(1, &s_VAO);
    glGenBuffers(1, &s_VBO);
    glGenBuffers(1, &s_EBO);

    // Bind VAO
    glBindVertexArray(s_VAO);

    // Bind and fill VBO
    glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);

    // Bind and fill EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // Vertex attribute pointer for position (location 0)
    // Each vertex is 3 floats (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO
    glBindVertexArray(0);
}

void Model3D::draw(GLuint shaderProgram, GLint transformLoc) const
{
    if (s_VAO == 0 || s_indexCount == 0)
        return;

    // Get and set transformation matrix
    glm::mat4 transform = getTransformMatrix();
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    // Bind and draw
    glBindVertexArray(s_VAO);
    glDrawElements(GL_TRIANGLES, s_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Model3D::cleanupSharedMesh()
{
    if (s_VAO != 0)
    {
        glDeleteVertexArrays(1, &s_VAO);
        s_VAO = 0;
    }
    if (s_VBO != 0)
    {
        glDeleteBuffers(1, &s_VBO);
        s_VBO = 0;
    }
    if (s_EBO != 0)
    {
        glDeleteBuffers(1, &s_EBO);
        s_EBO = 0;
    }
    s_indexCount = 0;
}