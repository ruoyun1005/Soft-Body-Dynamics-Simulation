#include<iostream>
#include <fstream>

#include <OpenGL/glu.h> //for mac
#include <GLFW/glfw3.h>

//OpenGL Mathematics (GLM)  https://glm.g-truc.net/
#include<glm/vec3.hpp>
#include<glm/vec4.hpp>
#include<glm/mat4x4.hpp>
#include<glm/mat3x3.hpp>
#include<Eigen/Dense>

#include "explicit_euler.h"
#include "inplicit_euler.h"
#include "mesh_generator.h"
#include "mesh_physics.h"
#include "grids.h"

using namespace std;
using namespace glm;
using namespace Eigen;

int winWidth = 1280;
int winHeight = 720;

float theta = 3.14159f / 4.0f;

bool running = false;

int k = 100;
int m = 2;
float dt = 0.02;

float x_0 = 0;
float v_0 = 5;

vec3 gravity = vec3(0.0f, -9.8f, 0.0f);
float restitution = 0.3f;

MatrixXf H, W;

void Display(GLFWwindow* window)
{
    int fbw, fbh;
    glfwGetFramebufferSize(window, &fbw, &fbh);   // Mac Retina 需要
    glViewport(0, 0, fbw, fbh);

    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(20.0, (double)winWidth / winHeight, 0.5, 40.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 5, 15,  0, -1, 0,  0, 1, 0);

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    for (const auto& e : edges_list) {
        const vec3& a = points_list[e.p1].x;
        const vec3& b = points_list[e.p2].x;
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
    }
    DrawGrid();
    glEnd();

    glfwSwapBuffers(window);
}
void init() {
    glClearColor(0, 0, 0, 0);
	glClearDepth(1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    mesh_generator();
    mesh_structure();
    H =  compute_H(edges_list);
    W = compute_W(dt, H, points_list[0].m);
    points_list[0].x += vec3(0.8f, 0.0f, 0.0f);
    cout<<gravity.x<<", "<<gravity.y<<", "<<gravity.z << endl;
}

void step_simulation(){
    accumulate_force(points_list, edges_list);
    apply_W(points_list, W, dt, H);
    integrate(points_list, dt, gravity);
    collide_floor(points_list, -2.0f, 0.3f);

    static int frame = 0;
if (++frame % 30 == 0) {
    vec3 c(0.0f);
    for (const auto& p : points_list) c += p.x;
    c /= (float)points_list.size();
    cout << "centroid: " << c.x << " " << c.y << " " << c.z << endl;
}
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

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    points_list[0].x += vec3(0.05f, 0.0f, 0.0f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Check if it's time to update (simulate timer)
        double currentTime = glfwGetTime();
        if (currentTime - previousTime >= interval) {
            // Here you can include any periodic update logic.
            // In GLUT you used glutPostRedisplay(), but in GLFW,
            // since you're in control of the loop, just call Display().
            if (running) step_simulation();
            previousTime = currentTime;
        }
        float pull = 0.05f;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            points_list[0].x += vec3(pull, 0.0f, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            points_list[0].x += vec3(-pull, 0.0f, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            points_list[0].x += vec3(0.0f, pull, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            points_list[0].x += vec3(0.0f, -pull, 0.0f);

        // Render here
        Display(window);

        // Poll for and process events
        glfwPollEvents();

        static bool enter_was_down = false;
        bool enter_down = (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS);
        if (enter_down && !enter_was_down)
            running = !running;        
        enter_was_down = enter_down;
    }

    // Clean up and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}