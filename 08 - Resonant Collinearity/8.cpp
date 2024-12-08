#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <algorithm>
#include <numeric>


struct PairHash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

std::unordered_map<char, std::vector<std::pair<int, int>>> parse(const std::vector<std::string>& grid) {
    std::unordered_map<char, std::vector<std::pair<int, int>>> res;
    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[i].size(); j++) {
            if(grid[i][j] != '.' && grid[i][j] != '#') {
                res[grid[i][j]].push_back({j, i});  // j (col) is x, i (row) is y
            }
        }
    }
    return res;
}

// input is structered susch that gcd of two antennas is always 1
// so, it is enough to mirror the points, we don't need to check the points between
int findLocations(const std::unordered_map<char, std::vector<std::pair<int, int>>>& map, int rows, int cols) {
    std::unordered_set<std::pair<int, int>, PairHash> visited; // need a pair hash
    for(auto& pair: map) {
        auto& coordinates = pair.second;
        for(int i = 0; i < coordinates.size(); i++) {
            for(int j = i+1; j < coordinates.size(); j++) {
                int x1 = 2*coordinates[i].first - coordinates[j].first;
                int y1 = 2*coordinates[i].second - coordinates[j].second;  
                std::pair<int, int> p1 = {x1, y1};
                if(x1 >= 0 && x1 < cols && y1 >= 0 && y1 < rows && !visited.count(p1)) {
                    visited.insert({x1, y1});
                }
            
                int x2 = 2*coordinates[j].first - coordinates[i].first;
                int y2 = 2*coordinates[j].second - coordinates[i].second;              
                std::pair<int, int> p2 = {x2, y2};
                if(x2 >= 0 && x2 < cols && y2 >= 0 && y2 < rows && !visited.count(p2)) {
                    visited.insert({x2, y2});
                }
            }
        
        }
    }    
    
    return visited.size();
}

void findPointsOnLine(const std::pair<int, int>& p1, const std::pair<int, int>& p2, std::unordered_set<std::pair<int, int>, PairHash>& visited, int rows, int cols) {
    int dx = p2.first - p1.first;
    int dy = p2.second - p1.second;
    int gcd = std::gcd(std::abs(dx), std::abs(dy));
    dx /= gcd;
    dy /= gcd;

    visited.insert({p1.first, p1.second});

    int x = p1.first + dx;
    int y = p1.second + dy;
    while(x >= 0 && x < cols && y >= 0 && y < rows) {
        visited.insert({x, y});
        x += dx;
        y += dy;
    }

    x = p1.first - dx;
    y = p1.second - dy;
    while(x >= 0 && x < cols && y >= 0 && y < rows) {
        visited.insert({x, y});
        x -= dx;
        y -= dy;
    }
}

int findLocationsWithResonance(const std::unordered_map<char, std::vector<std::pair<int, int>>>& map, int rows, int cols) {
    std::unordered_set<std::pair<int, int>, PairHash> visited; // need a pair hash
    for(auto& pair: map) {
        auto& coordinates = pair.second;
        for(int i = 0; i < coordinates.size(); i++) {
            for(int j = i+1; j < coordinates.size(); j++) {
                // coordinates[i] and coordinates[j] are two antennas
                findPointsOnLine(coordinates[i], coordinates[j], visited, rows, cols);
            }
        }
    }    
    
    return visited.size();
}

int main() {    
    std::ifstream file("input.txt");
    std::string line;
    auto startTime = std::chrono::high_resolution_clock::now();
    std::vector<std::string> grid;
    
    while(std::getline(file, line)) {
        grid.push_back(line);
    }
    std::unordered_map<char, std::vector<std::pair<int,int>>> map = parse(grid);
    auto parseEnd = std::chrono::high_resolution_clock::now();

    int r = grid.size();
    int c = grid[0].size();
    int count_part1 = findLocations(map, r, c);
    auto part1_end_time = std::chrono::high_resolution_clock::now();

    int count_part2 = findLocationsWithResonance(map, r, c);
    auto part2_end_time = std::chrono::high_resolution_clock::now();


    std::cout << "Execution Time: " << std::chrono::duration_cast<std::chrono::microseconds>(parseEnd - startTime).count() << "uS" << std::endl;
    std::cout << "Execution Time: " << std::chrono::duration_cast<std::chrono::microseconds>(part1_end_time - parseEnd).count() << "uS" << std::endl;
    std::cout << "Execution Time: " << std::chrono::duration_cast<std::chrono::microseconds>(part2_end_time - part1_end_time).count() << "uS" << std::endl;
    std::cout << count_part1 << std::endl;
    std::cout << count_part2 << std::endl;

    return 0;
}
