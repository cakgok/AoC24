#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>

int findPaths(std::vector<std::string>& grid, int row, int col, int search, std::vector<std::vector<int>>& visit) {
    if(row < 0 || row >= grid.size() || col < 0 || col >= grid[0].size()) return 0; 
    if(grid[row][col] != search + '0') return 0;
    if(visit[row][col]) return visit[row][col];
    if(search == 9 && grid[row][col] == '9') return visit[row][col] = 1;

    int count = 0;

    count +=  findPaths(grid, row + 1, col, search + 1, visit);
    count +=  findPaths(grid, row - 1, col, search + 1, visit);
    count +=  findPaths(grid, row, col + 1, search + 1, visit);
    count +=  findPaths(grid, row, col - 1, search + 1, visit);

    return visit[row][col] = count;
}

int traverse(std::vector<std::string>& grid, int row, int col, int search, std::vector<std::vector<bool>>& visit) {
    if(row < 0 || row >= grid.size() || col < 0 || col >= grid[0].size()) return 0; 
    if(grid[row][col] != search + '0') return 0;
    if(search == 9 && grid[row][col] == '9') {
        if(visit[row][col]) return 0;
        visit[row][col] = 1;
        return 1;
    }
    int count = 0;
    count += traverse(grid, row + 1, col, search + 1, visit);
    count += traverse(grid, row - 1, col, search + 1, visit);
    count += traverse(grid, row, col + 1, search + 1, visit);
    count += traverse(grid, row, col - 1, search + 1, visit);
    return count;
}

int main() {
    std::ifstream file("input.txt");
    std::vector<std::string> grid;
    std::string line;
    while(std::getline(file, line)){
        grid.push_back(line);
    }
    
    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<bool>> visit(grid.size(), std::vector<bool>(grid[0].size(), false));

    int score = 0;
    int score2 = 0;
    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[i].size(); j++) {
            if(grid[i][j] == '0') {
                score += traverse(grid, i, j, 0, visit);
                for(auto& row : visit) {
                    std::fill(row.begin(), row.end(), false);
                }                
            }
        }
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Part 1 Execution Time: " << std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() << "uS" << std::endl;
    std::cout << "Part 1: " << score << std::endl;

    std::vector<std::vector<int>> visited(grid.size(), std::vector<int>(grid[0].size(), 0)); //int instead of bool to store the number of paths
    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[i].size(); j++) {
            if(grid[i][j] == '0') {
                score2 += findPaths(grid, i, j, 0, visited);
            }
        }
    }
    auto endTime2 = std::chrono::high_resolution_clock::now();
    std::cout << "Part 2 Execution Time: " << std::chrono::duration_cast<std::chrono::microseconds>(endTime2 - endTime).count() << "uS" << std::endl;
    std::cout << "Part 2: " << score2 << std::endl;
}