#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <array>

static const std::array<std::pair<int, int>, 128> directions = [] {
    std::array<std::pair<int, int>, 128> arr{};
    arr['^'] = {0, -1};
    arr['v'] = {0, 1};
    arr['<'] = {-1, 0};
    arr['>'] = {1, 0};
    return arr;
}();

bool moveCases(std::vector<std::string>& grid, int x, int y, char dir) {
    auto [dx, dy] = directions[dir];
    int newX = x + dx;
    int newY = y + dy;

    if(grid[newY][newX] == '.') {
        grid[newY][newX] = 'O';
        grid[y][x] = '.';
        return true;
    }
    else if (grid[newY][newX] == '#') return false;
    else {
        bool moved = moveCases(grid, newX, newY, dir);
        if(moved) {
            grid[newY][newX] = 'O';
            grid[y][x] = '.';
            return true;
        }
        return false;
    }
    return false;
}

void move(std::vector<std::string>& grid, int& x, int& y, char dir) {
    auto [dx, dy] = directions[dir];
    int newX = x + dx;
    int newY = y + dy;

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
        bool moved = moveCases(grid, newX, newY, dir);             
        if(moved) {
            grid[y][x] = '.';
            grid[newY][newX] = '@';
            x = newX;
            y = newY;
        }
    }
}

bool moveLargeCasesHorizontal(std::vector<std::string>& grid, int x, int y, char dir) {
    auto [dx, dy] = directions[dir];
    int xNextNext = x + dx + dx;
    int yNextNext = y + dy + dy;
    int yNext = y + dy;
    int xNext = x + dx;
    char curr = grid[y][x];

    if(grid[yNextNext][xNextNext] == '.') {
        grid[yNextNext][xNextNext] = grid[yNext][xNext];
        grid[yNext][xNext] = curr;
        return true;
    }
    else if (grid[yNextNext][xNextNext] == '#') return false;
    else {
        bool moved = moveLargeCasesHorizontal(grid, xNextNext, yNextNext, dir);
        if(moved) {
            grid[yNextNext][xNextNext] = grid[yNext][xNext];
            grid[yNext][xNext] = curr;
            grid[y][x] = '.';
            return true;
        }
        return false;
    }
    return false;
}

bool moveLargeCasesVertical(std::vector<std::string>& grid, int x, int y, char dir) {
    auto [dx, dy] = directions[dir];
    int yNext = y + dy;
    int xNext = x + dx;
    char curr = grid[y][x];

    int yOther = y, xOther = x;

    if(curr == '[') {
        yOther = y;
        xOther = x + 1;
    }
    if(curr == ']') {
        yOther = y;
        xOther = x - 1;
    }

    int yOtherNext = yOther + dy;
    int xOtherNext = xOther + dx;
    std::cout << "Curr: " << curr << " " << grid[yOther][xOther] << std::endl;
    std::cout << grid[yNext][xNext] << " " << grid[yOtherNext][xOtherNext] << std::endl;

    if(grid[yNext][xNext] == '.' && grid[yOtherNext][xOtherNext] == '.') {
        grid[yNext][xNext] = grid[y][x];
        grid[yOtherNext][xOtherNext] = grid[yOther][xOther];
        grid[y][x] = '.';
        grid[yOther][xOther] = '.';
        return true;
    }
    else if (grid[yNext][xNext] == '#' || grid[yOtherNext][xOtherNext] == '#') return false;
    else {
        bool moved;
        if(grid[yNext][xNext] == '[' && grid[yOtherNext][xOtherNext] == ']' || grid[yNext][xNext] == ']' && grid[yOtherNext][xOtherNext] == '[') {
        //if(grid[yNext][xNext] == ']' && grid[yOtherNext][xOtherNext] == '[') {
        
            moved = moveLargeCasesVertical(grid, xNext, yNext, dir) && moveLargeCasesVertical(grid, xOtherNext, yOtherNext, dir);
        }
        else {
            std::cout << "non mix case" << std::endl;
            moved = moveLargeCasesVertical(grid, xNext, yNext, dir);
        }
        if(moved) {
            std::cout << "moved" << std::endl;
            grid[yNext][xNext] = curr;
            grid[yOtherNext][xOtherNext] = grid[yOther][xOther];
            grid[y][x] = '.';
            grid[yOther][xOther] = '.';
            return true;
         }
         return false;
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

    std::vector<std::string> gridCopy = grid;

    //part 1
    for(char dir: moves) {
        move(grid, x, y, dir);
    }
    
    long long score = 0;
    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[i].size(); j++) {
            if(grid[i][j] == 'O') {
                score += 100 * i + j;
            }
        }
    }

    std::cout << "Score: " << score << std::endl;

    //part 2
    std::vector<std::string> gridLarge(grid.size(), std::string(grid[0].size() * 2, '.'));

    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[i].size(); j++) {
            if(gridCopy[i][j] == '#') {
                gridLarge[i][j*2] = '#';
                gridLarge[i][j*2+1] = '#';
            }
            else if(gridCopy[i][j] == '@') {
                gridLarge[i][j*2] = '@';
            }
            else if(gridCopy[i][j] == 'O') {
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
        std::cout << "Moving: " << dir << "(y, x)" << y << ',' << x << std::endl;
        moveLarge(gridLarge, x, y, dir);
        for(auto& row: gridLarge) {
            std::cout << row << std::endl;
        }
    }

    return 0;
}