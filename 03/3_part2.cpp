#include <fstream>
#include <string>
#include <string_view>
#include <iostream>

int mul(std::string_view& sv) {
    int x = 0, y = 0, idx = 0;
    if(!std::isdigit(sv[idx])) return 0;
    // Parse first number
    while (idx < sv.size() && std::isdigit(sv[idx])) x = x * 10 + (sv[idx++] - '0');

    // Check for comma 
    if (idx >= sv.size() || sv[idx++] != ',') return 0;

    // Parse second number
    while (idx < sv.size() && std::isdigit(sv[idx])) 
        y = y * 10 + (sv[idx++] - '0');

    // Checl for closing parenthesis
    if (idx >= sv.size() || sv[idx] != ')') return 0;

    sv.remove_prefix(idx + 1);
    return x * y;
}

int main( ) {
    std::ifstream file;
    file.open("input.txt");
    std::string line;
    long long sum = 0;
    bool mulEnabled = true;
    while (std::getline(file, line)) {
        std::string_view sv(line);
        while (!sv.empty()) {
            size_t pos;

            if(!mulEnabled) {
                pos = sv.find("do()");

                if (pos == std::string_view::npos) break;

                mulEnabled = true;
                sv.remove_prefix(pos + 4);  // skip past "do()"
                continue;
            }
        
            size_t mulPos = sv.find("mul(");
            size_t dontPos = sv.find("don't()");

            // we find don't() before mul or mul is not found
            if (dontPos != std::string_view::npos && (mulPos == std::string_view::npos || dontPos < mulPos)) {
                mulEnabled = false;
                sv.remove_prefix(dontPos + 7);
                continue;
            }

            if (mulPos == std::string_view::npos) break;

            pos = mulPos;
            sv.remove_prefix(pos+4);
            sum += mul(sv);
            
        }
    }
    std::cout << sum;
}

//97529391
//168539636