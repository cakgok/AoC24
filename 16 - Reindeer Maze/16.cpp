#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <algorithm>
#include <chrono>
#include <set>

struct State {
    int row, col;     // position
    int direction;    // 0: right, 1: down, 2: left, 3: up
    int cost;         // total cost to reach this state
    
    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

bool dfs(std::vector<std::string>& maze, 
         std::vector<std::vector<std::vector<int>>>& visit,
         std::set<std::pair<int, int>>& nodes, 
         State curr, 
         int& score, 
         const std::vector<std::pair<int, int>>& directions) {
    
    // we just backtrack to the start, collecting all nodes in the path
    if(maze[curr.row][curr.col] == 'E' && curr.cost == score) {
        nodes.insert({curr.row, curr.col});
        return true;
    }

    if(visit[curr.row][curr.col][curr.direction] != curr.cost) {
        return false;
    }

    bool foundPath = false;
    for(int dir = 0; dir < 4; dir++) {
        int new_row = curr.row + directions[dir].first;
        int new_col = curr.col + directions[dir].second;
        
        if(maze[new_row][new_col] == '#') continue;
        int move_cost = (dir == curr.direction) ? 1 : 1001;
        
        State next = {
            new_row,
            new_col,
            dir,
            curr.cost + move_cost
        };

        if(dfs(maze, visit, nodes, next, score, directions)) {
            nodes.insert({curr.row, curr.col});
            foundPath = true;
        }
    }
    return foundPath;
}

int main () {
    auto start = std::chrono::high_resolution_clock::now();        
    std::ifstream file("input.txt");
    std::string line;
    std::vector<std::string> maze;
    while (std::getline(file, line)) {
        maze.push_back(line);
    }
    auto part1 = std::chrono::high_resolution_clock::now();

    const size_t rows = maze.size();
    const size_t cols = maze[0].size();

    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    
    std::vector<std::vector<std::vector<int>>> visit(rows, std::vector<std::vector<int>>(cols, std::vector<int>(4, -1)));
    const std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    int xStart, yStart, xEnd, yEnd;
    for(int i = 0; i < maze.size(); i++) {
        for(int j = 0; j < maze[i].size(); j++) {
            if(maze[i][j] == 'S') {
                xStart = i;
                yStart = j;
            }
            if(maze[i][j] == 'E') {
                xEnd = i;
                yEnd = j;
            }
        }
    }

    pq.push({xStart, yStart, 0, 0}); // Start facing right

    int score = INT_MAX;
    
    // dijkstra's where edge cost is 1 if moving in same direction, 1001 if changing direction
    while(!pq.empty()) {
        State curr = pq.top();
        pq.pop();

        if(visit[curr.row][curr.col][curr.direction] != -1) continue;
        visit[curr.row][curr.col][curr.direction] = curr.cost;

        if(maze[curr.row][curr.col] == 'E') {
            score = std::min(score, curr.cost);
            continue;;
        }

        for(int dir = 0; dir <  4; dir++) {
            int new_row = curr.row + directions[dir].first;
            int new_col = curr.col + directions[dir].second;
            
            if(maze[new_row][new_col] == '#') continue; //no need for bounds check, grid is walled
            int move_cost = (dir == curr.direction) ? 1 : 1001;

            State next = {
                new_row,
                new_col,
                dir,
                curr.cost + move_cost
            };
        
            if(visit[new_row][new_col][dir] == -1) {
                pq.push(next);
            }
        }
    }

    std::set<std::pair<int, int>> nodes;

    auto p2 = std::chrono::high_resolution_clock::now();
    dfs(maze, visit, nodes, {xStart, yStart, 0, 0}, score, directions);

    std::cout << "Parse Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(part1 - start).count() << "ms" << std::endl;
    std::cout << "Part 1 Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(p2 - part1).count() << "ms" << std::endl;
    std::cout << "Part 2 Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - p2).count() << "ms" << std::endl;

    std::cout << "Part 1 Result: " << score << std::endl;
    std::cout << "Part 2 Result: " << nodes.size() << std::endl;

    return 0;
}