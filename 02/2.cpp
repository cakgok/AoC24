#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <cassert>

std::vector<int> parse_line(const std::string& line) {
    std::vector<int> numbers;
    numbers.reserve(16);
    std::istringstream stream(line);
    int value;
    while (stream >> value) {
        numbers.push_back(value);
    }
    return numbers;
}


bool process_line(std::vector<int>& sequence) {
    
    bool increasing = (sequence[0] < sequence[1]);
    if (sequence[0] == sequence[1]) return false;
    
    if (increasing && sequence[1] - sequence[0] > 3) return false;
    if (!increasing && sequence[0] - sequence[1] > 3) return false;


    for (int i = 1; i < sequence.size() - 1; ++i) {
        if (increasing) {
            if (sequence[i + 1] - sequence[i] > 3 || sequence[i + 1] - sequence[i] <= 0) {
                return false;
            }
        } else {
            if (sequence[i] - sequence[i + 1] > 3 || sequence[i] - sequence[i + 1] <= 0) {
                return false;
            }
        }
    }
    return true;
}

bool dampen(int i, const std::vector<int>& original_sequence) {
    std::vector<int> dampened_sequence = original_sequence;
    dampened_sequence.erase(dampened_sequence.begin() + i);
    return process_line(dampened_sequence);
}

int main() {

    std::ifstream file("input.txt");
    std::string line;
    int count = 0;
    int dampened_count = 0;
    while (std::getline(file, line)) {
        std::vector<int> sequence = parse_line(line);
        count += process_line(sequence);
        bool dampened = false;
        dampened |= dampen(0, sequence);
        for (int i = 0; i < sequence.size() - 1; ++i) {
            int diff = sequence[i + 1] - sequence[i];
            if(std::abs(diff) < 1 || std::abs(diff) > 3) {
                dampened = dampen(i ,sequence) || dampen(i+1, sequence);
                break;
            }
            if(i+2 < sequence.size()) {
                int diff2 = sequence[i+2] - sequence[i+1];
                if(diff > 0 != diff2 > 0) {
                    dampened = dampen(i, sequence) || dampen(i+1, sequence) || dampen(i+2, sequence);
                    break;
                }
            }
        }
        if(dampened) dampened_count++;
    }
 
    file.close();
    std::cout << count << std::endl;
    std::cout << dampened_count << std::endl;
    return 0;
}
