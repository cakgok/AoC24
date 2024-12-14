#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>

#define GRID_ROWS 103
#define GRID_COLS 101

int getQuadrantSums(const std::vector<std::vector<int>>& grid) {
    int n = grid.size(); 
    int m = grid[0].size();
    std::vector<int> sums(4, 0);
    
    int midRow = n / 2;
    int midCol = m / 2;
    
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(i == midRow || j == midCol) continue; // Skip the middle row and column
            int quadrant = (i < midRow ? 0 : 2) + (j < midCol ? 0 : 1);
            sums[quadrant] += grid[i][j];
        }
    }
    
    return sums[0] * sums[1] * sums[2] * sums[3];
}

std::pair<int, int> solve(int x, int y, int vx, int vy, int moves) {
    int x_end = ((x + vx * moves) % GRID_COLS + GRID_COLS) % GRID_COLS;
    int y_end = ((y + vy * moves) % GRID_ROWS + GRID_ROWS) % GRID_ROWS;
    return {x_end, y_end};
}

int main () {
    std::ifstream file("input.txt");
    std::string line;
    std::vector<std::vector<int>> grid(GRID_ROWS, std::vector<int>(GRID_COLS, 0));
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> guards;
    while(std::getline(file, line)) {
        //p=62,65 v=-96,-93
        int x, y, vx, vy;
        sscanf(line.c_str(), "p=%d, %d v=%d, %d", &x, &y, &vx, &vy);
        guards.push_back({{x, y}, {vx, vy}});
    }

    for(int i = 0; i < guards.size(); i++) {
        auto [pos, velocity] = guards[i];
        auto [x, y] = pos;
        auto [vx, vy] = velocity;
        auto [x_end, y_end] = solve(x, y, vx, vy, 100);
        grid[y_end][x_end] += 1;
    }

    int sum = getQuadrantSums(grid);
    
    std::cout << sum << std::endl;

    std::pair<int, int> minScore = {INT_MAX, 0};    
    for(int i = 0; i < 20000; i++) {
        std::vector<std::vector<int>> grid2(GRID_ROWS, std::vector<int>(GRID_COLS, 0));
        for(int j = 0; j < guards.size(); j++) {
            auto [pos, velocity] = guards[j];
            auto [x, y] = pos;
            auto [vx, vy] = velocity;
            auto [x_end, y_end] = solve(x, y, vx, vy, i);
            grid2[y_end][x_end] += 1;
        }
        int score = getQuadrantSums(grid2);
        if(score < minScore.first) {
            minScore = {score, i};
        }
    }
    std::cout << minScore.second << std::endl;

    std::vector<std::vector<int>> grid3(GRID_ROWS, std::vector<int>(GRID_COLS, 0));

    for(int i = 0; i < guards.size(); i++) {
        auto [pos, velocity] = guards[i];
        auto [x, y] = pos;
        auto [vx, vy] = velocity;
        auto [x_end, y_end] = solve(x, y, vx, vy, minScore.second);
        grid3[y_end][x_end] += 1;
    }

    for(int i = 0; i < GRID_ROWS; i++) {
        for(int j = 0; j < GRID_COLS; j++) {
            if (grid3[i][j] > 0) std::cout << grid3[i][j];
            else std::cout << ".";
        }
        std::cout << std::endl;
    }

    return 0;
}

