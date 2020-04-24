#include <GL/glew.h>
#include <GL/freeglut.h>
#include "util.h"
#include <stdio.h>
#include <math.h>

GLuint vbo;
GLuint ibo;
GLuint projMatrLoc;

void setUniformLocations() {
    projMatrLoc = glGetUniformLocation(prog, "projMatr");
}

void createBuffer();

void init() {
    createBuffer();

    GLuint shaders[] = {
        createShader("shader.vs", GL_VERTEX_SHADER),
        createShader("shader.fs", GL_FRAGMENT_SHADER)
    };
    int len = sizeof(shaders) / sizeof(shaders[0]);
    createProg(shaders, len);
    int i = 0;
    for (; i < len; i++) {
        glDeleteShader(shaders[i]);
    }

    setUniformLocations();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
}

void createBuffer() {
    GLushort indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    float vertices[] = {
         // vertices
        -1.0f, -1.0f, -2.0f, 1.0f, // left-bottom
        -1.0f,  1.0f, -2.0f, 1.0f, // left-top
         1.0f,  1.0f, -2.0f, 1.0f, // right-top
         1.0f, -1.0f, -2.0f, 1.0f, // right-bottom

         // colors
         1.0f, 0.0f, 0.0f, 1.0f, // red
         0.0f, 1.0f, 0.0f, 1.0f, // green
         1.0f, 0.0f, 1.0f, 1.0f, // purple
         1.0f, 1.0f, 0.0f, 1.0f, // yellow
    };
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void display() {
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(prog);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*) (4*4*sizeof(float)));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glUseProgram(0);
    glutSwapBuffers();
    glutPostRedisplay();
}

float toRad(float deg) {
    // 180 deg = pi
    // "deg" deg = x
    return deg * 3.141593f / 180.0f;
}

void reshape(int w, int h) {
    float n = 0.2f;
    float f = 1000.0f;
    float fov = toRad(90);
    float aspect = w / (float) h;
    float t = n * tanf(fov / 2.0f);
    float r = t * aspect;

    float projMatr[] = {
        n/r, 0,   0,              0,
        0,   n/t, 0,              0,
        0,   0,   (f+n)/(n-f),    -1,
        0,   0,   2.0f*n*f/(n-f), 0
    };
    printf("n=%.1f  f=%.1f  fov=%.2f  aspect=%.2f  t=%.2f  r=%.2f\n", n, f, fov, aspect, t, r);
    glUseProgram(prog);
    glUniformMatrix4fv(projMatrLoc, 1, GL_FALSE, projMatr);
    glUseProgram(0);
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void keyboard(unsigned char key, int x, int y) {
    (void) x;
    (void) y;
    if (key == 27) {
        glutLeaveMainLoop();
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Game");

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
        return 1;
    }
    printf("GL version: %s\n", glGetString(GL_VERSION));

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    init();
    glutMainLoop();
    return 0;
}
