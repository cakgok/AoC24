#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <unordered_set>

#define MOD 16777216 // 2^24 == 0xFFFFFF

inline int packChanges(int a, int b, int c, int d) {
    const int a_shifted = a + 9; 
    const int b_shifted = b + 9;
    const int c_shifted = c + 9;
    const int d_shifted = d + 9;
    return a_shifted | (b_shifted << 5) | (c_shifted << 10) | (d_shifted << 15);
}

void find(std::vector<int>& seq, std::unordered_map<int ,int>& seqMap) {
    std::unordered_set<int> seen;
    std::vector<int> changes(seq.size(), 0);
        
    for(int i = 1; i < seq.size() - 3; i++) {
        int change1 = seq[i] - seq[i-1];
        int change2 = seq[i+1] - seq[i];
        int change3 = seq[i+2] - seq[i+1];
        int change4 = seq[i+3] - seq[i+2];
        
        int packed = packChanges(change1, change2, change3, change4);

        if(seen.contains(packed)) continue;
        seen.insert(packed);
        seqMap[packed] += seq[i+3];
    }
}

long long calc(long long s) {
    long long temp = s << 6;
    s = s ^ temp;
    s %= MOD;
    
    temp = s >> 5;
    s = s ^ temp;
    s %= MOD;
    
    temp = s << 11;
    s = s ^ temp;
    s %= MOD;
    
    return s;
}

int main() {
    std::ifstream file("input.txt");
    std::string n;
    std::vector<int> input;
    while(std::getline(file, n)) {
        input.push_back(std::stoi(n));
    }
    auto start = std::chrono::high_resolution_clock::now();

    long long result_p1 = 0, result_p2 = 0;
    int iterations = 2000;
    std::unordered_map<int, int> map;
    for(int i = 0; i < input.size(); i++) {
        long long s = input[i];
        std::vector<int> seq(2000);
        for(int j = 0; j < iterations; j++) {
            s = calc(s);
            seq[j] = s % 10;
        }
        find(seq, map);
        result_p1 += s;
    }

    int max = 0;
    for(auto& [seq, calc] : map) {
        max = std::max(max, calc);
    }

    std::cout << "Execution time: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << "ms" << std::endl;
    std::cout << "Part 1 Result " << result_p1 << std::endl;
    std::cout << "Part 2 Result " << max << std::endl;
}
