#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <vector>


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


    std::string design;
    int possible = 0;
    while(std::getline(file, design)) {
        std::unordered_set<int> cache; //possible designs
        possible += canConstruct(design, patternVec, cache, 0);
    }
    std::cout << possible << std::endl;

    file.clear();
    file.seekg(0);
    std::getline(file, patterns);
    std::getline(file, patterns);

    uint64_t ways = 0;
    while(std::getline(file, design)) {
        std::unordered_map<int, uint64_t> cache; //possible designs
        ways += countWays(design, patternVec, cache, 0);
    }

    std::cout << ways << std::endl;

    return 0;
}