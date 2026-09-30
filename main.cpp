#include<iostream>
#include <fstream>

#include <OpenGL/glu.h> //for mac
#include <GLFW/glfw3.h>

//OpenGL Mathematics (GLM)  https://glm.g-truc.net/
#include<glm/vec3.hpp>
#include<glm/vec4.hpp>
#include<glm/mat4x4.hpp>
#include<glm/mat3x3.hpp>

#include "explicit_euler.h"
#include "inplicit_euler.h"
#include "mesh_generator.h"
#include "mesh_physics.h"

using namespace std;
using namespace glm;

int winWidth = 1280;
int winHeight = 720;


int k = 100;
int m = 2;
float dt = 0.5;

float x_0 = 0;
float v_0 = 5;

void Display(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(20.0, (double)winWidth / winHeight, 0.5, 40.0);

    glFlush();
    glfwSwapBuffers(window);
}

void init() {
	// float x_curr = x_0;
    // float v_curr = v_0;

    // ofstream outfile("inplicit_result.csv");
    // for(int i = 0; i<=200; i++){
    //     Point result = inplicit_euler(x_curr, v_curr, k, m, dt);
    //     x_curr = result.x;
    //     v_curr = result.v;

    //     outfile << i << "," << x_curr << endl;
    // };
    mesh_generator();

}


int main(void)
{
    
   
    // Initialize GLFW
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "trans: Press F1 to add a tetrahedron", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize OpenGL
    init();

    // Timing for periodic updates (~33ms interval)
    double previousTime = glfwGetTime();
    const double interval = 0.033; // ~33ms

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Check if it's time to update (simulate timer)
        double currentTime = glfwGetTime();
        if (currentTime - previousTime >= interval) {
            // Here you can include any periodic update logic.
            // In GLUT you used glutPostRedisplay(), but in GLFW,
            // since you're in control of the loop, just call Display().
            previousTime = currentTime;
        }

        // Render here
        Display(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}