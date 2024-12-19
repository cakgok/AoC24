#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <array>
#include <cmath>

static const std::array<std::pair<int, int>, 128> directions = [] {
    std::array<std::pair<int, int>, 128> arr{};
    arr['^'] = {0, -1};
    arr['v'] = {0, 1};
    arr['<'] = {-1, 0};
    arr['>'] = {1, 0};
    return arr;
}();

bool moveLargeCasesHorizontal(std::vector<std::string>& grid, int x, int y, char dir) {
    auto [dx, dy] = directions[dir];
    int xNextNext = x + dx + dx;
    int xNext = x + dx;
    char curr = grid[y][x];

    if(grid[y][xNextNext] == '.') {
        grid[y][xNextNext] = grid[y][xNext];
        grid[y][xNext] = curr;
        return true;
    }
    
    if (grid[y][xNextNext] == '#') return false;
    
    bool moved = moveLargeCasesHorizontal(grid, xNextNext, y, dir);

    if(moved) {
        grid[y][xNextNext] = grid[y][xNext];
        grid[y][xNext] = curr;
        grid[y][x] = '.';
        return true;
    }

    return false;
}

bool canMove(std::vector<std::string>& grid, int x, int y, char dir) {
    auto [dx, dy] = directions[dir];

    int yNext = y + dy;
    int xLeft = (grid[y][x] == '[') ? x : x - 1;
    int xRight = xLeft + 1;
    if (grid[yNext][xLeft] == '#' || grid[yNext][xRight] == '#') return false;
    if (grid[yNext][xLeft] == '.' && grid[yNext][xRight] == '.') return true;
    
    bool moved = false;
    if (grid[yNext][xLeft] == '[' && grid[yNext][xRight] == ']') return moved = canMove(grid, xLeft, yNext, dir);

    bool movedLeft = true;
    bool movedRight = true;

    if (grid[yNext][xRight] == '#') return false;
    if (grid[yNext][xLeft] == '#') return false;

    if (grid[yNext][xLeft] == ']') movedLeft = canMove(grid, xLeft, yNext, dir);
    if (grid[yNext][xRight] == '[') movedRight = canMove(grid, xRight, yNext, dir);

    if (movedLeft && movedRight) return true;
    return false; 
}

void moveConnectedBoxes(std::vector<std::string>& grid, int x, int y, char dir) {
    auto [dx, dy] = directions[dir];

    int yNext = y + dy;
    int xLeft = (grid[y][x] == '[') ? x : x - 1;
    int xRight = xLeft + 1;

    if (grid[yNext][xLeft] == '.' && grid[yNext][xRight] == '.') {
        grid[yNext][xLeft] = '[';
        grid[yNext][xRight] = ']';
        grid[y][xLeft] = '.';
        grid[y][xRight] = '.';
        return;
    }

    // aligned boxes
    if (grid[yNext][xLeft] == '[' && grid[yNext][xRight] == ']') {
        moveConnectedBoxes(grid, xLeft, yNext, dir);
        grid[yNext][xLeft] = '[';
        grid[yNext][xRight] = ']';
        grid[y][xLeft] = '.';
        grid[y][xRight] = '.';
        return;
    }

    // misaligned boxes, 
    if (grid[yNext][xLeft] == ']') moveConnectedBoxes(grid, xLeft, yNext, dir);
    if (grid[yNext][xRight] == '[') moveConnectedBoxes(grid, xRight, yNext, dir);

    // move the box
    grid[yNext][xLeft] = '[';
    grid[yNext][xRight] = ']';
    grid[y][xLeft] = '.';
    grid[y][xRight] = '.';
}

bool moveLargeCasesVertical(std::vector<std::string>& grid, int x, int y, char dir) {
    // first we check if we can move the box, recursively, until the end
    // then we move it
    if (canMove(grid, x, y, dir)) {
        moveConnectedBoxes(grid, x, y, dir);
        return true;
    }
    return false;
}

void moveLarge(std::vector<std::string>& grid, int& x, int& y, char dir) {
    auto [dx, dy] = directions[dir];
    int newX = x + dx;
    int newY = y + dy;
    char curr = grid[y][x];

    if(grid[newY][newX] == '.') {
        grid[y][x] = '.';
        grid[newY][newX] = '@';
        x = newX;
        y = newY;
    }
    else if(grid[newY][newX] == '#') {
        return;
    }
    else {
        bool moved;
        if(dir == '<' || dir == '>') moved = moveLargeCasesHorizontal(grid, newX, newY, dir);
        if(dir == '^' || dir == 'v') moved = moveLargeCasesVertical(grid, newX, newY, dir);
        if(moved) {
            std::cout << "Moved" << std::endl;
            grid[y][x] = '.';
            grid[newY][newX] = '@';
            x = newX;
            y = newY;
        }
    }
}

int main () {
    std::ifstream file("input.txt");
    std::string line;
    std::vector<std::string> grid;
    std::string moves;
    bool parsingGrid = true;

    while(std::getline(file, line)) {
        if(line.empty()) {
            parsingGrid = false;
            continue;
        }
        
        if(parsingGrid) grid.push_back(line);
        else moves += line;
    }

    int x, y;
    bool found = false;
    for (int i = 0; i < grid.size() && !found; i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == '@') {
                x = i;
                y = j;
                found = true;
                break;
            }
        }
    }

    std::vector<std::string> gridLarge(grid.size(), std::string(grid[0].size() * 2, '.'));

    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[i].size(); j++) {
            if(grid[i][j] == '#') {
                gridLarge[i][j*2] = '#';
                gridLarge[i][j*2+1] = '#';
            }
            else if(grid[i][j] == '@') {
                gridLarge[i][j*2] = '@';
            }
            else if(grid[i][j] == 'O') {
                gridLarge[i][j*2] = '[';
                gridLarge[i][j*2+1] = ']';
            }
        }
    }

    x = y = 0;
    found = false;
    for(int i = 0; i < gridLarge.size() && !found; i++) {
        for(int j = 0; j < gridLarge[i].size(); j++) {
            if(gridLarge[i][j] == '@') {
                x = j;
                y = i;
                found = true;
                break;
            }
        }
    }

    for(auto& row: gridLarge) {
            std::cout << row << std::endl;
        }

    for(char dir: moves) {
        // std::cout << "Moving: " << dir << "(y, x)" << y << ',' << x << std::endl;
        moveLarge(gridLarge, x, y, dir);
        // for(auto& row: gridLarge) {
        //     std::cout << row << std::endl;
        // }
    }

    long long score = 0;
    for(int i = 0; i < gridLarge.size(); i++) {
        for(int j = 0; j < gridLarge[i].size(); j++) {
            if(gridLarge[i][j] == '[') {
                score += 100 * i + j;
            }
        }
    }
    
    std::cout << "Score: " << score << std::endl;

    return 0;
}