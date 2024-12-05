#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <chrono>
#include <algorithm>

void parseRules(std::string& line, std::unordered_map<int, std::unordered_set<int>>& rules) {
    size_t pos = line.find('|');
    int x = std::stoi(line.substr(0, pos));
    int y = std::stoi(line.substr(pos + 1));
    rules[x].insert(y);
}

void parseUpdatesAndCheck(std::string& line,  std::unordered_map<int, std::unordered_set<int>>& rules, int& ordered, int& unordered) {
    std::stringstream ss(line);
    std::string token;
    std::vector<int> pages;

    bool marked_unordered = false;

    while (std::getline(ss, token, ',')) {
        int page = std::stoi(token);
        pages.push_back(page);
        if(marked_unordered) continue;
        for (int prev_page : pages) {
            if (rules[page].count(prev_page)) {
                marked_unordered = true;
            }
        }
    }

    if(marked_unordered) {
        std::sort(pages.begin(), pages.end(), [&rules](int x, int y) {
            return rules[x].count(y) > 0;
        });

        unordered += pages[pages.size() / 2];
    } else {
        ordered += pages[pages.size() / 2];
    }
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::ifstream file;
    file.open("input.txt");
    std::unordered_map<int, std::unordered_set<int>> rules;
    std::string line;

    bool parsingRules = true;

    int ordered = 0;
    int unordered = 0;

    while(std::getline(file, line)) {
        if(line.empty()) {
            parsingRules = false;
            continue;
        }

        if(parsingRules) parseRules(line, rules);
        else parseUpdatesAndCheck(line, rules, ordered, unordered);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;
    std::cout << "Result: " << ordered << std::endl;
    std::cout << "Result: " << unordered << std::endl;

    return 0;
}