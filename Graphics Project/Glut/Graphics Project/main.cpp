#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Window dimensions
const int windowWidth = 900, windowHeight = 700;

// Mosquito variables
const int numMosquitoes = 10; // Number of mosquitoes
float mosquitoX[numMosquitoes], mosquitoY[numMosquitoes];
float mosquitoRedX[numMosquitoes], mosquitoRedY[numMosquitoes];
float mosquitoSpeedX[numMosquitoes], mosquitoSpeedY[numMosquitoes];
bool mosquitoBiting = false;

// Time of day (day-night cycle simulation)
float timeOfDay = 0.0f;

// Cleaning animation variables
float cleanerX = 0.5f, broomAngle = 0.0f;
bool broomDirection = true;

// Helicopter variables
float heliX = -1.0f, heliY = 0.6f, heliSpeed = 0.01f; // Initial position and speed

// Initialize OpenGL settings
void initGL() {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f); // Background color: sky blue
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    srand(static_cast<unsigned>(time(0))); // Seed for randomization

    // Initialize mosquito positions and speeds
    for (int i = 0; i < numMosquitoes; i++) {
        mosquitoX[i] = 0.6f + (rand() % 100) / 100.0f * 0.4f - 0.2f; // Around dust hill
        mosquitoY[i] = -0.2f + (rand() % 100) / 100.0f * 0.4f - 0.2f;
        mosquitoSpeedX[i] = ((rand() % 100) / 100.0f * 0.02f - 0.01f);
        mosquitoSpeedY[i] = ((rand() % 100) / 100.0f * 0.02f - 0.01f);
    }
    for (int i = 0; i < numMosquitoes; i++) {
        mosquitoRedX[i] = 0.6f + (rand() % 100) / 100.0f * 0.4f - 0.2f; // Around dust hill
        mosquitoRedY[i] = -0.2f + (rand() % 100) / 100.0f * 0.4f - 0.2f;
    }
}

// Draw a rectangle
void drawRectangle(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawSchool() {
   glClear(GL_COLOR_BUFFER_BIT);

    // Main central building
    glColor3f(0.6f, 0.8f, 0.9f); // Light blue
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -0.5f); // Bottom-left
    glVertex2f(0.2f, -0.5f);  // Bottom-right
    glVertex2f(0.2f, 0.2f);   // Top-right
    glVertex2f(-0.2f, 0.2f);  // Top-left
    glEnd();

    // Left wing
    glColor3f(0.7f, 0.9f, 1.0f); // Lighter blue
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f); // Bottom-left
    glVertex2f(-0.2f, -0.5f);  // Bottom-right
    glVertex2f(-0.2f, 0.0f);   // Top-right
    glVertex2f(-0.5f, 0.0f);   // Top-left
    glEnd();

    // Right wing
    glBegin(GL_QUADS);
    glVertex2f(0.2f, -0.5f); // Bottom-left
    glVertex2f(0.5f, -0.5f);  // Bottom-right
    glVertex2f(0.5f, 0.0f);   // Top-right
    glVertex2f(0.2f, 0.0f);   // Top-left
    glEnd();

    // Doorway
    glColor3f(0.8f, 0.6f, 0.4f); // Brown
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, -0.5f); // Bottom-left
    glVertex2f(0.05f, -0.5f);  // Bottom-right
    glVertex2f(0.05f, -0.3f);  // Top-right
    glVertex2f(-0.05f, -0.3f); // Top-left
    glEnd();

    // Windows on central building
    glColor3f(1.0f, 1.0f, 1.0f); // White
    for (float x = -0.15f; x <= 0.15f; x += 0.1f) {
        for (float y = -0.4f; y <= 0.1f; y += 0.1f) {
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + 0.05f, y);
            glVertex2f(x + 0.05f, y + 0.05f);
            glVertex2f(x, y + 0.05f);
            glEnd();
        }
    }

    // Windows on left wing
    for (float x = -0.45f; x <= -0.25f; x += 0.1f) {
        for (float y = -0.4f; y <= -0.1f; y += 0.1f) {
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + 0.05f, y);
            glVertex2f(x + 0.05f, y + 0.05f);
            glVertex2f(x, y + 0.05f);
            glEnd();
        }
    }

    // Windows on right wing
    for (float x = 0.25f; x <= 0.45f; x += 0.1f) {
        for (float y = -0.4f; y <= -0.1f; y += 0.1f) {
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + 0.05f, y);
            glVertex2f(x + 0.05f, y + 0.05f);
            glVertex2f(x, y + 0.05f);
            glEnd();
        }
    }

    // Flag pole
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.2f); // Bottom
    glVertex2f(0.0f, 0.4f); // Top
    glEnd();

    // Flag
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.4f);    // Bottom of the flag
    glVertex2f(0.1f, 0.35f);   // Right corner
    glVertex2f(0.0f, 0.35f);   // Top of the flag
    glEnd();

    glFlush();
}


void drawHospital() {
    // Hospital main building
    glColor3f(0.4f, 0.8f, 0.9f); // Light blue
    glBegin(GL_QUADS);
    glVertex2f(-0.9f, -0.5f); // Bottom-left
    glVertex2f(-0.3f, -0.5f);  // Bottom-right
    glVertex2f(-0.3f, 0.3f);   // Top-right
    glVertex2f(-0.9f, 0.3f);  // Top-left
    glEnd();

    // Roof
    glColor3f(1.0f, 0.0f, 0.4f); // Red
    glBegin(GL_TRIANGLES);
    glVertex2f(-1.0f, 0.3f); // Left
    glVertex2f(-0.2f, 0.3f);  // Right
    glVertex2f(-0.6f, 0.6f);  // Top
    glEnd();

    // Entrance
    glColor3f(1.0f, 0.8f, 0.4f); // Orange
    glBegin(GL_QUADS);
    glVertex2f(-0.62f, -0.5f); // Bottom-left
    glVertex2f(-0.52f, -0.5f);  // Bottom-right
    glVertex2f(-0.52f, -0.3f);  // Top-right
    glVertex2f(-0.62f, -0.3f); // Top-left
    glEnd();

    // Windows (left side)
    glColor3f(0.8f, 0.9f, 1.0f); // Light blue
    for (float x = -0.8f; x <= -0.7f; x += 0.1f) {
        for (float y = -0.4f; y <= 0.2f; y += 0.2f) {
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + 0.05f, y);
            glVertex2f(x + 0.05f, y + 0.1f);
            glVertex2f(x, y + 0.1f);
            glEnd();
        }
    }

    // Windows (right side)
    for (float x = -0.5f; x <= -0.4f; x += 0.1f) {
        for (float y = -0.4f; y <= 0.2f; y += 0.2f) {
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + 0.05f, y);
            glVertex2f(x + 0.05f, y + 0.1f);
            glVertex2f(x, y + 0.1f);
            glEnd();
        }
    }

    // Circular background for the red cross
    glColor3f(1.0f, 1.0f, 1.0f); // White
    float cx = -0.6f, cy = 0.45f, r = 0.1f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 100; ++i) {
        float angle = 2.0f * 3.14159f * i / 100;
        float x = cx + r * cos(angle);
        float y = cy + r * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    // Red cross
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    // Vertical bar
    glBegin(GL_QUADS);
    glVertex2f(-0.62f, 0.40f);
    glVertex2f(-0.58f, 0.40f);
    glVertex2f(-0.58f, 0.50f);
    glVertex2f(-0.62f, 0.50f);
    glEnd();

    // Horizontal bar
    glBegin(GL_QUADS);
    glVertex2f(-0.65f, 0.44f);
    glVertex2f(-0.55f, 0.44f);
    glVertex2f(-0.55f, 0.46f);
    glVertex2f(-0.65f, 0.46f);
    glEnd();
}

// Draw a complex dust hill
void drawDustHill() {
    glColor3f(0.6f, 0.4f, 0.2f); // Brown color for dust
    glBegin(GL_POLYGON);
    glVertex2f(0.4f, -0.5f);
    glVertex2f(0.5f, -0.1f);
    glVertex2f(0.6f, -0.2f);
    glVertex2f(0.7f, 0.1f);
    glVertex2f(0.8f, -0.2f);
    glVertex2f(0.9f, -0.1f);
    glVertex2f(1.0f, -0.5f);
    glEnd();
}

// Draw the sun
void drawSun() {
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for sun
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(0.8f + 0.1f * cos(angle), 0.8f + 0.1f * sin(angle));
    }
    glEnd();
}

// Draw clouds
void drawCloud(float x, float y) {
    glColor4f(1.0f, 1.0f, 1.0f, 0.8f); // White, semi-transparent clouds
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(x + 0.05f * cos(angle), y + 0.05f * sin(angle));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(x + 0.1f + 0.05f * cos(angle), y + 0.05f * sin(angle));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(x + 0.025f + 0.1f * cos(angle), y + 0.1f + 0.05f * sin(angle));
    }
    glEnd();
}

void drawRoad(float x, float y, float width, float height) {
    float borderThickness = 0.02f; // Thickness of yellow border
    float dashLength = 0.1f;       // Length of each white dash
    float dashGap = 0.05f;         // Gap between dashes

    // Draw the top yellow border
    glColor3f(1.0f, 0.9f, 0.1f); // Yellow color
    glBegin(GL_QUADS);
    glVertex2f(x - width / 2 - borderThickness, y + height / 2 + borderThickness);
    glVertex2f(x + width / 2 + borderThickness, y + height / 2 + borderThickness);
    glVertex2f(x + width / 2 + borderThickness, y + height / 2);
    glVertex2f(x - width / 2 - borderThickness, y + height / 2);
    glEnd();

    // Draw the bottom yellow border
    glBegin(GL_QUADS);
    glVertex2f(x - width / 2 - borderThickness, y - height / 2);
    glVertex2f(x + width / 2 + borderThickness, y - height / 2);
    glVertex2f(x + width / 2 + borderThickness, y - height / 2 - borderThickness);
    glVertex2f(x - width / 2 - borderThickness, y - height / 2 - borderThickness);
    glEnd();

    // Draw the left yellow border
    glBegin(GL_QUADS);
    glVertex2f(x - width / 2 - borderThickness, y + height / 2);
    glVertex2f(x - width / 2, y + height / 2);
    glVertex2f(x - width / 2, y - height / 2);
    glVertex2f(x - width / 2 - borderThickness, y - height / 2);
    glEnd();

    // Draw the right yellow border
    glBegin(GL_QUADS);
    glVertex2f(x + width / 2, y + height / 2);
    glVertex2f(x + width / 2 + borderThickness, y + height / 2);
    glVertex2f(x + width / 2 + borderThickness, y - height / 2);
    glVertex2f(x + width / 2, y - height / 2);
    glEnd();

    // Draw the main road surface
    glColor3f(0.2f, 0.2f, 0.2f); // Dark gray color
    glBegin(GL_QUADS);
    glVertex2f(x - width / 2, y + height / 2);
    glVertex2f(x + width / 2, y + height / 2);
    glVertex2f(x + width / 2, y - height / 2);
    glVertex2f(x - width / 2, y - height / 2);
    glEnd();

    // Draw the dashed white centerline
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    float currentX = x - width / 2;
    while (currentX < x + width / 2) {
        glBegin(GL_QUADS);
        glVertex2f(currentX, y + 0.01f); // Top of dash
        glVertex2f(currentX + dashLength, y + 0.01f);
        glVertex2f(currentX + dashLength, y - 0.01f); // Bottom of dash
        glVertex2f(currentX, y - 0.01f);
        glEnd();

        currentX += dashLength + dashGap; // Move to next dash position
    }
}


void drawVerticalRoad(float x, float y, float width, float height) {
    float borderThickness = 0.02f; // Thickness of yellow border
    float dashLength = 0.1f;       // Length of each white dash
    float dashGap = 0.05f;         // Gap between dashes

    // Draw the yellow border
    glColor3f(1.0f, 0.9f, 0.1f); // Yellow color
    glBegin(GL_QUADS);
    glVertex2f(x - width / 2 - borderThickness, y + height / 2);
    glVertex2f(x + width / 2 + borderThickness, y + height / 2);
    glVertex2f(x + width / 2 + borderThickness, y - height / 2);
    glVertex2f(x - width / 2 - borderThickness, y - height / 2);
    glEnd();

    // Draw the main road surface
    glColor3f(0.2f, 0.2f, 0.2f); // Dark gray color
    glBegin(GL_QUADS);
    glVertex2f(x - width / 2, y + height / 2);
    glVertex2f(x + width / 2, y + height / 2);
    glVertex2f(x + width / 2, y - height / 2);
    glVertex2f(x - width / 2, y - height / 2);
    glEnd();

    // Draw the dashed white centerline
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    float currentY = y + height / 2;
    while (currentY > y - height / 2) {
        glBegin(GL_QUADS);
        glVertex2f(x - 0.01f, currentY); // Left side of dash
        glVertex2f(x + 0.01f, currentY); // Right side of dash
        glVertex2f(x + 0.01f, currentY - dashLength); // Bottom-right of dash
        glVertex2f(x - 0.01f, currentY - dashLength); // Bottom-left of dash
        glEnd();

        currentY -= dashLength + dashGap; // Move to next dash position
    }
}





// Draw mosquito with decreased size
void drawMosquito(float x, float y) {
    // Body
    glColor3f(0.0f, 0.0f, 0.0f); // Black color
    glBegin(GL_POLYGON);
    glVertex2f(x - 0.015f, y); // Decreased size
    glVertex2f(x - 0.007f, y + 0.015f);
    glVertex2f(x + 0.007f, y + 0.015f);
    glVertex2f(x + 0.015f, y);
    glVertex2f(x + 0.007f, y - 0.015f);
    glVertex2f(x - 0.007f, y - 0.015f);
    glEnd();

    // Wings
    glColor4f(0.7f, 0.7f, 0.7f, 0.5f); // Semi-transparent gray for wings
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y);
    glVertex2f(x + 0.03f, y + 0.045f);
    glVertex2f(x - 0.015f, y + 0.03f);

    glVertex2f(x, y);
    glVertex2f(x - 0.03f, y + 0.045f);
    glVertex2f(x + 0.015f, y + 0.03f);
    glEnd();

    // Legs
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(x - 0.015f, y - 0.007f);
    glVertex2f(x - 0.03f, y - 0.04f);

    glVertex2f(x + 0.015f, y - 0.007f);
    glVertex2f(x + 0.03f, y - 0.04f);

    glVertex2f(x - 0.01f, y - 0.01f);
    glVertex2f(x - 0.02f, y - 0.05f);

    glVertex2f(x + 0.01f, y - 0.01f);
    glVertex2f(x + 0.02f, y - 0.05f);
    glEnd();

    // Antennae
    glBegin(GL_LINES);
    glVertex2f(x - 0.007f, y + 0.015f);
    glVertex2f(x - 0.02f, y + 0.03f);

    glVertex2f(x + 0.007f, y + 0.015f);
    glVertex2f(x + 0.02f, y + 0.03f);
    glEnd();
}

// Draw a red mosquito with decreased size
void drawRedMosquito(float x, float y) {
    // Body (Red color)
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glBegin(GL_POLYGON);
    glVertex2f(x - 0.015f, y); // Decreased size
    glVertex2f(x - 0.007f, y + 0.015f);
    glVertex2f(x + 0.007f, y + 0.015f);
    glVertex2f(x + 0.015f, y);
    glVertex2f(x + 0.007f, y - 0.015f);
    glVertex2f(x - 0.007f, y - 0.015f);
    glEnd();

    // Wings (Semi-transparent gray for wings)
    glColor4f(0.7f, 0.7f, 0.7f, 0.5f); // Semi-transparent gray for wings
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y);
    glVertex2f(x + 0.03f, y + 0.045f);
    glVertex2f(x - 0.015f, y + 0.03f);

    glVertex2f(x, y);
    glVertex2f(x - 0.03f, y + 0.045f);
    glVertex2f(x + 0.015f, y + 0.03f);
    glEnd();

    // Legs (Black color)
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(x - 0.015f, y - 0.007f);
    glVertex2f(x - 0.03f, y - 0.04f);

    glVertex2f(x + 0.015f, y - 0.007f);
    glVertex2f(x + 0.03f, y - 0.04f);

    glVertex2f(x - 0.01f, y - 0.01f);
    glVertex2f(x - 0.02f, y - 0.05f);

    glVertex2f(x + 0.01f, y - 0.01f);
    glVertex2f(x + 0.02f, y - 0.05f);
    glEnd();

    // Antennae (Black color)
    glBegin(GL_LINES);
    glVertex2f(x - 0.007f, y + 0.015f);
    glVertex2f(x - 0.02f, y + 0.03f);

    glVertex2f(x + 0.007f, y + 0.015f);
    glVertex2f(x + 0.02f, y + 0.03f);
    glEnd();
}



// Variables for the controlled mosquito
float controlledMosquitoX = 0.0f, controlledMosquitoY = 0.0f;
float cleanerHeadRadius = 0.05f; // Radius of the cleaner's head

// Handle keyboard input for mosquito movement
void handleKeyboardInput(int key, int x, int y) {
    float step = 0.05f; // Movement step size

    switch (key) {
    case GLUT_KEY_LEFT:
        controlledMosquitoX -= step;
        break;
    case GLUT_KEY_RIGHT:
        controlledMosquitoX += step;
        break;
    case GLUT_KEY_UP:
        controlledMosquitoY += step;
        break;
    case GLUT_KEY_DOWN:
        controlledMosquitoY -= step;
        break;
    }
    glutPostRedisplay(); // Redraw the screen after movement
}

// Collision detection between mosquito and cleaner's head
bool checkCollisionWithCleaner() {
    float dx = controlledMosquitoX - cleanerX;
    float dy = controlledMosquitoY - (-0.2f); // Cleaner head Y-position
    float distance = sqrt(dx * dx + dy * dy);
    return distance <= cleanerHeadRadius; // Collision if within the radius
}



// Render the controlled mosquito
void drawControlledMosquito() {
    drawRedMosquito(controlledMosquitoX, controlledMosquitoY);
}



// Draw cleaner
void drawCleaner() {
    // Head (Improved)
    glColor3f(1.9f, 0.8f, 0.6f); // Skin color for head
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(cleanerX + 0.05f * cos(angle), -0.2f + 0.05f * sin(angle));
    }
    glEnd();

    // Body (Improved)
    glColor3f(0.0f, 0.0f, 0.0f); // Black for body
    glBegin(GL_LINES);
    glVertex2f(cleanerX, -0.2f); // Neck to torso
    glVertex2f(cleanerX, -0.4f); // Torso to lower body

    // Legs (Improved)
    // Left leg
    glVertex2f(cleanerX, -0.4f);
    glVertex2f(cleanerX - 0.05f, -0.6f); // Left foot

    // Right leg
    glVertex2f(cleanerX, -0.4f);
    glVertex2f(cleanerX + 0.05f, -0.6f); // Right foot
    glEnd();

    // Arms with broom (Improved)
    // Left arm
    glBegin(GL_LINES);
    glVertex2f(cleanerX, -0.3f); // Shoulder to hand
    glVertex2f(cleanerX - 0.1f * cos(broomAngle), -0.3f - 0.1f * sin(broomAngle)); // Left hand (angled)

    // Right arm holding broom
    glVertex2f(cleanerX, -0.3f);
    glVertex2f(cleanerX + 0.1f * cos(broomAngle), -0.3f + 0.1f * sin(broomAngle)); // Right hand (angled)
    glEnd();

    // Broom's bristles (improved)
    glColor3f(0.5f, 0.25f, 0.0f); // Broom color
    glBegin(GL_LINES);
    glVertex2f(cleanerX + 0.1f * cos(broomAngle), -0.3f + 0.1f * sin(broomAngle)); // Broom handle
    glVertex2f(cleanerX + 0.2f * cos(broomAngle), -0.3f + 0.2f * sin(broomAngle)); // Broom handle end
    glEnd();

    // Broom bristles (extending the bristles)
    glBegin(GL_LINES);
    glVertex2f(cleanerX + 0.2f * cos(broomAngle), -0.3f + 0.2f * sin(broomAngle));
    glVertex2f(cleanerX + 0.25f * cos(broomAngle) + 0.03f, -0.3f + 0.95f * sin(broomAngle)); // Left bristle
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(cleanerX + 0.2f * cos(broomAngle), -0.3f + 0.2f * sin(broomAngle));
    glVertex2f(cleanerX + 0.25f * cos(broomAngle) - 0.03f, -0.3f + 0.95f * sin(broomAngle)); // Right bristle
    glEnd();


    if (checkCollisionWithCleaner()) {
        glColor3f(1.0f, 0.0f, 0.0f); // Red for head
    } else {
        glColor3f(1.0f, 0.8f, 0.6f); // Default skin color
    }
    // Draw the head
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(cleanerX + cleanerHeadRadius * cos(angle), -0.2f + cleanerHeadRadius * sin(angle));
    }
    glEnd();

}



void drawCircle(float cx, float cy, float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * 3.14159f * i / segments;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

void drawHelicopter(float x, float y) {
    float scale = 0.7f; // Scale factor to make the helicopter larger

    // Helicopter Body (Dark Blue with a rounded front)
    glColor3f(0.1f, 0.3f, 0.6f); // Dark blue color
    glBegin(GL_POLYGON);
    glVertex2f(x + 0.25f * scale, y - 0.06f * scale); // Bottom-left
    glVertex2f(x - 0.25f * scale, y - 0.06f * scale); // Bottom-right
    glVertex2f(x - 0.25f * scale, y + 0.06f * scale); // Top-right
    glVertex2f(x + 0.25f * scale, y + 0.06f * scale); // Top-left
    glEnd();

    // Cockpit (Rounded front in a lighter blue shade)
    glColor3f(0.2f, 0.5f, 0.8f); // Light blue color
    drawCircle(x + 0.25f * scale, y, 0.07f * scale, 100);

    // Windows (White circles for cockpit)
    glColor3f(1.0f, 1.0f, 1.0f); // White
    drawCircle(x + 0.15f * scale, y + 0.015f * scale, 0.03f * scale, 100); // Left window
    drawCircle(x + 0.27f * scale, y + 0.015f * scale, 0.03f * scale, 100); // Right window

    // Tail (Rectangle in a lighter blue shade)
    glColor3f(0.2f, 0.5f, 0.8f); // Light blue
    glBegin(GL_POLYGON);
    glVertex2f(x - 0.25f * scale, y + 0.03f * scale);
    glVertex2f(x - 0.45f * scale, y + 0.03f * scale);
    glVertex2f(x - 0.45f * scale, y - 0.03f * scale);
    glVertex2f(x - 0.25f * scale, y - 0.03f * scale);
    glEnd();

    // Tail Fin (Red triangle)
    glColor3f(0.8f, 0.1f, 0.1f); // Red color
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 0.45f * scale, y);
    glVertex2f(x - 0.55f * scale, y + 0.06f * scale);
    glVertex2f(x - 0.55f * scale, y - 0.06f * scale);
    glEnd();

    // Landing Legs (Dark Gray Rectangles)
    glColor3f(0.3f, 0.3f, 0.3f); // Dark gray
    glBegin(GL_POLYGON); // Left leg
    glVertex2f(x + 0.12f * scale, y - 0.06f * scale);
    glVertex2f(x + 0.10f * scale, y - 0.06f * scale);
    glVertex2f(x + 0.10f * scale, y - 0.12f * scale);
    glVertex2f(x + 0.12f * scale, y - 0.12f * scale);
    glEnd();

    glBegin(GL_POLYGON); // Right leg
    glVertex2f(x - 0.10f * scale, y - 0.06f * scale);
    glVertex2f(x - 0.12f * scale, y - 0.06f * scale);
    glVertex2f(x - 0.12f * scale, y - 0.12f * scale);
    glVertex2f(x - 0.10f * scale, y - 0.12f * scale);
    glEnd();

    // Landing Base (Black)
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_POLYGON);
    glVertex2f(x + 0.18f * scale, y - 0.12f * scale);
    glVertex2f(x - 0.18f * scale, y - 0.12f * scale);
    glVertex2f(x - 0.18f * scale, y - 0.14f * scale);
    glVertex2f(x + 0.18f * scale, y - 0.14f * scale);
    glEnd();

    // Main Rotor (Black blades)
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_POLYGON);
    glVertex2f(x - 0.35f * scale, y + 0.14f * scale);
    glVertex2f(x + 0.35f * scale, y + 0.14f * scale);
    glVertex2f(x + 0.35f * scale, y + 0.13f * scale);
    glVertex2f(x - 0.35f * scale, y + 0.13f * scale);
    glEnd();

    // Rotor Support
    glColor3f(0.3f, 0.3f, 0.3f); // Dark gray
    glBegin(GL_POLYGON);
    glVertex2f(x - 0.015f * scale, y + 0.06f * scale);
    glVertex2f(x + 0.015f * scale, y + 0.06f * scale);
    glVertex2f(x + 0.015f * scale, y + 0.13f * scale);
    glVertex2f(x - 0.015f * scale, y + 0.13f * scale);
    glEnd();

    // Tail Rotor (Black Fan Blades)
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_LINES);
    glVertex2f(x - 0.45f * scale, y);
    glVertex2f(x - 0.55f * scale, y); // Horizontal blade
    glVertex2f(x - 0.50f * scale, y + 0.05f * scale);
    glVertex2f(x - 0.50f * scale, y - 0.05f * scale); // Vertical blade
    glEnd();
}



// Function to draw outlined circle
void drawCircleOutline(float cx, float cy, float r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(num_segments); // Angle
        float x = r * cosf(theta); // X coordinate
        float y = r * sinf(theta); // Y coordinate
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}
void drawCar(float x, float y, float scale) {
   // Scale factor to make the car smaller
    float smallScale = scale * 0.7f;

    // Body of the Ambulance (White)
    glColor3f(1.0f, 1.0f, 1.0f); // White color for the body
    glBegin(GL_POLYGON);
    glVertex2f(x - 0.4f * smallScale, y - 0.1f * smallScale);
    glVertex2f(x + 0.4f * smallScale, y - 0.1f * smallScale);
    glVertex2f(x + 0.35f * smallScale, y + 0.1f * smallScale);
    glVertex2f(x - 0.35f * smallScale, y + 0.1f * smallScale);
    glEnd();

    // Roof of the Ambulance (Red)
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for roof
    glBegin(GL_POLYGON);
    glVertex2f(x - 0.2f * smallScale, y + 0.1f * smallScale);
    glVertex2f(x + 0.2f * smallScale, y + 0.1f * smallScale);
    glVertex2f(x + 0.15f * smallScale, y + 0.2f * smallScale);
    glVertex2f(x - 0.15f * smallScale, y + 0.2f * smallScale);
    glEnd();

    // Roof Lights (Blue)
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color for roof lights
    glBegin(GL_POLYGON); // First blue light on the roof
    glVertex2f(x - 0.1f * smallScale, y + 0.2f * smallScale);
    glVertex2f(x - 0.05f * smallScale, y + 0.2f * smallScale);
    glVertex2f(x - 0.05f * smallScale, y + 0.25f * smallScale);
    glVertex2f(x - 0.1f * smallScale, y + 0.25f * smallScale);
    glEnd();

    glBegin(GL_POLYGON); // Second blue light on the roof
    glVertex2f(x + 0.05f * smallScale, y + 0.2f * smallScale);
    glVertex2f(x + 0.1f * smallScale, y + 0.2f * smallScale);
    glVertex2f(x + 0.1f * smallScale, y + 0.25f * smallScale);
    glVertex2f(x + 0.05f * smallScale, y + 0.25f * smallScale);
    glEnd();

    // Windshield (Light Blue)
    glColor3f(0.2f, 0.6f, 0.9f); // Light blue color for the windshield
    glBegin(GL_POLYGON);
    glVertex2f(x - 0.1f * smallScale, y + 0.1f * smallScale);
    glVertex2f(x + 0.1f * smallScale, y + 0.1f * smallScale);
    glVertex2f(x + 0.05f * smallScale, y + 0.18f * smallScale);
    glVertex2f(x - 0.05f * smallScale, y + 0.18f * smallScale);
    glEnd();

    // Ambulance Red Cross (Symbol)
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for the cross
    glBegin(GL_POLYGON); // Horizontal part of the cross
    glVertex2f(x - 0.1f * smallScale, y + 0.05f * smallScale);
    glVertex2f(x + 0.1f * smallScale, y + 0.05f * smallScale);
    glVertex2f(x + 0.1f * smallScale, y + 0.15f * smallScale);
    glVertex2f(x - 0.1f * smallScale, y + 0.15f * smallScale);
    glEnd();

    glBegin(GL_POLYGON); // Vertical part of the cross
    glVertex2f(x - 0.05f * smallScale, y + 0.1f * smallScale);
    glVertex2f(x + 0.05f * smallScale, y + 0.1f * smallScale);
    glVertex2f(x + 0.05f * smallScale, y + 0.2f * smallScale);
    glVertex2f(x - 0.05f * smallScale, y + 0.2f * smallScale);
    glEnd();

    // Ambulance Stripe (Red)
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for the stripe
    glBegin(GL_POLYGON); // Side stripe
    glVertex2f(x - 0.4f * smallScale, y);
    glVertex2f(x + 0.4f * smallScale, y);
    glVertex2f(x + 0.35f * smallScale, y - 0.1f * smallScale);
    glVertex2f(x - 0.35f * smallScale, y - 0.1f * smallScale);
    glEnd();

    // Wheels (Black Circles)
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for the wheels
    drawCircle(x - 0.25f * smallScale, y - 0.1f * smallScale, 0.1f * smallScale, 100);
    drawCircle(x + 0.25f * smallScale, y - 0.1f * smallScale, 0.1f * smallScale, 100);

    // Wheel Rims (Gray Circles)
    glColor3f(0.5f, 0.5f, 0.5f); // Gray color for the wheel rims
    drawCircle(x - 0.25f * smallScale, y - 0.1f * smallScale, 0.05f * smallScale, 100);
    drawCircle(x + 0.25f * smallScale, y - 0.1f * smallScale, 0.05f * smallScale, 100);

    // Headlights (Yellow Circles)
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for the headlights
    drawCircle(x - 0.35f * smallScale, y, 0.03f * smallScale, 100);
    drawCircle(x + 0.35f * smallScale, y, 0.03f * smallScale, 100);

    // Rear Lights (Red Circles)
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for the rear lights
    drawCircle(x - 0.4f * smallScale, y - 0.08f * smallScale, 0.03f * smallScale, 100);
    drawCircle(x + 0.4f * smallScale, y - 0.08f * smallScale, 0.03f * smallScale, 100);
}





// Animate the mosquitoes
void animateMosquitoes() {
    for (int i = 0; i < numMosquitoes; i++) {
        mosquitoX[i] += mosquitoSpeedX[i];
        mosquitoY[i] += mosquitoSpeedY[i];

        // Reflect off boundaries
        if (mosquitoX[i] > 1.0f || mosquitoX[i] < -1.0f) mosquitoSpeedX[i] = -mosquitoSpeedX[i];
        if (mosquitoY[i] > 0.1f || mosquitoY[i] <= -1.0f) mosquitoSpeedY[i] = -mosquitoSpeedY[i];

    }
}


// Animate the broom
void animateBroom() {
    if (broomDirection) {
        broomAngle += 0.05f;
        if (broomAngle > 0.5f) broomDirection = false;
    } else {
        broomAngle -= 0.05f;
        if (broomAngle < -0.5f) broomDirection = true;
    }
}

// Animate the helicopter
void animateHelicopter() {
    heliX += heliSpeed;
    if (heliX > 1.0f) heliX = -1.0f; // Reset position when it moves off screen
}


// Update the display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawSchool();
    drawHospital();
    drawDustHill();
    drawSun();
    drawRoad(0.0f, -0.9f, 2.0f, 0.3f);
    drawVerticalRoad(0.0f, -0.63f, 0.17f, 0.25f);
    drawVerticalRoad(-0.58f, -0.63f, 0.17f, 0.25f);
    drawCar(0.6f, -0.88f, 0.8f);
    drawCloud(0.1f, 0.8f);
    drawCloud(0.3f, 0.85f);
    drawCloud(0.6f, 0.75f);

    drawCleaner();

    for (int i = 0; i < 15; i++) {
        drawMosquito(mosquitoX[i], mosquitoY[i]);
    }
    for (int i = 0; i < 5; i++) {
        drawRedMosquito(mosquitoRedX[i], mosquitoRedY[i]);
    }

    drawControlledMosquito(); // Draw the controlled red mosquito
    drawHelicopter(heliX, 0.8f);

    animateMosquitoes();
    animateBroom();
    animateHelicopter();

    glutSwapBuffers();
}







// Timer function for animation
void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // Call the timer every 16 milliseconds (60 FPS)
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Dust Hill Scene");
    initGL();

    glutDisplayFunc(display);
     glutSpecialFunc(handleKeyboardInput);
    glutTimerFunc(25, timer, 0);

    glutMainLoop();
    return 0;
}
