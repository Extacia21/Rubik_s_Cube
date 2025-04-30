#include <GL/glut.h>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const float CUBE_SIZE = 3.0f;
const float ROTATION_SPEED = 5.0f;

// More vibrant and saturated colors
GLfloat colors[6][3] = {
    {1.0f, 0.2f, 0.2f},  // Red (more saturated)
    {0.2f, 0.8f, 0.2f},  // Green (brighter)
    {0.2f, 0.5f, 1.0f},  // Blue (lighter)
    {1.0f, 1.0f, 0.0f},  // Yellow (pure)
    {1.0f, 0.6f, 0.0f},  // Orange (brighter)
    {0.9f, 0.9f, 0.9f}   // White (slightly off-white for better lighting)
};

// Face glow colors for extra vibrancy
GLfloat glowColors[6][3] = {
    {1.0f, 0.5f, 0.5f},  // Red glow
    {0.5f, 1.0f, 0.5f},  // Green glow
    {0.5f, 0.8f, 1.0f},  // Blue glow
    {1.0f, 1.0f, 0.5f},  // Yellow glow
    {1.0f, 0.8f, 0.5f},  // Orange glow
    {1.0f, 1.0f, 1.0f}   // White glow
};

struct Cubie {
    int faces[6];
    float x, y, z;
};

std::vector<Cubie> cubies;
float angle = 0;
int rotatingFace = -1;
bool isRotating = false;
float currentRotation = 0;
int rotationDirection = 1;

float cameraAngleX = 30.0f;
float cameraAngleY = -45.0f;
float cameraDistance = 10.0f;

void initCube() {
    cubies.clear();

    for (float x = -1; x <= 1; x++) {
        for (float y = -1; y <= 1; y++) {
            for (float z = -1; z <= 1; z++) {

                if (x == 0 && y == 0 && z == 0) continue;

                Cubie c;
                c.x = x;
                c.y = y;
                c.z = z;

                for (int i = 0; i < 6; i++) c.faces[i] = -1;

                if (x == -1) c.faces[0] = 0;
                if (x == 1) c.faces[1] = 1;
                if (y == -1) c.faces[2] = 2;
                if (y == 1) c.faces[3] = 3;
                if (z == -1) c.faces[4] = 4;
                if (z == 1) c.faces[5] = 5;

                cubies.push_back(c);
            }
        }
    }
}

void drawCubie(const Cubie& c) {
    float s = CUBE_SIZE / 3.0f * 0.95f;

    glPushMatrix();
    glTranslatef(c.x * CUBE_SIZE / 3.0f, c.y * CUBE_SIZE / 3.0f, c.z * CUBE_SIZE / 3.0f);

    for (int i = 0; i < 6; i++) {
        if (c.faces[i] == -1) continue;

        // Set material properties for more vibrant colors
        GLfloat matAmbient[] = {colors[c.faces[i]][0]*0.3, colors[c.faces[i]][1]*0.3, colors[c.faces[i]][2]*0.3, 1.0f};
        GLfloat matDiffuse[] = {colors[c.faces[i]][0], colors[c.faces[i]][1], colors[c.faces[i]][2], 1.0f};
        GLfloat matSpecular[] = {glowColors[c.faces[i]][0], glowColors[c.faces[i]][1], glowColors[c.faces[i]][2], 1.0f};
        GLfloat matShininess[] = {50.0f};

        glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

        switch(i) {
            case 0:
                glBegin(GL_QUADS);
                    glNormal3f(-1, 0, 0);
                    glVertex3f(-s, -s, -s);
                    glVertex3f(-s, -s, s);
                    glVertex3f(-s, s, s);
                    glVertex3f(-s, s, -s);
                glEnd();
                break;
            case 1:
                glBegin(GL_QUADS);
                    glNormal3f(1, 0, 0);
                    glVertex3f(s, -s, -s);
                    glVertex3f(s, s, -s);
                    glVertex3f(s, s, s);
                    glVertex3f(s, -s, s);
                glEnd();
                break;
            case 2:
                glBegin(GL_QUADS);
                    glNormal3f(0, -1, 0);
                    glVertex3f(-s, -s, -s);
                    glVertex3f(s, -s, -s);
                    glVertex3f(s, -s, s);
                    glVertex3f(-s, -s, s);
                glEnd();
                break;
            case 3:
                glBegin(GL_QUADS);
                    glNormal3f(0, 1, 0);
                    glVertex3f(-s, s, -s);
                    glVertex3f(-s, s, s);
                    glVertex3f(s, s, s);
                    glVertex3f(s, s, -s);
                glEnd();
                break;
            case 4:
                glBegin(GL_QUADS);
                    glNormal3f(0, 0, -1);
                    glVertex3f(-s, -s, -s);
                    glVertex3f(-s, s, -s);
                    glVertex3f(s, s, -s);
                    glVertex3f(s, -s, -s);
                glEnd();
                break;
            case 5:
                glBegin(GL_QUADS);
                    glNormal3f(0, 0, 1);
                    glVertex3f(-s, -s, s);
                    glVertex3f(s, -s, s);
                    glVertex3f(s, s, s);
                    glVertex3f(-s, s, s);
                glEnd();
                break;
        }
    }

    // Black edges with some shine
    GLfloat edgeAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat edgeDiffuse[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat edgeSpecular[] = {0.3f, 0.3f, 0.3f, 1.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, edgeAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, edgeDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, edgeSpecular);

    glLineWidth(2.0f);
    glutWireCube(s * 1.01f);

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(
        cameraDistance * sin(cameraAngleY * M_PI / 180) * cos(cameraAngleX * M_PI / 180),
        cameraDistance * sin(cameraAngleX * M_PI / 180),
        cameraDistance * cos(cameraAngleY * M_PI / 180) * cos(cameraAngleX * M_PI / 180),
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0
    );

    if (isRotating) {
        glPushMatrix();
        switch (rotatingFace) {
            case 0: glRotatef(currentRotation, -1, 0, 0); break;
            case 1: glRotatef(currentRotation, 1, 0, 0); break;
            case 2: glRotatef(currentRotation, 0, -1, 0); break;
            case 3: glRotatef(currentRotation, 0, 1, 0); break;
            case 4: glRotatef(currentRotation, 0, 0, -1); break;
            case 5: glRotatef(currentRotation, 0, 0, 1); break;
        }

        for (const auto& c : cubies) {
            bool shouldDraw = false;
            switch (rotatingFace) {
                case 0: shouldDraw = (c.x == -1); break;
                case 1: shouldDraw = (c.x == 1); break;
                case 2: shouldDraw = (c.y == -1); break;
                case 3: shouldDraw = (c.y == 1); break;
                case 4: shouldDraw = (c.z == -1); break;
                case 5: shouldDraw = (c.z == 1); break;
            }
            if (shouldDraw) drawCubie(c);
        }
        glPopMatrix();

        for (const auto& c : cubies) {
            bool shouldSkip = false;
            switch (rotatingFace) {
                case 0: shouldSkip = (c.x == -1); break;
                case 1: shouldSkip = (c.x == 1); break;
                case 2: shouldSkip = (c.y == -1); break;
                case 3: shouldSkip = (c.y == 1); break;
                case 4: shouldSkip = (c.z == -1); break;
                case 5: shouldSkip = (c.z == 1); break;
            }
            if (!shouldSkip) drawCubie(c);
        }
    } else {
        for (const auto& c : cubies) {
            drawCubie(c);
        }
    }

    glutSwapBuffers();
}

void updateCubeState() {
    std::vector<Cubie> newCubies = cubies;

    for (size_t i = 0; i < cubies.size(); i++) {
        Cubie& c = cubies[i];
        bool shouldRotate = false;

        switch (rotatingFace) {
            case 0: shouldRotate = (c.x == -1); break;
            case 1: shouldRotate = (c.x == 1); break;
            case 2: shouldRotate = (c.y == -1); break;
            case 3: shouldRotate = (c.y == 1); break;
            case 4: shouldRotate = (c.z == -1); break;
            case 5: shouldRotate = (c.z == 1); break;
        }

        if (shouldRotate) {
            float x = c.x, y = c.y, z = c.z;

            switch (rotatingFace) {
                case 0:
                    y = c.y * cos(rotationDirection * M_PI / 2) - c.z * sin(rotationDirection * M_PI / 2);
                    z = c.y * sin(rotationDirection * M_PI / 2) + c.z * cos(rotationDirection * M_PI / 2);
                    break;
                case 1:
                    y = c.y * cos(-rotationDirection * M_PI / 2) - c.z * sin(-rotationDirection * M_PI / 2);
                    z = c.y * sin(-rotationDirection * M_PI / 2) + c.z * cos(-rotationDirection * M_PI / 2);
                    break;
                case 2:
                    x = c.x * cos(-rotationDirection * M_PI / 2) - c.z * sin(-rotationDirection * M_PI / 2);
                    z = c.x * sin(-rotationDirection * M_PI / 2) + c.z * cos(-rotationDirection * M_PI / 2);
                    break;
                case 3:
                    x = c.x * cos(rotationDirection * M_PI / 2) - c.z * sin(rotationDirection * M_PI / 2);
                    z = c.x * sin(rotationDirection * M_PI / 2) + c.z * cos(rotationDirection * M_PI / 2);
                    break;
                case 4:
                    x = c.x * cos(rotationDirection * M_PI / 2) - c.y * sin(rotationDirection * M_PI / 2);
                    y = c.x * sin(rotationDirection * M_PI / 2) + c.y * cos(rotationDirection * M_PI / 2);
                    break;
                case 5:
                    x = c.x * cos(-rotationDirection * M_PI / 2) - c.y * sin(-rotationDirection * M_PI / 2);
                    y = c.x * sin(-rotationDirection * M_PI / 2) + c.y * cos(-rotationDirection * M_PI / 2);
                    break;
            }

            x = round(x);
            y = round(y);
            z = round(z);

            newCubies[i].x = x;
            newCubies[i].y = y;
            newCubies[i].z = z;

            int newFaces[6];
            for (int j = 0; j < 6; j++) newFaces[j] = c.faces[j];

            switch (rotatingFace) {
                case 0:
                    if (c.faces[2] != -1) newFaces[4] = c.faces[2];
                    if (c.faces[3] != -1) newFaces[5] = c.faces[3];
                    if (c.faces[4] != -1) newFaces[3] = c.faces[4];
                    if (c.faces[5] != -1) newFaces[2] = c.faces[5];
                    break;
                case 1:
                    if (c.faces[2] != -1) newFaces[5] = c.faces[2];
                    if (c.faces[3] != -1) newFaces[4] = c.faces[3];
                    if (c.faces[4] != -1) newFaces[2] = c.faces[4];
                    if (c.faces[5] != -1) newFaces[3] = c.faces[5];
                    break;
                case 2:
                    if (c.faces[0] != -1) newFaces[5] = c.faces[0];
                    if (c.faces[1] != -1) newFaces[4] = c.faces[1];
                    if (c.faces[4] != -1) newFaces[0] = c.faces[4];
                    if (c.faces[5] != -1) newFaces[1] = c.faces[5];
                    break;
                case 3:
                    if (c.faces[0] != -1) newFaces[4] = c.faces[0];
                    if (c.faces[1] != -1) newFaces[5] = c.faces[1];
                    if (c.faces[4] != -1) newFaces[1] = c.faces[4];
                    if (c.faces[5] != -1) newFaces[0] = c.faces[5];
                    break;
                case 4:
                    if (c.faces[0] != -1) newFaces[3] = c.faces[0];
                    if (c.faces[1] != -1) newFaces[2] = c.faces[1];
                    if (c.faces[2] != -1) newFaces[0] = c.faces[2];
                    if (c.faces[3] != -1) newFaces[1] = c.faces[3];
                    break;
                case 5:
                    if (c.faces[0] != -1) newFaces[2] = c.faces[0];
                    if (c.faces[1] != -1) newFaces[3] = c.faces[1];
                    if (c.faces[2] != -1) newFaces[1] = c.faces[2];
                    if (c.faces[3] != -1) newFaces[0] = c.faces[3];
                    break;
            }

            for (int j = 0; j < 6; j++) newCubies[i].faces[j] = newFaces[j];
        }
    }

    cubies = newCubies;
}

void timer(int value) {
    if (isRotating) {
        currentRotation += ROTATION_SPEED * rotationDirection;

        if (abs(currentRotation) >= 90) {
            isRotating = false;
            currentRotation = 0;
            updateCubeState();
            rotatingFace = -1;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (isRotating) return;

    switch (key) {
        case 'l': rotatingFace = 0; rotationDirection = 1; break;
        case 'L': rotatingFace = 0; rotationDirection = -1; break;
        case 'r': rotatingFace = 1; rotationDirection = 1; break;
        case 'R': rotatingFace = 1; rotationDirection = -1; break;
        case 'd': rotatingFace = 2; rotationDirection = 1; break;
        case 'D': rotatingFace = 2; rotationDirection = -1; break;
        case 'u': rotatingFace = 3; rotationDirection = 1; break;
        case 'U': rotatingFace = 3; rotationDirection = -1; break;
        case 'b': rotatingFace = 4; rotationDirection = 1; break;
        case 'B': rotatingFace = 4; rotationDirection = -1; break;
        case 'f': rotatingFace = 5; rotationDirection = 1; break;
        case 'F': rotatingFace = 5; rotationDirection = -1; break;

        case ' ': initCube(); break;

        case 27: exit(0); break;
    }

    if (rotatingFace != -1) {
        isRotating = true;
        currentRotation = 0;
    }

    glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT: cameraAngleY -= 5.0f; break;
        case GLUT_KEY_RIGHT: cameraAngleY += 5.0f; break;
        case GLUT_KEY_UP: cameraAngleX += 5.0f; break;
        case GLUT_KEY_DOWN: cameraAngleX -= 5.0f; break;
        case GLUT_KEY_PAGE_UP: cameraDistance -= 0.5f; break;
        case GLUT_KEY_PAGE_DOWN: cameraDistance += 0.5f; break;
    }

    if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
    if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;

    if (cameraDistance < 5.0f) cameraDistance = 5.0f;
    if (cameraDistance > 20.0f) cameraDistance = 20.0f;

    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void initGL() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);  // Darker background for better contrast
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);  // Allow color material tracking

    // Brighter light for more vibrant colors
    GLfloat lightPos[4] = {5.0f, 10.0f, 5.0f, 1.0f};
    GLfloat lightColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);

    // Global material properties
    GLfloat matSpecular[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat matShininess[] = {80.0f};

    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Colorful Rubik's Cube");

    initGL();
    initCube();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
