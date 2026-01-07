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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Calculate nonagon vertices
        float radius = 0.9f;
        float centerX = 0.0f;
        float centerY = 1.0f - radius; // Position so top vertices touch y=1.0
        float angleOffset = M_PI / 2.0f + M_PI / 9.0f; // Top side horizontal

        float vertices[9][2];
        for (int i = 0; i < 9; i++) {
            float angle = angleOffset + (2.0f * M_PI * i) / 9.0f;
            vertices[i][0] = centerX + radius * cos(angle);
            vertices[i][1] = centerY + radius * sin(angle);
        }

        // Draw triangles from center to each edge
        glBegin(GL_TRIANGLES);
        glVertex2f(centerX, centerY);
        glVertex2f(vertices[0][0], vertices[0][1]);
        glVertex2f(vertices[1][0], vertices[1][1]);

        glBegin(GL_TRIANGLES);
        glVertex2f(centerX, centerY);
        glVertex2f(vertices[1][0], vertices[1][1]);
        glVertex2f(vertices[2][0], vertices[2][1]);

        glBegin(GL_TRIANGLES);
        glVertex2f(centerX, centerY);
        glVertex2f(vertices[2][0], vertices[2][1]);
        glVertex2f(vertices[3][0], vertices[3][1]);

        glBegin(GL_TRIANGLES);
        glVertex2f(centerX, centerY);
        glVertex2f(vertices[3][0], vertices[3][1]);
        glVertex2f(vertices[4][0], vertices[4][1]);

        glBegin(GL_TRIANGLES);
        glVertex2f(centerX, centerY);
        glVertex2f(vertices[4][0], vertices[4][1]);
        glVertex2f(vertices[5][0], vertices[5][1]);

        glBegin(GL_TRIANGLES);
        glVertex2f(centerX, centerY);
        glVertex2f(vertices[5][0], vertices[5][1]);
        glVertex2f(vertices[6][0], vertices[6][1]);

        glBegin(GL_TRIANGLES);
        glVertex2f(centerX, centerY);
        glVertex2f(vertices[6][0], vertices[6][1]);
        glVertex2f(vertices[7][0], vertices[7][1]);

        glBegin(GL_TRIANGLES);
        glVertex2f(centerX, centerY);
        glVertex2f(vertices[7][0], vertices[7][1]);
        glVertex2f(vertices[8][0], vertices[8][1]);

        glBegin(GL_TRIANGLES);
        glVertex2f(centerX, centerY);
        glVertex2f(vertices[8][0], vertices[8][1]);
        glVertex2f(vertices[0][0], vertices[0][1]);

        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}