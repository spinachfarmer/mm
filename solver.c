#include "solver.h"
#include "API.h"

#define MAX_SIZE 16 

int distances[MAX_SIZE][MAX_SIZE];
int walls[MAX_SIZE + 1][MAX_SIZE + 1];
int visited[MAX_SIZE][MAX_SIZE];
int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { 1, 0, -1, 0 };
Heading currentHeading = NORTH;

int currentX = 0; // Mouse's starting X position
int currentY = 0; // Mouse's starting Y position

Action solver() {
    return floodFill();
}

// This is an example of a simple left wall following algorithm.
Action leftWallFollower() {
    if(API_wallFront()) {
        if(API_wallLeft()){
            return RIGHT;
        }
        return LEFT;
    }
    return FORWARD;
}


void init() {
  
}

void updateManhattanDistances(int goalX, int goalY, int width, int height) {
    // Simple flood fill to calculate Manhattan distances from the goal
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            distances[x][y] = abs(x - goalX) + abs(y - goalY);
            char distText[11];
            snprintf(distText, 11, "%d", distances[x][y]);
            API_setText(x, y, distText);
        }
    }
}

void detectAndUpdateWalls(int x, int y) {
    // Check for walls and update the maze data and display
    if (API_wallFront()) {
        walls[x][y+1] = 1; // Assume a simple representation
        API_setWall(x, y, 'n');  // Set wall in front
    }
    if (API_wallRight()) {
        walls[x+1][y] = 1; // Assume a simple representation
        API_setWall(x, y, 'e');  // Set wall in right
    }
    if (API_wallLeft()) {
        walls[x][y] = 1; // Assume a simple representation
        API_setWall(x, y, 'w');  // Set wall in left
    }
   
}

// Put your implementation of floodfill here!
Action floodFill() {
    int width = API_mazeWidth();
    int height = API_mazeHeight();

    if (currentX < 0 || currentX >= API_mazeWidth() || currentY < 0 || currentY >= API_mazeHeight()) {
        return IDLE; // Out of bounds or invalid state
    }

    visited[currentX][currentY] = 1; // Mark as visited
    distances[currentX][currentY] = 0; // Distance to goal (you can implement distance calculation based on your maze's goal)
    
    updateManhattanDistances(width / 2, height / 2, width, height);
    // Check surrounding cells
       
        // case where walls on both side 
        if (API_wallLeft() && API_wallRight && !API_wallFront()) {
            detectAndUpdateWalls(currentX, currentY);
            API_moveForward();
            currentY += 1;
        }
        // deadend
        if (API_wallLeft() && API_wallRight && API_wallFront()) {
            detectAndUpdateWalls(currentX, currentY);
            API_turnRight();
            API_turnRight();
            API_moveForward();
            currentY -= 1;
        }
        // wall in front and on the left
        if (API_wallLeft()&& API_wallFront()) {
            detectAndUpdateWalls(currentX, currentY);
            API_turnRight();
            API_moveForward();
            currentX += 1;
        }
        // wall in front and on the right
        if ( API_wallRight && API_wallFront()) {
            detectAndUpdateWalls(currentX, currentY);
            API_turnRight();
            API_moveForward();
            currentX -= 1;
        }
        // choice


     
    
    return IDLE; // If no valid moves are available, return IDLE
}