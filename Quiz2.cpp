#include <iostream>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

// GLM (math)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// OpenGL/GLFW 
#include <glad/gl.h>
#include <GLFW/glfw3.h>

// TinyObjLoader
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// STB Image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

// Constants
const float WINDOW_WIDTH = 600.0f;
const float WINDOW_HEIGHT = 600.0f;
const string VERT_PATH = "Shaders/sample.vert";
const string FRAG_PATH = "Shaders/sample.frag";
const string MODEL_PATH = "3D/bunny.obj";
//const string TEXTURE_PATH = "3D/ayaya.png";
const float CAMERA_FOV = 60.0f;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 100.0f;
const glm::vec3 INITIAL_CAMERA_POS = glm::vec3(0.0f, 0.0f, 10.0f);
const glm::vec3 CAMERA_TARGET = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const float INITIAL_MODEL_Z = -5.0f;
const int NUM_INSTANCES = 3;
const float CIRCLE_RADIUS = 6.5f;

const float MODEL_SCALE = 9.0f;
const float ROTATE_TIME = 65.0f;

class Model3D {
public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE);
    float theta = 0.0f;
    glm::vec3 axisRotation = glm::vec3(0.0f, 1.0f, 0.0f);
};
Model3D model;

string loadShaderFromFile(const string& filepath)
{
    fstream file(filepath);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLFWwindow* createWindow(float width, float height, const string& title)
{
    if (!glfwInit()) {
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    return window;
}

GLuint loadAndCompileShaders(const string& vertpath, const string& fragpath)
{
    // Load shaders
    string vertString = loadShaderFromFile(vertpath);
    const char* vertChar = vertString.c_str();
    string fragString = loadShaderFromFile(fragpath);
    const char* fragChar = fragString.c_str();

    // Compile shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &vertChar, NULL);
    glShaderSource(fragmentShader, 1, &fragChar, NULL);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Clean-up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main(void)
{
    GLFWwindow* window = createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Erica Barundia");
    if (!window) return -1;

    gladLoadGL(glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);

    // Load shaders
    GLuint shaderProgram = loadAndCompileShaders(VERT_PATH, FRAG_PATH);

    // Load .obj file
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> material;
    string error;
    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &error,
        MODEL_PATH.c_str()
    );

    if (!success) {
        cout << "Error loading model: " << error << endl;
        return -1;
    }

    // Build mesh indices
    vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    // Setup VAO, VBO, EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * attributes.vertices.size(),
        &attributes.vertices[0],
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(GLfloat),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    // Setup projection matrix
    glm::mat4 projection = glm::perspective(
        glm::radians(CAMERA_FOV),
        WINDOW_WIDTH / WINDOW_HEIGHT,
        NEAR_PLANE,
        FAR_PLANE
    );

    // Setup view matrix
    glm::mat4 view = glm::lookAt(INITIAL_CAMERA_POS, CAMERA_TARGET, WORLD_UP);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Set uniform matrices once
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        double currentTime = glfwGetTime();
        float orbitAngle = currentTime * ROTATE_TIME;

        // Render multiple instances in a circle
        for (int i = 0; i < NUM_INSTANCES; i++)
        {
            float angle = (360.0f / NUM_INSTANCES) * i + orbitAngle;

            float circleX = CIRCLE_RADIUS * cos(glm::radians(angle));
            float circleY = CIRCLE_RADIUS * sin(glm::radians(angle));

            glm::mat4 identity_matrix = glm::mat4(1.0f);
            glm::mat4 transformation_matrix = glm::translate(identity_matrix,
                glm::vec3(circleX, circleY, INITIAL_MODEL_Z));
            transformation_matrix = glm::scale(transformation_matrix,
                glm::vec3(model.scale.x, model.scale.y, model.scale.z));

            // Fix orientation of bunny
            float tiltAngle = angle;
            transformation_matrix = glm::rotate(transformation_matrix,
                glm::radians(angle),
                glm::vec3(0.0f, 0.0f, 1.0f));

            unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

            // Render
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
        }

        // Swap and poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean-up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}