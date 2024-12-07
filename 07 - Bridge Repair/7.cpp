#include <fstream>
#include <string_view>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
std::pair<long long, std::vector<int>> parseLine(const std::string& line) {
    std::pair<long long, std::vector<int>> res;
    std::stringstream ss(line);
    std::string token;
    
    std::getline(ss, token, ':');
    res.first = std::stoll(token);
    
    while(ss >> token) {
        res.second.push_back(std::stoi(token));
    }
    return res;
}


bool isFixable(long long target, const std::vector<int>& nums, long long curr, int idx) {
    // try dfs with backtracking to find a solution 

    if(curr == target && idx == nums.size()) return true;
    if(curr > target) return false;
    if(idx >= nums.size()) return false;

    bool res = false;
    res |= isFixable(target, nums, curr + nums[idx], idx + 1);
    res |= isFixable(target, nums, curr * nums[idx], idx + 1);

    return res;
}

long long conc(long long x, int y) {
    return std::stoll(std::to_string(x) + std::to_string(y));
}

bool isFixableWithConc(long long target, const std::vector<int>& nums, long long curr, int idx) {
    if(curr == target && idx == nums.size()) return true;
    if(curr > target) return false;
    if(idx >= nums.size()) return false;

    bool res = false;
    res |= isFixableWithConc(target, nums, curr + nums[idx], idx + 1);
    res |= isFixableWithConc(target, nums, curr * nums[idx], idx + 1);
    res |= isFixableWithConc(target, nums, conc(curr, nums[idx]), idx + 1);

    return res;
}

int main() {
    std::ifstream file("input.txt");
    std::string line;
    auto startTime = std::chrono::high_resolution_clock::now();

    long long part1 = 0;
    long long part2 = 0;
    
    while(std::getline(file, line)) {
        auto [target, nums] = parseLine(line);
        if(isFixable(target, nums, nums[0], 1)) part1 += target;
        if(isFixableWithConc(target, nums, nums[0], 1)) part2 += target;
    }

    auto parseEnd = std::chrono::high_resolution_clock::now();
    std::cout << "Execution Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(parseEnd - startTime).count() << "ms" << std::endl;
    std::cout << part1 << std::endl;
    std::cout << part2 << std::endl;
}

//8401132154762
//95297119227552