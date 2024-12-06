#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <bitset>

std::pair<int, int> findStart(const std::vector<std::string>& grid) {
    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[i].size(); j++) {
            if(grid[i][j] == '^') return {i, j};
        }
    }
    return {-1, -1};
}

int search(const std::vector<std::string>& grid, std::vector<bool>& visit, int x, int y) {
    int count = 0;
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    const int width = grid[0].size();
    
    int dir = 0;
    while(x >= 0 && x < grid.size() && y >= 0 && y < width) {
        int idx = x * width + y; // position state, don't need to store direction
        if(!visit[idx]) {
            count++;
            visit[idx] = true;
        }

        int nextX = x + directions[dir].first;
        int nextY = y + directions[dir].second;

        if(nextX < 0 || nextX >= grid.size() || nextY < 0 || nextY >= width) 
            return count;

        if(grid[nextX][nextY] == '#') {
            dir = (dir + 1) % 4;
            nextX = x + directions[dir].first;
            nextY = y + directions[dir].second;
        }

        x = nextX;
        y = nextY;
    }
    return count;
}

bool searchLoops(const std::vector<std::string>& grid, int x, int y) {
    const std::vector<std::pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    const int width = grid[0].size();
    const int height = grid.size();
    std::vector<bool> visited(height * width * 4, false);  // position + direction state
    
    int dir = 0;
    
    while(true) {
        int state = (x * width + y) * 4 + dir;
        
        if(visited[state]) return true;  // Found a loop
        visited[state] = true;

        int nextX = x + directions[dir].first;
        int nextY = y + directions[dir].second;

        if(nextX < 0 || nextX >= height || nextY < 0 || nextY >= width) return false;  // Escaped

        int turnCount = 0;
        while(grid[nextX][nextY] == '#') {
            dir = (dir + 1) % 4;
            turnCount++;
            nextX = x + directions[dir].first;
            nextY = y + directions[dir].second;
            if(turnCount == 4) return true; // dead end
        }

        x = nextX;
        y = nextY;
    }
    return true;  // Exceeded maximum steps
}

int main() {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    std::ifstream file("input.txt");
    std::vector<std::string> grid;
    std::string line;
    while(std::getline(file, line)) {
        grid.push_back(line);
    }

    auto parseEnd = std::chrono::high_resolution_clock::now();
    
    const int width = grid[0].size();
    const int height = grid.size();
    std::vector<bool> visit(height * width * 4, false);
    auto start = findStart(grid);
    int pos = search(grid, visit, start.first, start.second);
    
    auto part1End = std::chrono::high_resolution_clock::now();
    
    int loops = 0;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            int idx = i * width + j;
            if(!visit[idx]) continue;
            if(grid[i][j] == '.') {
                grid[i][j] = '#';    
                loops += searchLoops(grid, start.first, start.second);
                grid[i][j] = '.';
            }
        }
    }
    
    auto part2End = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> parseDiff = parseEnd - startTime;
    std::chrono::duration<double> part1Diff = part1End - parseEnd;
    std::chrono::duration<double> part2Diff = part2End - part1End;
    
    std::cout << "Parse  Time: " << parseDiff.count() << " s\n";
    std::cout << "Part 1 Time: " << part1Diff.count() << " s\n";
    std::cout << "Part 2 Time: " << part2Diff.count() << " s\n";
    std::cout << "Part 1 Result: " << pos << "\n";
    std::cout << "Part 2 Result: " << loops << "\n";
    
    return 0;
}