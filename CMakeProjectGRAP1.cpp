// CMakeProjectGRAP1.cpp
// Main cpp file (removed comments for clarity)
// Last modified: 02/09/2026

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

const float SCALE_VALUE = 1.0f;
const float MOVEMENT_SPEED = 0.1f;
const float ROTATION_SPEED = 5.0f;
const float ZOOM_SPEED = 0.1f;

class Model3D {
public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(SCALE_VALUE, SCALE_VALUE, SCALE_VALUE);
    float theta = 0.0f;
    glm::vec3 axisRotation = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 setPosition(glm::vec3 pos) { position = pos;  };
    glm::vec3 getPosition() const { return position; };
};Model3D model;

class Camera {
private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float fov;
    float aspect;
    float nearPlane;
    float farPlane;
public:
    Camera(glm::vec3 pos, glm::vec3 tgt, glm::vec3 upVec, float fov = 60.0f) :
        position(pos), target(tgt), up(upVec), fov(fov),
        aspect(800.f / 800.f), nearPlane(0.1f), farPlane(100.f){}

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, target, up);
    }

    glm::mat4 getProjectionMatrix() const {
        return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
    }

    void setPosition(glm::vec3 pos) { position = pos; }
    void setTarget(glm::vec3 tgt) { target = tgt; }
    glm::vec3 getPosition()const { return position; }
};

string loadShaderFromFile(const string& filepath)
{
    fstream file(filepath);
    stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

// Key callback function for handling input
void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)      
{
	bool down = (action == GLFW_PRESS) || (action == GLFW_REPEAT);
               
	// WASD = Movement X/Y 
	if (key == GLFW_KEY_W && down)
		model.position.y += MOVEMENT_SPEED;
	if (key == GLFW_KEY_S && down)
		model.position.y -= MOVEMENT_SPEED;
	if (key == GLFW_KEY_A && down)
		model.position.x -= MOVEMENT_SPEED;
	if (key == GLFW_KEY_D && down)
        model.position.x += MOVEMENT_SPEED;

    // QE = Increase or Decrease Scale
    if (key == GLFW_KEY_Q && down)
    {
        model.scale.x -= MOVEMENT_SPEED;
        model.scale.y -= MOVEMENT_SPEED;
        model.scale.z -= MOVEMENT_SPEED;
    }
    if (key == GLFW_KEY_E && down)
    {
		model.scale.x += MOVEMENT_SPEED;
        model.scale.y += MOVEMENT_SPEED;
		model.scale.z += MOVEMENT_SPEED;
    }
   
	// Arrow Keys = Rotate Model 
    if (key == GLFW_KEY_UP && down)
    {
        model.axisRotation = glm::vec3(1.0f, 0.0f, 0.0f);
        model.theta -= 5.0f;
	}
    if (key == GLFW_KEY_LEFT && down)
    {
        model.axisRotation = glm::vec3(0.0f, 1.0f, 0.0f);
        model.theta -= ROTATION_SPEED;
    }
    if (key == GLFW_KEY_RIGHT && down)
    {
        model.axisRotation = glm::vec3(0.0f, 1.0f, 0.0f);
        model.theta += ROTATION_SPEED;
    }
    if (key == GLFW_KEY_DOWN && down)
    {
        model.axisRotation = glm::vec3(1.0f, 0.0f, 0.0f);
        model.theta += ROTATION_SPEED;
    }

	// Zoom In/Out = Z/C
    if (key == GLFW_KEY_Z && down)
		model.position.z += ZOOM_SPEED;
	if (key == GLFW_KEY_C && down)
		model.position.z -= ZOOM_SPEED;

	// Escape Key = Close Window
    if (key == GLFW_KEY_ESCAPE && down)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
    // Initialize GLFW
    GLFWwindow* window;

    if (!glfwInit())
        return -1;
    
    const float WINDOW_WIDTH = 800.0f;
    const float WINDOW_HEIGHT = 800.0f;
    int img_width,
        img_height,
        colorChannels;
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Erica Barundia", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

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

    GLfloat UV[]{
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 1.f,
        0.f, 0.f
    };

    stbi_set_flip_vertically_on_load(true);

    unsigned char* tex_bytes = 
		stbi_load("3D/ayaya.png",       // path to image
			      &img_width,           // fills out the image width
			      &img_height,          // fills out the image height
                  &colorChannels,       // fills out the color channel (3 | 4)
                  0                     // border (fills out the color channel)
		);

	// Load/dump texture into OpenGL from unsigned char array 
	// OpenGL texture reference
    GLuint texture;
    // Generate a reference
	glGenTextures(1, &texture);
	// Set the current texture to the one we just generated/working on (Texture 0)
    glActiveTexture(GL_TEXTURE0);
	// Bind our next tasks to Texture 0
    // To our current reference
	// Similar to what we're doing to VAO/VBO/EBO
	glBindTexture(GL_TEXTURE_2D, texture);

	// Assign the loaded image to the OpenGL Texture or the OpenGL reference
    glTexImage2D(GL_TEXTURE_2D,
        0,                              // Texture 0
        GL_RGBA,                        // or RGB - target color format of the texture | PNG: RGBA, JPG: RGB
        img_width,              
        img_height,
		0,                              // Border
		GL_RGBA,                        // or RGB - format of the loaded image (should match above) | PNG: RGBA, JPG: RGB
		GL_UNSIGNED_BYTE,               // Type of data
		tex_bytes                       // The actual image data
    );

	// Generate Mipmaps to the current texture
    glGenerateMipmap(GL_TEXTURE_2D);
    // Free up the loaded bytes
	stbi_image_free(tex_bytes);

    glEnable(GL_DEPTH_TEST);

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
    string path = "3D/myCube.obj";
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

	// add a new VBO for the UVs
    GLuint VAO, VBO, EBO, VBO_UV;
    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO_UV);
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
                 GL_STATIC_DRAW	    );

    glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * (sizeof(UV) / sizeof(UV[0])),
        &UV[0],
        GL_DYNAMIC_DRAW
    );

	// Add in how to interpret the UV data
    glVertexAttribPointer(2, 
        2, 
        GL_FLOAT, 
        GL_FALSE, 
        2 * sizeof(float), 
        (void*)0
    );

	// Enable 2 for our UV / Texture Coordinates
    glEnableVertexAttribArray(2);

    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f),               // Field of View Angle - DEFAULT 60
        WINDOW_WIDTH / WINDOW_HEIGHT,        // Aspect Ratio
		0.1f,                              // Near Plane (must be > 0)
        100.0f                             // Far Plane
    );

	model.position.z = -5.0f; // Move the model away from the camera

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 identity_matrix = glm::mat4(1.0f);
		glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(model.position.x, model.position.y, model.position.z));
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(model.scale.x, model.scale.y, model.scale.z));
		transformation_matrix = glm::rotate(transformation_matrix, glm::radians(model.theta), glm::vec3(model.axisRotation.x, model.axisRotation.y, model.axisRotation.z));

        /* Render here */
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
        glm::mat4 cameraPosMatrix =
			glm::translate(identity_matrix, cameraPos * -1.0f);

		glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraCenter = glm::vec3(0.0f, 3.f, 0.f);

        // F - model
        glm::vec3 F = cameraCenter - cameraPos;
		F = glm::normalize(F);
        
        // R - view
		glm::vec3 R = glm::cross(F, WorldUp);
		R = glm::normalize(R);

        // U - projection
		glm::vec3 U = glm::cross(R, F);
		U = glm::normalize(U);

        // Construct the orientation matrix
		glm::mat4 cameraRotationMatrix = glm::mat4(1.0f); // identity matrix
        
        // Matrix [Col] [Row]
		// R
        cameraRotationMatrix[0][0] = R.x;
		cameraRotationMatrix[1][0] = R.y;
		cameraRotationMatrix[2][0] = R.z;
		// U
        cameraRotationMatrix[0][1] = U.x;
        cameraRotationMatrix[1][1] = U.y;
        cameraRotationMatrix[2][1] = U.z;
		// -F
        cameraRotationMatrix[0][2] = -F.x;
        cameraRotationMatrix[1][2] = -F.y;
        cameraRotationMatrix[2][2] = -F.z;

        // V = cR * cP
		//glm::mat4 view = cameraRotationMatrix * cameraPosMatrix;
        
        glm::mat4 view = glm::lookAt(cameraPos, cameraCenter, WorldUp);

        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

		// Activate the shader program BEFORE setting uniforms
        glUseProgram(shaderProgram);
		unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

		// Get location of projection matrix uniform and set matrix
		unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
		// Apply the projection matrix
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Get location of view matrix uniform and set matrix
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        // Apply the view matrix
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);                                                                                  

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