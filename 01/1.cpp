#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>

void part1(std::vector<int>& col1, std::vector<int>& col2) {
    long long diff = 0;
    for(int i = 0; i < col1.size(); i++) {
        diff += std::abs(col2[i] - col1[i]);
    }
    std::cout << diff  << std::endl;    
}

void part2(std::vector<int>& col1, std::vector<int>& col2) {
    long long sim = 0;
    std::unordered_map<int, int> freq_left;
    std::unordered_map<int, int> freq_right;
    for(int i = 0; i < col1.size(); i++) {
        freq_left[col1[i]]++;
    }
    for(int i = 0; i < col2.size(); i++) {
        freq_right[col2[i]]++;
    }
    for(auto& pair: freq_left) {
        sim += pair.first * pair.second * freq_right[pair.first];
    }
    std::cout << sim << std::endl;
}

int main() {
    std::ifstream file("input.txt");
    int num1, num2;
    std::vector<int> col1, col2;

    while (file >> num1 >> num2) {
        col1.push_back(num1);
        col2.push_back(num2);
    }

    file.close();
    std::sort(col1.begin(), col1.end());
    std::sort(col2.begin(), col2.end());
    //part1(col1, col2);
    part2(col1, col2);
    return 0;
}

