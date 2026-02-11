/**
 * @file CMakeProjectGRAP1.cpp
 * @author Erica Barundia
 * @date February 2026
 *
 * GDGRAP1 Programming Challenge 1
 *
 * Description:
 * This program demonstrates 3D rendering with object-oriented design:
 * - Custom Model3D class for managing individual model instances
 * - FPS camera with WASD keyboard controls
 * - Model spawning system (Space key) with 3-second cooldown
 * - Perspective projection (45° FOV)
 * - Single vertex and fragment shader for all models
 *
 * 3D Model Credit:
 * Low-Poly Mccree by Youssef Zidan
 * Source: https://sketchfab.com/3d-models/low-poly-mccree-38aedc02c0b2412babdc4d0eac7c6803
 * License: CC Attribution
 *
 * Controls:
 * - W/S: Move forward/backward
 * - A/D: Strafe left/right
 * - Arrow Keys: Rotate camera view
 * - Space: Spawn model in front of camera (3 second cooldown)
 * - ESC: Exit application
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

 // GLM (mathematics library)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// OpenGL/GLFW
#include <glad/gl.h>
#include <GLFW/glfw3.h>

// File loading
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// Custom classes
#include "Model3D.h"
#include "Camera.h"

using namespace std;

// ===== WINDOW CONSTANTS =====
const float WINDOW_WIDTH = 1280.0f;
const float WINDOW_HEIGHT = 720.0f;
const string WINDOW_TITLE = "GDGRAP1 - Programming Challenge 1 - Barundia";

// ===== FILE PATHS =====
const string SHADER_VERT_PATH = "Shaders/sample.vert";
const string SHADER_FRAG_PATH = "Shaders/sample.frag";
const string MODEL_PATH = "3D/mccree.obj";
const string MODEL_MTL_DIR = "3D/";

// ===== GLOBAL VARIABLES =====
Camera* g_camera = nullptr;
vector<Model3D> g_spawnedModels;
GLuint g_shaderProgram = 0;

// Timing for spawn cooldown
auto g_lastSpawnTime = chrono::high_resolution_clock::now();
const float SPAWN_COOLDOWN = 3.0f;  // 3 seconds between spawns

// ===== SHADER LOADING UTILITIES =====

/**
 * Load shader source code from file
 * @param filepath Path to shader file
 * @return Shader source as string, empty if failed
 */
string loadShaderFromFile(const string& filepath)
{
    fstream file(filepath);
    if (!file.is_open())
    {
        cerr << "ERROR: Could not open shader file: " << filepath << endl;
        return "";
    }

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/**
 * Compile and link vertex and fragment shaders into a program
 * @param vertPath Path to vertex shader file
 * @param fragPath Path to fragment shader file
 * @return OpenGL shader program ID, or 0 if failed
 */
GLuint loadAndCompileShaders(const string& vertPath, const string& fragPath)
{
    // Load shader source code
    string vertString = loadShaderFromFile(vertPath);
    string fragString = loadShaderFromFile(fragPath);

    if (vertString.empty() || fragString.empty())
    {
        cerr << "ERROR: Failed to load shader files" << endl;
        return 0;
    }

    const char* vertChar = vertString.c_str();
    const char* fragChar = fragString.c_str();

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertChar, NULL);
    glCompileShader(vertexShader);

    // Check vertex shader for compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cerr << "ERROR: Vertex shader compilation failed:\n" << infoLog << endl;
    }

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragChar, NULL);
    glCompileShader(fragmentShader);

    // Check fragment shader for compilation errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cerr << "ERROR: Fragment shader compilation failed:\n" << infoLog << endl;
    }

    // Link shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cerr << "ERROR: Shader program linking failed:\n" << infoLog << endl;
    }

    // Delete shader objects (program has them compiled now)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    cout << "Shaders compiled and linked successfully" << endl;
    return shaderProgram;
}

// ===== MODEL LOADING =====

/**
 * Load 3D model from OBJ file using tinyobjloader
 * Automatically loads associated .mtl material file if referenced
 * @param filepath Path to .obj file
 * @param outVertices Output vector for vertex positions (as floats)
 * @param outIndices Output vector for face indices
 * @return True if loading successful, false otherwise
 */
bool loadOBJModel(const string& filepath, vector<float>& outVertices, vector<unsigned int>& outIndices)
{
    tinyobj::attrib_t attributes;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    string error;

    // Load OBJ file with material directory specified
    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &materials,
        &error,
        filepath.c_str(),
        MODEL_MTL_DIR.c_str()
    );

    if (!success)
    {
        cerr << "ERROR: Failed to load OBJ file: " << error << endl;
        return false;
    }

    if (shapes.empty())
    {
        cerr << "ERROR: OBJ file contains no shapes" << endl;
        return false;
    }

    // Extract vertex positions directly as floats
    outVertices = attributes.vertices;

    // Extract face indices from first shape
    for (size_t i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        outIndices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    cout << "Model loaded successfully:" << endl;
    cout << "  - Vertices: " << (outVertices.size() / 3) << endl;
    cout << "  - Indices: " << outIndices.size() << endl;

    return true;
}

// ===== WINDOW MANAGEMENT =====

/**
 * Create GLFW window and initialize OpenGL context
 * @param width Window width in pixels
 * @param height Window height in pixels
 * @param title Window title
 * @return GLFW window pointer, or nullptr if failed
 */
GLFWwindow* createWindow(float width, float height, const string& title)
{
    // Initialize GLFW
    if (!glfwInit())
    {
        cerr << "ERROR: GLFW initialization failed" << endl;
        return nullptr;
    }

    // Create window
    GLFWwindow* window = glfwCreateWindow((int)width, (int)height, title.c_str(), NULL, NULL);
    if (!window)
    {
        cerr << "ERROR: Window creation failed" << endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    // Load OpenGL function pointers using Glad
    if (!gladLoadGL(glfwGetProcAddress))
    {
        cerr << "ERROR: Failed to load OpenGL functions" << endl;
        return nullptr;
    }

    cout << "Window created and OpenGL initialized successfully" << endl;
    return window;
}

// ===== INPUT CALLBACKS =====

/**
 * Keyboard input callback
 * Handles:
 * - WASD for camera movement (forward/backward, strafe)
 * - Arrow keys for camera rotation
 * - Space to spawn models with cooldown
 * - ESC to exit
 */
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Only process key presses and repeats, not releases
    bool pressed = (action == GLFW_PRESS) || (action == GLFW_REPEAT);
    if (!pressed) return;

    // ===== CAMERA MOVEMENT (WASD) =====
    if (key == GLFW_KEY_W)
        g_camera->moveForward(1.0f);
    if (key == GLFW_KEY_S)
        g_camera->moveForward(-1.0f);
    if (key == GLFW_KEY_A)
        g_camera->moveRight(-1.0f);
    if (key == GLFW_KEY_D)
        g_camera->moveRight(1.0f);

    // ===== CAMERA ROTATION (ARROW KEYS) =====
    if (key == GLFW_KEY_UP)
        g_camera->rotate(0.0f, 5.0f);    // Look up
    if (key == GLFW_KEY_DOWN)
        g_camera->rotate(0.0f, -5.0f);   // Look down
    if (key == GLFW_KEY_LEFT)
        g_camera->rotate(-5.0f, 0.0f);   // Look left
    if (key == GLFW_KEY_RIGHT)
        g_camera->rotate(5.0f, 0.0f);    // Look right

    // ===== MODEL SPAWNING (SPACE) =====
    if (key == GLFW_KEY_SPACE)
    {
        auto currentTime = chrono::high_resolution_clock::now();
        float elapsed = chrono::duration<float>(currentTime - g_lastSpawnTime).count();

        // Check if 3-second cooldown has passed
        if (elapsed >= SPAWN_COOLDOWN)
        {
            // Create new model instance
            Model3D newModel;

            // Position in front of camera (5 units forward)
            glm::vec3 spawnPos = g_camera->getPosition() + g_camera->getFront() * 5.0f;
            newModel.setPosition(spawnPos);
            newModel.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
            newModel.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));

            // Add to spawned models list (no need to initialize mesh - uses static)
            g_spawnedModels.push_back(newModel);
            g_lastSpawnTime = currentTime;

            cout << "Model spawned at (" << spawnPos.x << ", " << spawnPos.y << ", " << spawnPos.z << ")" << endl;
            cout << "Total models: " << g_spawnedModels.size() << endl;
        }
    }

    // ===== EXIT (ESC) =====
    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// ===== MAIN PROGRAM =====

int main(void)
{
    cout << "========================================" << endl;
    cout << "GDGRAP1 Programming Challenge 1" << endl;
    cout << "3D Model Viewer with FPS Camera" << endl;
    cout << "========================================" << endl << endl;

    // Create window and initialize OpenGL
    cout << "Initializing window..." << endl;
    GLFWwindow* window = createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    if (!window)
        return -1;

    cout << "\nInitializing camera..." << endl;
    // Initialize camera at (0, 2, 8) looking towards origin
    g_camera = new Camera(glm::vec3(0.0f, 2.0f, 8.0f));
    g_camera->setAspectRatio(WINDOW_WIDTH / WINDOW_HEIGHT);
    g_camera->setFOV(45.0f);  // 45 degree field of view

    // Set input callback
    glfwSetKeyCallback(window, keyCallback);

    // Load shaders
    cout << "Loading shaders..." << endl;
    g_shaderProgram = loadAndCompileShaders(SHADER_VERT_PATH, SHADER_FRAG_PATH);
    if (g_shaderProgram == 0)
    {
        cerr << "FATAL ERROR: Failed to load shaders" << endl;
        glfwTerminate();
        return -1;
    }

    // Load 3D model
    cout << "Loading 3D model..." << endl;
    vector<float> modelVertices;
    vector<unsigned int> modelIndices;
    if (!loadOBJModel(MODEL_PATH, modelVertices, modelIndices))
    {
        cerr << "FATAL ERROR: Failed to load model" << endl;
        glfwTerminate();
        return -1;
    }

    // Initialize the shared mesh (call once before creating any models)
    cout << "Initializing shared mesh..." << endl;
    Model3D::initializeSharedMesh(modelVertices.data(), modelVertices.size() / 3,
        modelIndices.data(), modelIndices.size());

    // Enable depth testing for 3D rendering
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    // Spawn initial model
    cout << "Spawning initial model..." << endl;
    Model3D initialModel;
    initialModel.setPosition(glm::vec3(0.0f, 0.0f, -5.0f));
    initialModel.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    initialModel.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    g_spawnedModels.push_back(initialModel);

    cout << "\n========================================" << endl;
    cout << "Controls:" << endl;
    cout << "  W/S     - Move forward/backward" << endl;
    cout << "  A/D     - Strafe left/right" << endl;
    cout << "  Arrows  - Rotate camera view" << endl;
    cout << "  Space   - Spawn model (3s cooldown)" << endl;
    cout << "  ESC     - Exit application" << endl;
    cout << "========================================\n" << endl;

    // ===== MAIN RENDER LOOP =====
    while (!glfwWindowShouldClose(window))
    {
        // Clear color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader program
        glUseProgram(g_shaderProgram);

        // Get uniform locations
        GLint transformLoc = glGetUniformLocation(g_shaderProgram, "transform");
        GLint viewLoc = glGetUniformLocation(g_shaderProgram, "view");
        GLint projLoc = glGetUniformLocation(g_shaderProgram, "projection");

        // Calculate and set view matrix from camera
        glm::mat4 view = g_camera->getViewMatrix();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // Calculate and set perspective projection matrix
        glm::mat4 projection = g_camera->getProjectionMatrix();
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Draw all spawned models
        for (auto& model : g_spawnedModels)
        {
            model.draw(g_shaderProgram, transformLoc);
        }

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Process events (like keyboard input)
        glfwPollEvents();
    }

    // ===== CLEANUP =====
    cout << "\nCleaning up..." << endl;

    // Clean up shared mesh resources
    Model3D::cleanupSharedMesh();

    // Delete shader program
    glDeleteProgram(g_shaderProgram);

    // Clean up camera
    delete g_camera;

    // Terminate GLFW
    glfwTerminate();

    cout << "Application terminated successfully" << endl;
    return 0;
}