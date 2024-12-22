#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <chrono>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <unordered_set>
struct PairHash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const {
        std::size_t seed = 0;
        seed ^= std::hash<T1>{}(pair.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<T2>{}(pair.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

// BFS to find all reachable points within maxMoves, possibly overkill
std::vector<std::pair<int, int>> generateTeleportPattern(int maxMoves) {
    std::vector<std::pair<int, int>> pattern;
    std::vector<std::pair<int, int>> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    
    std::queue<std::pair<int, int>> q;
    std::set<std::pair<int, int>> visited;
    
    q.push({0, 0});
    visited.insert({0, 0});
    
    int moves = 0;
    while (!q.empty() && moves < maxMoves) {
        int levelSize = q.size();
        
        for (int i = 0; i < levelSize; i++) {
            auto [r, c] = q.front();
            q.pop();
            
            for (const auto& [dr, dc] : dirs) {
                int newR = r + dr;
                int newC = c + dc;
                std::pair<int, int> newPos = {newR, newC};
                
                if (visited.find(newPos) == visited.end()) {
                    visited.insert(newPos);
                    q.push(newPos);
                    pattern.push_back(newPos);
                }
            }
        }
        moves++;
    }
    
    return pattern;
}

std::vector<std::pair<int, int>> getTeleportPositions(int row, int col, 
                                                     const std::vector<std::string>& grid, 
                                                     std::unordered_map<std::pair<int, int>, int, PairHash>& path,
                                                     int maxMoves) {
    std::vector<std::pair<int, int>> positions;
    auto pattern = generateTeleportPattern(maxMoves);
    
    for (const auto& [dr, dc] : pattern) {
        int newRow = row + dr;
        int newCol = col + dc;
        if (newRow >= 0 && newRow < grid.size() && 
            newCol >= 0 && newCol < grid[0].size() && 
            grid[newRow][newCol] != '#' && path.contains({newRow, newCol})) {
            positions.push_back({newRow, newCol});
        }
    }
    return positions;
}

int bfs(std::vector<std::string> grid, int rowStart, int colStart, int rowEnd, int colEnd, 
        std::unordered_map<std::pair<int, int>, int, PairHash>& nodes) {

    std::vector<std::pair<int, int>> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    std::queue<std::pair<int, int>> q;
    
    q.push({rowStart, colStart});

    int steps = 0;
    while(!q.empty()) {
        size_t qSize = q.size();
        for(int i = 0; i < qSize; i++) {
            auto [r, c] = q.front();
            q.pop();
            
            if(r == rowEnd && c == colEnd) {
                nodes[{r, c}] = steps;
                return steps;
            }
            
            for(auto [dr, dc] : dirs) {
                int rNext = r + dr;
                int cNext = c + dc;
                
                if(rNext >= 0 && rNext < grid.size() && cNext >= 0 && cNext < grid[0].size() && grid[rNext][cNext] != '#') {
                    q.push({rNext, cNext});
                    grid[rNext][cNext] = '#';
                    nodes[{rNext, cNext}] = steps + 1;
                }
            }
        }
        steps++;
    }
    return -1;
}

int main() {
    std::ifstream file("input.txt");
    std::string line;
    std::vector<std::string> grid;
    while(std::getline(file, line)) {
        grid.push_back(line);
    }

    int rowStart, colStart;
    int rowEnd, colEnd;

    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[i].size(); j++) {
            if(grid[i][j] == 'S') {
                rowStart = i;
                colStart = j;
            }
            if(grid[i][j] == 'E') {
                rowEnd = i;
                colEnd = j;
            }
        }
    }
    std::unordered_map<std::pair<int, int>, int, PairHash> path;
    int initiaTime = bfs(grid, rowStart, colStart, rowEnd, colEnd, path);

    std::vector<int> paths;
    for(auto& [node, dist] : path) {
        int r = node.first;
        int c = node.second;
        auto teleportPositions = getTeleportPositions(r, c, grid, path, 2);

        for(auto [rNext, cNext] : teleportPositions) {
            // Check if teleport destination is on original path and is closer to end
            if(path.contains({rNext, cNext}) && path[{rNext, cNext}] > path[{r, c}]) {
                // Original steps to teleport start + 1 for teleport + remaining steps from teleport end
                int newPathLength = path[{r, c}] + 1 + (initiaTime - path[{rNext, cNext}]);
                if(newPathLength <= initiaTime - 100) {
                    paths.push_back(newPathLength);
                }
            }
        }
    }

    std::cout << "Initial time: " << initiaTime << std::endl;
    std::cout << "Path length: " << path.size() << std::endl;
    std::cout << "Count: " << paths.size() << std::endl;

    std::set<std::pair<std::pair<int,int>, std::pair<int,int>>> uniqueCheats;

    for(auto& [node, dist] : path) {
        int r = node.first;
        int c = node.second;
        auto teleportPositions = getTeleportPositions(r, c, grid, path, 20);

        for(auto [rNext, cNext] : teleportPositions) {
            if(path.contains({rNext, cNext}) && path[{rNext, cNext}] > path[{r, c}]) {
                int manhattan = std::abs(rNext - r) + std::abs(cNext - c);

                int newPathLength = path[{r, c}] + manhattan + (initiaTime - path[{rNext, cNext}]);
                if(newPathLength <= initiaTime - 100) {
                    auto start = std::make_pair(r, c);
                    auto end = std::make_pair(rNext, cNext);
                    uniqueCheats.insert({start, end});
                    
                }
            }
        }
    }
    std::cout << "Cheats: " << uniqueCheats.size() << std::endl;
    return 0;
}


