#include <GLFW/glfw3.h>

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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f); // a
        glVertex2f(0.0f, 0.5f);   // b
		glVertex2f(0.5f, -0.5f);  // c

        glBegin(GL_TRIANGLES);
        glVertex2f(-1, 0);       // d
        glVertex2f(0.0f, 0.5f);  // b
        glVertex2f(-0.5f, -0.5f); // a
        
        glBegin(GL_TRIANGLES); // FBD
        glVertex2f(-1.1f, 0.6f); // f
        glVertex2f(0.0f, 0.5f);  // b
        glVertex2f(-1, 0);       // d

        glBegin(GL_TRIANGLES);
        glVertex2f(-1.1f, 0.6f); // f
        glVertex2f(0.0f, 0.5f);  // b
        glVertex2f(-0.75f, 1.2f); // h

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.75f, 1.2f); // h
        glVertex2f(0.0f, 0.5f);   // b
        glVertex2f(0, 1.5f);      // i

		glBegin(GL_TRIANGLES); // IBJ
        glVertex2f(0, 1.5f);      // i
        glVertex2f(0.0f, 0.5f);   // b
        glVertex2f(0.75f, 1.2f);  // j

        glBegin(GL_TRIANGLES); // JBG
		glVertex2f(0.75f, 1.2f);  // j
		glVertex2f(0.0f, 0.5f);   // b
		glVertex2f(1.1f, 0.6f);  // g

        glBegin(GL_TRIANGLES); // GBE
		glVertex2f(1.1f, 0.6f);  // g
		glVertex2f(0.0f, 0.5f);   // b
		glVertex2f(1, 0);        // e

        glBegin(GL_TRIANGLES); // EBC
		glVertex2f(1, 0);        // e
		glVertex2f(0.0f, 0.5f);   // b
		glVertex2f(0.5f, -0.5f);  // c

        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}