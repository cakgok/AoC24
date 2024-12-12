#include<fstream>
#include<iostream>
#include<vector>
#include<unordered_map>
#include<string>
#include<cstdint>
#include<chrono>

// Boost hash_combine
struct pair_hash {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        size_t seed = std::hash<T1>{}(p.first);
        seed ^= std::hash<T2>{}(p.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

int cache_hits = 0;
int total_blinks = 0;  
std::unordered_map<int, int> hits_by_blinks;  // blinks_left -> hit count
std::unordered_map<int, int> hits_by_digits;  // number_of_digits -> hit count

int64_t blink(int64_t n, std::unordered_map<std::pair<int64_t,int64_t>, int64_t, pair_hash>& cache, int64_t blinks) {

    total_blinks++;  

    if(blinks == 0) return 1;
    if(cache.count({n, blinks})) {
        cache_hits++;

        hits_by_blinks[blinks]++;
        hits_by_digits[std::to_string(n).length()]++;

        return cache[{n, blinks}];
    }
    int64_t result = 0;

    if(n == 0) result = blink(1, cache, blinks - 1);
    else if(std::to_string(n).size() % 2 == 0) {
        std::string ns = std::to_string(n);
        int64_t mid = ns.size() / 2;
        int64_t first = std::stoi(ns.substr(0, mid));
        int64_t second = std::stoi(ns.substr(mid));
        result += blink(first, cache, blinks - 1); 
        result += blink(second, cache, blinks - 1);
    } else {
        result = blink(n * 2024, cache, blinks - 1);
    }

    return cache[{n, blinks}] = result;
}


int main () {
    auto startTime = std::chrono::high_resolution_clock::now();
    std::unordered_map<std::pair<int64_t,int64_t>, int64_t, pair_hash> cache;

    std::vector<int> v;
    std::ifstream file("input.txt");
    int num;
    while (file >> num) {
        v.push_back(num);
    }

    int64_t result = 0;
    for(int64_t i = 0; i < v.size(); i++) {
        result += blink(v[i], cache, 75);
    }

    auto part1_end_time = std::chrono::high_resolution_clock::now();
    std::cout << "Execution Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(part1_end_time - startTime).count() << "ms" << std::endl;

    std::cout << "Result: " << result << std::endl;
    std::cout << "Cache Hits: " << cache_hits << std::endl;
    std::cout << "Total Blinks: " << total_blinks << std::endl;
    std::cout << "Cache Hit Ratio: " << (double)cache_hits / total_blinks << std::endl;

    std::cout << "\nCache hits by remaining blinks:\n";
    for(const auto& [blinks, hits] : hits_by_blinks) {
        std::cout << "Blinks " << blinks << ": " << hits << " hits\n";
    }

    std::cout << "\nCache hits by number of digits:\n";
    for(const auto& [digits, hits] : hits_by_digits) {
        std::cout << digits << " digits: " << hits << " hits\n";
    }


    return 0;
}

/**Rules
n == 0 > 1
n_size % 2 == 0 > N  N  //gets divided to 2 from the middle becomes, two odd size numbers
else > N * 2024 // 
**/

