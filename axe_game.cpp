#include <iostream>
#include "raylib.h"

// Window Properties
int widthInPixels{ 1000 };
int heightInPixels{ 800 };
int targetFps{ 60 };
Color textColor{ WHITE };
Color backgroundColor{ BLUE };

// Circle Properties
int circleCenterX{ 50 };
int circlCenterY{ 400 };
int radius{ 50 };
int circleSpeed{ 5 };
int circleDirection{ 1 };
Color circleColor{ WHITE };

int circleLeftEdge;
int circleRightEdge;
int circleTopEdge;
int circleBottomEdge;

// Axe Properties
int rectangleXpos{ 475 }; // Left side of Axe
int rectangleRightEdge;
int rectangleYpos{ 375 }; // Top side of Axe
int rectangleBottomEdge;

int rectangleWidth{ 100 };
int rectangleHeight{ 100 };
int axeSpeed{ 5 };
int axeDirection{ -1 };
Color axeColor{ RED };

// General Game Properties
int score{ 0 }; 
bool hasAxeContact{ false };

void checkUserInput();
void calculateCircleEdgeCoordinates();
void calculateRectangleEdgeCoordinates();
void moveAxe();
int axeChangeInPosition(int yCoordinate);
void generateDisplay();
void displayScore();
void calculateScore();
void displayGameOver();
void checkForAxeCollision();

int main() {

    InitWindow(widthInPixels, heightInPixels, "axe_game"); // Raylib function
    SetTargetFPS(targetFps);
    calculateCircleEdgeCoordinates();
    calculateRectangleEdgeCoordinates();

    // Game Start
    while (!WindowShouldClose()) {
        //setup screen
        BeginDrawing();
        ClearBackground(backgroundColor);

        if (hasAxeContact) {
            displayGameOver();
        }
        else {
            // Create Circle
            generateDisplay();

            // Move Objects
            checkUserInput();
            moveAxe();

            // Calculate Score
            calculateScore();
        }

        // tear down screen
        EndDrawing();
    }
}


void checkUserInput() {
    if (IsKeyDown(KEY_LEFT) && (circleLeftEdge - circleSpeed >= 0)) {
        circleCenterX -= circleSpeed;
    }
    if (IsKeyDown(KEY_RIGHT) && (circleRightEdge + circleSpeed <= widthInPixels)) {
        circleCenterX += circleSpeed;
    }
    if (IsKeyDown(KEY_UP) && (circleTopEdge - circleSpeed >= 0)) {
        circlCenterY -= circleSpeed;
    }
    if (IsKeyDown(KEY_DOWN) && (circleBottomEdge + circleSpeed <= heightInPixels)) {
        circlCenterY += circleSpeed;
    }
    calculateCircleEdgeCoordinates();
}


void calculateCircleEdgeCoordinates() {
    circleLeftEdge = circleCenterX - radius;
    circleRightEdge = circleCenterX + radius;
    circleTopEdge = circlCenterY - radius;
    circleBottomEdge = circlCenterY + radius;
}


void calculateRectangleEdgeCoordinates() {
    rectangleRightEdge = rectangleXpos + rectangleWidth;
    rectangleBottomEdge = rectangleYpos + rectangleHeight;
}


void moveAxe() {
    if ((axeChangeInPosition(rectangleBottomEdge) >= heightInPixels) || (axeChangeInPosition(rectangleYpos)) <= 0) {
        axeDirection *= -1;
    }
    if (score >= axeSpeed) {
        axeSpeed += 5;
        circleSpeed += 5;
    }

    rectangleYpos = axeChangeInPosition(rectangleYpos);
    calculateRectangleEdgeCoordinates();
}


int axeChangeInPosition(int yCoordinate) {
    return yCoordinate + (axeSpeed * axeDirection);
}

void generateDisplay() {
    displayScore();
    DrawCircle(circleCenterX, circlCenterY, radius, circleColor);
    DrawRectangle(rectangleXpos, rectangleYpos, rectangleWidth, rectangleHeight, axeColor);
}

void displayScore() {
    char baseString[] = "Score: ";
    char message[50];
    sprintf_s(message, "%s%d", baseString, score);
    DrawText(message, 900, 50, 20, WHITE);
}

void calculateScore() {
    checkForAxeCollision();
    if ((circleDirection > 0 && circleRightEdge == widthInPixels) || (circleDirection < 0 && circleLeftEdge == 0)) {
        ++score;
        circleDirection *= -1;
    }
}

void displayGameOver() {
    int width = widthInPixels * .15;
    int height = heightInPixels / 2;
    
    char baseString[] = "Game Over! Your Score: ";
    char message[75];
    sprintf_s(message, "%s%d", baseString, score);

    DrawText(message, width, height, 50, RED);
}

void checkForAxeCollision() {   
    bool LeftCircleOverRightAxe = circleLeftEdge <= rectangleRightEdge;
    bool UpperCircleOverLowerAxe = circleTopEdge <= rectangleBottomEdge;
    bool RightCircleOverLeftAxe = circleRightEdge >= rectangleXpos;
    bool BottomCircleOverUpperAxe = circleBottomEdge >= rectangleYpos;

    hasAxeContact = LeftCircleOverRightAxe && UpperCircleOverLowerAxe && RightCircleOverLeftAxe && BottomCircleOverUpperAxe;
}