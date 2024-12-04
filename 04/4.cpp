#include <fstream>
#include <string>
#include <vector>
#include <iostream>

int findWord(std::vector<std::string>& grid, int i, int j, int idx, std::string& searchWord, int dX, int dY) {
    int count = 0;
    if( i < 0 || j < 0 || i >= grid.size() || j >= grid[i].size()) return 0;
    if(grid[i][j] != searchWord[idx]) return 0;
    if(idx == 3 && grid[i][j] == searchWord[idx]) return 1;

    idx++;
    return findWord(grid, i + dX, j + dY, idx, searchWord, dX, dY);
}

int findCross(std::vector<std::string>& grid, int i, int j) {
    if(i < 1 || j < 1 || i >= grid.size() - 1 || j >= grid[i].size() - 1) return 0; //at the edge

    bool pattern1 = (grid[i-1][j-1] == 'S' && grid[i+1][j+1] == 'M' ) || (grid[i-1][j-1] == 'M' && grid[i+1][j+1] == 'S');
    bool pattern2 = (grid[i-1][j+1] == 'S' && grid[i+1][j-1] == 'M') || (grid[i-1][j+1] == 'M' && grid[i+1][j-1] == 'S');

    return pattern1 && pattern2 ? 1 : 0;
}

int main() { 
    std::ifstream file;
    file.open("input.txt");
    std::vector<std::string> grid;
    std::string line;
    while (std::getline(file, line)) {
        grid.push_back(line);
    }
    file.close();

    int countXMAS = 0;
    int countCross = 0;

    const int dX[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dY[] = {-1, 0, 1, -1, 1, -1, 0, 1};


    std::string searchWord = "XMAS";
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if(grid[i][j] == 'A') countCross += findCross(grid, i, j);
            for(int k = 0; k < 8; k++) {
                countXMAS += findWord(grid, i, j, 0, searchWord, dX[k], dY[k]);
            }
        }
    }
    std::cout << "Part 1: " << countXMAS <<std::endl;
    std::cout << "Part 2: " << countCross <<std::endl;
    return 0;
}