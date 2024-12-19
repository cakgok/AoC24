#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#define rows 71
#define cols 71

int bfs(std::vector<std::vector<char>>& grid, std::vector<std::vector<char>>& visit, int r_start, int c_start, int r_end, int c_end) {
    std::queue<std::pair<int, int>> queue;
    queue.push({r_start, c_start});
    int steps = 0;
    while(!queue.empty()) {
        size_t qsize = queue.size();
        for(int i = 0; i < qsize; i++) {
            auto [r, c] = queue.front();
            queue.pop();
            if(r < 0 || r >= rows || c < 0 || c >= cols ) continue;
            if(visit[r][c] == '#') continue;
            if(r == rows-1 && c == cols-1) return steps;

            visit[r][c] = '#';
            
            queue.push({r+1, c});
            queue.push({r-1, c});
            queue.push({r, c+1});
            queue.push({r, c-1});
        }
        steps++;

    }
    return -1;
}


int main () {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");
    std::string line;
    std::vector<std::vector<char>> grid(rows, std::vector<char>(cols,'.'));
    
    for(int i = 0; i < 3000; i++) {
        std::getline(file, line);
        int r, c;
        std::sscanf(line.c_str(), "%d,%d", &r, &c);
        grid[r][c] = '#';
    }
    auto part1 = std::chrono::high_resolution_clock::now();

    // part 1
    std::vector<std::vector<char>> visit = grid;
    std::queue<std::pair<int, int>> queue;
    int steps = bfs(grid, visit, 0, 0, rows-1, cols-1);
    std::cout<< steps <<std::endl;

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            grid[i][j] = '.';
        }
    }


    // part 2
    auto part2 = std::chrono::high_resolution_clock::now();


    int left = 0;
    int right = 4000;  
    std::pair<int, int> result = {0, 0};
    std::vector<std::vector<char>> clean_grid(rows, std::vector<char>(cols, '.'));

    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        grid = clean_grid; 
        int r, c;    
        // Read first 'mid' lines
        file.clear();
        file.seekg(0);
        for(int i = 0; i < mid; i++) {
            std::getline(file, line);
            std::sscanf(line.c_str(), "%d,%d", &r, &c);
            grid[r][c] = '#';
        }
        
        visit = grid;
        steps = bfs(grid, visit, 0, 0, rows-1, cols-1);
        
        if (steps == -1) {
            right = mid - 1;
            result = {r, c};
        } else {
            left = mid + 1;
        }
    }
    std::cout << "Parse time: " << std::chrono::duration_cast<std::chrono::microseconds>(part1 - start).count() << "uS\n";
    std::cout << "Part 1 time: " << std::chrono::duration_cast<std::chrono::microseconds>(part2 - part1).count() << "uS\n";
    std::cout << "Part 2 time: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - part2).count() << "uS\n";
    std::cout << "Blocking occurs at line: " << result.first << "," << result.second << std::endl;

    return 0;
}