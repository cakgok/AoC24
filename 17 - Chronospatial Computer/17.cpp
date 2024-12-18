#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <functional>
using ll = long long;

ll getComboOperand(int operand, ll& A, ll& B, ll& C) {
    if(operand >= 0 && operand < 4) return operand;
    if(operand == 4) return A;
    if(operand == 5) return B;
    if(operand == 6) return C;
    return -1;
}

void run(int& iPtr, ll& A, ll& B, ll& C, std::vector<int>& instructions, std::vector<int>& out) {
    while(iPtr < instructions.size()) {
        ll opcode = instructions[iPtr];
        ll operand = instructions[iPtr + 1];

        ll comb = getComboOperand(operand, A, B, C);
        
        switch (opcode)
        {
            case 0:
                A /= (1 << comb);
                break;
            case 1:
                B ^= operand;
                break;
            case 2:
                B = comb % 8;
                break;
            case 3:
                if(A != 0) {
                    iPtr = operand;
                    continue;
                }
                break;
            case 4:
                B ^= C;
                break;
            case 5:
                out.push_back(comb % 8);
                break;
            case 6:
                B = A/(1 << comb);
                break;
            case 7:
                C = A/(1 << comb);
                break;
            default:
                break;
        }
        iPtr+=2;
    }
}

int calc(ll& a) {
    ll b = a % 8;
    b = b ^ 3;
    ll c = a/(1 << b);
    a = a/(1 << 3);
    b = b ^ c;
    b = b ^ 5;
    return b % 8;
}

bool findInitialValue(ll curr, std::vector<int>& instructions, int& idx) {
    ll temp = curr;  // copy because calc modifies its input
    int output = calc(temp);

    if(output != instructions[idx]) {
        return false;
    }
    
    idx--;
    
    if(idx == -1) {
        std::cout << "Found solution: " << temp << std::endl;
        return true;
    }
    
    for(int r = 0; r < 8; r++) {
        long long nextValue = curr * 8 + r;
        if(findInitialValue(nextValue, instructions, idx)) {
            return true;
        }
    }
    
    idx++;
    return false;
}

int main() {
    long long regA, regB, regC;
    std::vector<int> instructions;
    int instructionPointer = 0;
    std::vector<int> output;

    regA = 45483412;
    regB = 0;
    regC = 0;
    instructions = {2,4,1,3,7,5,0,3,4,1,1,5,5,5,3,0};
    int iPtr = 0;
    run(iPtr, regA, regB, regC, instructions, output);
    for(int i = 0; i < output.size(); i++) {
        std::cout << output[i] << ",";
    }
    std::cout << std::endl;

    ll a = 45483412;

    // 6 give us 0 so we start there 
    // next answer (3,0) will be in the range of 6*8 + 1...8 
    // try for all possible values recuresively
    // backtrack if needed

    int targetPtr = instructions.size() - 1;
    findInitialValue(6, instructions, targetPtr);

    // part2
    // instructions = {2,4,1,3,7,5,0,3,4,1,1,5,5,5,3,0};
    // 2,4 -> B = A % 8
    // 1,3 -> B = B XOR 3
    // 7,5 -> C = A/(1 << B)
    // 0,3 -> A = A/(1 << 3) 
    // 4,1 -> B = B XOR C
    // 1,5 -> B = B XOR 5
    // 5,5 -> Output B % 8
    // 3,0 -> IF A == 0 THEN RETURN  (we repeat until A == 0 then exit)

    return 0;
}
