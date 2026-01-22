// C++ library
#include <iostream>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

<<<<<<< Updated upstream
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
=======
// GLAD library
#include <glad/gl.h>
#include <GLFW/glfw3.h>

// TinyObjLoader library
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// GLM Mathematics library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
>>>>>>> Stashed changes

// Initialize transformation values
float x = 0.0f, y = 0.0f, z = 0.0f;
float x_scale = 1.0f, y_scale = 1.0f, z_scale = 1.0f;
float theta = 0.0f;
float axisv_x = 0.0f, axisv_y = 1.0f, axisv_z = 0.0f;

// Key callback function for handling input
void Key_Callback(GLFWwindow* window, // Pointer to GLFW window
	int key,                          // Key code of the pressed key
    int scancode,                     // Physical position of the key
	int action,                       // Either press/release/repeat event
    int mods)                         // Modifier keys 
{
	// Use PRESS or REPEAT so a single press and holding (auto-repeat) both work.
	bool down = (action == GLFW_HOLD) || (action == GLFW_REPEAT);

	// WASD = Movement X/Y
	if (key == GLFW_KEY_W && down)
		y += 0.1f;
	if (key == GLFW_KEY_S && down)
		y -= 0.1f;
	if (key == GLFW_KEY_A && down)
		x -= 0.1f;
	if (key == GLFW_KEY_D && down)
        x += 0.1f;

    // QE = Increase or Decrease Scale
    if (key == GLFW_KEY_Q && down)
    {
        x_scale -= 0.1f;
		y_scale -= 0.1f;
        z_scale -= 0.1f;
    }
    if (key == GLFW_KEY_E && down)
    {
		x_scale += 0.1f;
        y_scale += 0.1f;
		z_scale += 0.1f;
    }
   
	// Arrow Keys = Rotate Model 
    if (key == GLFW_KEY_UP && down)
    {
        axisv_x = 1.0f; axisv_y = 0.0f; axisv_z = 0.0f;
        theta -= 5.0f;
	}
    if (key == GLFW_KEY_LEFT && down)
    {
        axisv_x = 0.0f; axisv_y = 1.0f; axisv_z = 0.0f;
        theta -= 5.0f;
    }
    if (key == GLFW_KEY_RIGHT && down)
    {
        axisv_x = 0.0f; axisv_y = 1.0f; axisv_z = 0.0f;
        theta += 5.0f;
    }
    if (key == GLFW_KEY_DOWN && down)
    {
        axisv_x = 1.0f; axisv_y = 0.0f; axisv_z = 0.0f;
        theta += 5.0f;
    }

	// Escape Key = Close Window
    if (key == GLFW_KEY_ESCAPE && down)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Erica Barundia", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);

	glfwSetKeyCallback(window, Key_Callback);

    GLfloat vertices[]{
        0.f,  0.5f, 0.f,  //> [0] (0, 0.5, 0)    top vertex
        -1.f, -0.5f, 0.f, //> [1] (-1, -0.5, 0)  bottom left verteex
        0.5f, -0.5f, 0.f  //> [2] (0.5, -0.5, 0) bottom right vertex
    };

    GLuint indices[]{
        0,1,2
    };

<<<<<<< Updated upstream
=======
    // Load the vertex shader file
    fstream vertSrc("Shaders/sample.vert");
    stringstream vertBuffer;
	vertBuffer << vertSrc.rdbuf();
	string vertString = vertBuffer.str();
    const char* vertChar = vertString.c_str();

    // Load the fragment shader file
	fstream fragSrc("Shaders/sample.frag");
	stringstream fragBuffer;
	fragBuffer << fragSrc.rdbuf();
	string fragString = fragBuffer.str();
	const char* fragChar = fragString.c_str();

    // Load .obj file
    string path = "3D/bunny.obj";
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> material;
    string error;
	tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &error,
        path.c_str()
    );

	// Compile vertex and fragment shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertChar, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragChar, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	
    glLinkProgram(shaderProgram);

    // EBO array
    vector<GLuint> mesh_indices;

    // For loop for all indices
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

>>>>>>> Stashed changes
    GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

<<<<<<< Updated upstream
	// Assigns vertex data to buffer (currVBO)
    glBufferData(GL_ARRAY_BUFFER,               // What type of buffer
		         sizeof(vertices),              // Size of data in bytes
                 vertices,                      // data
                 // Optimization only
	             GL_STATIC_DRAW                 // Static if almost no movement, Dynamic if constantly moving
=======
    glBufferData(GL_ARRAY_BUFFER,
		         sizeof(GLfloat) * attributes.vertices.size(),
                 &attributes.vertices[0],
	             GL_STATIC_DRAW
>>>>>>> Stashed changes
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
                 sizeof(indices),
                 indices,
                 GL_STATIC_DRAW
	    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
		glm::mat4 identity_matrix = glm::mat4(1.0f);
		glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(x, y, z));
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(x_scale, y_scale, z_scale));
		transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta), glm::vec3(axisv_x, axisv_y, axisv_z));

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

		// Activate the shader program BEFORE setting uniforms
        glUseProgram(shaderProgram);
		unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

		glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    // Clean-up
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}