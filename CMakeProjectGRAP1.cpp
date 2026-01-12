#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

    GLfloat vertices[]{
        0.f,  0.5f, 0.f,  //> [0] (0, 0.5, 0)    top vertex
        -1.f, -0.5f, 0.f, //> [1] (-1, -0.5, 0)  bottom left verteex
        0.5f, -0.5f, 0.f  //> [2] (0.5, -0.5, 0) bottom right vertex
    
    };

    GLuint indices[]{
        0,1,2
    };

    GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

    // Strictly in order

    // currVAO = null
	glBindVertexArray(VAO);
    // currVAO = VAO

	// currVBO = null
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// currVBO = VBO <- GL_ARRAY_BUFFER
    // currVAO.push(currVBO)

	// Assigns vertex data to buffer (currVBO)
    glBufferData(GL_ARRAY_BUFFER,               // What type of buffer
		         sizeof(vertices),              // Size of data in bytes
                 vertices,                      // data
                 // Optimization only
	             GL_STATIC_DRAW                 // Static if almost no movement, Dynamic if constantly moving
                );

	// Tells OpenGL how to interpret vertex data (currVAO)
    glVertexAttribPointer(
		                0,                      // Index/Buffer Index (0 reserved for positions)
		                3,                      // size (num components) = 3 (x,y,z)
		                GL_FLOAT,               // type of data
                        GL_FALSE,               // normalized?
                        3 * sizeof(GLfloat),    // stride (size of data per vertex)
                        (void*)0                // array buffer offset
	);

    // Enable index 0
    glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(indices),
                 indices,
                 GL_STATIC_DRAW
	    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // currVBO = null
    glBindVertexArray(0);
    // currVAO = null
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

        /*
        // Primitive type, start index, number of vertices
		glDrawArrays(GL_TRIANGLES, 0, 3);
        */


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