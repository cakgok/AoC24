#include <fstream>
#include <iostream>
#include <numeric>
#include <chrono>
using ll = long long;

// ax+by=c
ll solve(ll a1, ll b1, ll c1, ll a2, ll b2, ll c2) {  
    ll det = a1 * b2 - a2 * b1;
    
    if(det == 0) return 0;
    
    if ((c1 * b2 - c2 * b1) % det != 0 || 
        (a1 * c2 - a2 * c1) % det != 0) return 0;

    ll x = (c1 * b2 - c2 * b1) / det;
    ll y = (a1 * c2 - a2 * c1) / det;

    if (x < 0 || y < 0) return 0;
    return 3*x + y;
}

int main() {

    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");
    std::string line;
    
    ll sum = 0;
    ll sum2 = 0;
    while(std::getline(file, line)) {
        int buttonA_x, buttonA_y;
        int buttonB_x, buttonB_y;
        int prize_x, prize_y;
        sscanf(line.c_str(), "Button A: X+%d, Y+%d", &buttonA_x, &buttonA_y);
        
        std::getline(file, line); 
        sscanf(line.c_str(), "Button B: X+%d, Y+%d", &buttonB_x, &buttonB_y);
        
        std::getline(file, line);
        sscanf(line.c_str(), "Prize: X=%d, Y=%d", &prize_x, &prize_y);
        std::getline(file, line); // empty line
        
        sum += solve(buttonA_x, buttonB_x, prize_x, buttonA_y, buttonB_y, prize_y);
        sum2 += solve(buttonA_x, buttonB_x, prize_x+10000000000000, buttonA_y, buttonB_y, prize_y+10000000000000);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Execution Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " uS" << std::endl;
    std::cout << "Part 1: " << sum << std::endl;
    std::cout << "Part 2: " << sum2 << std::endl;
    return 0;
}