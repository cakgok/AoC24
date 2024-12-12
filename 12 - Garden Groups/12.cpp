#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

std::pair<int, int> dfs(std::vector<std::string>& garden, int r, int c, std::vector<std::vector<bool>>& visit, char plant) {
    if(r < 0 || r >= garden.size() || c < 0 || c >= garden[r].size() || visit[r][c] || garden[r][c] != plant) return {0, 0};

    visit[r][c] = true;

    int area = 1;
    int perimeter = 0;

    if(r+1 >= garden.size() || garden[r+1][c] != plant) perimeter++;
    if(r-1 < 0 || garden[r-1][c] != plant) perimeter++;
    if(c+1 >= garden[r].size() || garden[r][c+1] != plant) perimeter++;
    if(c-1 < 0 || garden[r][c-1] != plant) perimeter++;

    auto [area1, perimeter1] = dfs(garden, r + 1, c, visit, plant);
    auto [area2, perimeter2] = dfs(garden, r - 1, c, visit, plant);
    auto [area3, perimeter3] = dfs(garden, r, c + 1, visit, plant);
    auto [area4, perimeter4] = dfs(garden, r, c - 1, visit, plant);

    area += area1 + area2 + area3 + area4;
    perimeter += perimeter1 + perimeter2 + perimeter3 + perimeter4;

    return {area, perimeter};
}

bool isPlant(char c) {
    return c == 'T' || c == 'C' || c == 'G' || c == 'A';
}

std::pair<int, int> dfs_2(std::vector<std::string>& garden, int r, int c, std::vector<std::vector<bool>>& visit, char plant) {
    auto isPlant = [&](int row, int col) -> bool { 
        if(row < 0 || row >= garden.size() || col < 0 || col >= garden[row].size()) return false;
        return garden[row][col] == plant;
    };

    if(!isPlant(r, c) || visit[r][c]) return {0, 0};

    visit[r][c] = true;

    int area = 1;
    int corners = 0;

    // Lemma is: Coreners == Sides
    // A corner exists when EITHER:
    // 1. Both adjacent cells are NOT plant OR
    // 2. Both adjacent cells ARE plant AND diagonal is NOT plant

    if((!isPlant(r-1, c) && !isPlant(r, c-1)) || 
       (isPlant(r-1, c) && isPlant(r, c-1) && !isPlant(r-1, c-1))) corners++; // top-left
       
    if((!isPlant(r-1, c) && !isPlant(r, c+1)) || 
       (isPlant(r-1, c) && isPlant(r, c+1) && !isPlant(r-1, c+1))) corners++; // top-right
       
    if((!isPlant(r+1, c) && !isPlant(r, c-1)) || 
       (isPlant(r+1, c) && isPlant(r, c-1) && !isPlant(r+1, c-1))) corners++; // bottom-left
       
    if((!isPlant(r+1, c) && !isPlant(r, c+1)) || 
       (isPlant(r+1, c) && isPlant(r, c+1) && !isPlant(r+1, c+1))) corners++; // bottom-right

    auto [area1, corners1] = dfs_2(garden, r + 1, c, visit, plant);
    auto [area2, corners2] = dfs_2(garden, r - 1, c, visit, plant);
    auto [area3, corners3] = dfs_2(garden, r, c + 1, visit, plant);
    auto [area4, corners4] = dfs_2(garden, r, c - 1, visit, plant);

    area += area1 + area2 + area3 + area4;
    corners += corners1 + corners2 + corners3 + corners4;

    return {area, corners};
}

int main() {
    auto parseStart = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");
    std::string line;
    std::vector<std::string> garden;
    while(std::getline(file, line)) {
        garden.push_back(line);
    }

    auto partOneStart = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<bool>> visit(garden.size(), std::vector<bool>(garden[0].size(), false));
    int price = 0;
    for(int row = 0; row < garden.size(); row++) {
        for(int col = 0; col < garden[row].size(); col++) {
            if(visit[row][col]) continue;
            char plant = garden[row][col];
            auto [area, perimeter] = dfs(garden, row, col, visit, plant);
            price += area * perimeter;
        }
    }

    auto partTwoStart = std::chrono::high_resolution_clock::now();

    int price_2 = 0;
    std::vector<std::vector<bool>> visited(garden.size(), std::vector<bool>(garden[0].size(), false));

    for(int row = 0; row < garden.size(); row++) {
        for(int col = 0; col < garden[row].size(); col++) {
            if(visited[row][col]) continue;
            char plant = garden[row][col];
            auto [area, corners] = dfs_2(garden, row, col, visited, plant);
            price_2 += area * corners;
        }
    }

    auto partTwoEnd = std::chrono::high_resolution_clock::now();
    std::cout << "Part 1: " << price << std::endl;
    std::cout << "Part 2: " << price_2 << std::endl;

    std::cout << "Parse Time: " << std::chrono::duration_cast<std::chrono::microseconds>(partOneStart - parseStart).count() << "uS" << std::endl;
    std::cout << "Part One Execution Time: " << std::chrono::duration_cast<std::chrono::microseconds>(partTwoStart - partOneStart).count() << "uS" << std::endl;
    std::cout << "Part Two Execution Time: " << std::chrono::duration_cast<std::chrono::microseconds>(partTwoEnd - partTwoStart).count() << "uS" << std::endl;
    std::cout << "Total Execution Time: " << std::chrono::duration_cast<std::chrono::microseconds>(partTwoEnd - parseStart).count() << "uS" << std::endl;


    return 0;
}
