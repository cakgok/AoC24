#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <chrono>

bool canConstruct(std::string& design, std::vector<std::string>& patterns, std::unordered_set<int>& cache, int idx) {
    if(design.size() == idx) return true;
    if(cache.contains(idx)) return false;

    for(auto& pattern: patterns) {
        if(design.substr(idx, pattern.size()) == pattern) {
            if(canConstruct(design, patterns, cache, idx+pattern.size())) return true;
            else cache.insert(idx+pattern.size());
        }
    }
    
    return false; 
}

uint64_t countWays(std::string& design, std::vector<std::string>& patterns, std::unordered_map<int, uint64_t>& cache, int idx) {
    if(design.size() == idx) return 1;
    if(cache.contains(idx)) return cache[idx];

    uint64_t ways = 0;
    for(auto& pattern: patterns) {
        if(design.substr(idx, pattern.size()) == pattern) {
            ways += countWays(design, patterns, cache, idx+pattern.size());
        }
    }
    
    return cache[idx] = ways; 
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");

    std::string patterns;
    std::getline(file, patterns);
    std::istringstream iss(patterns);
    std::string pattern;
    std::vector<std::string> patternVec;

    std::getline(file, patterns); //skip empty line

    while(std::getline(iss, pattern, ' ')) {
        if(pattern.back() == ',') pattern.erase(pattern.size()-1,1);
        patternVec.push_back(pattern);
    } 

    auto part1 = std::chrono::high_resolution_clock::now();

    std::string design;
    std::vector<std::string> designs;
    while(std::getline(file, design)) {
        designs.push_back(design);
    }

    std::unordered_set<int> cache;
    int possible = 0;
    for(auto& design: designs) {
        cache.clear();
        possible += canConstruct(design, patternVec, cache, 0);
    }

    auto part2 = std::chrono::high_resolution_clock::now();

    uint64_t ways = 0;
    std::unordered_map<int, uint64_t> cache_p2;
    for(auto& design: designs) {
        cache_p2.clear();
        ways += countWays(design, patternVec, cache_p2, 0);
    }

    std::cout << "Parse Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(part1 - start).count() << "ms" << std::endl;
    std::cout << "Part 1 Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(part2 - part1).count() << "ms" << std::endl;
    std::cout << "Part 2 Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - part2).count() << "ms" << std::endl;

    std::cout << possible << std::endl;
    std::cout << ways << std::endl;

    return 0;
}