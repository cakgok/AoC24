#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <cmath>
#include <unordered_map>
#include <queue>
#include <map>
// l, r pointers of the orioginal string
// leftBlocks, rightBlocks are the number of blocks, for checksum calculation
// l_ID, r_ID are the indexes of the blocks
// O(n) time, O(1) space complexity
long long compact(std::string& s) {
    int l = 0;
    int l_ID = 0;

    int r = s.size() - 1;
    int r_ID = r / 2;

    long long sum = 0;
    long long checksum = 0;

    int rem = 0, free = 0;
    while(l <= r) {
        int leftBlocks = (free > 0) ? free : s[l] - '0';
        int rightBlocks = (rem > 0) ? rem : s[r] - '0';

        if(l % 2 == 0) { // left pointer poitnts to a file block
            // we are at a file block, simply caclualte ID*index for each and add to checksum

            checksum += l_ID * (leftBlocks*leftBlocks + 2*leftBlocks*sum - leftBlocks) / 2;
            sum += leftBlocks;
            l++, 
            l_ID++;
        }

        else{ // left points to a free block
            if(r % 2 == 0) r--;
            if(leftBlocks == rightBlocks) {
                // means that free blocks are equyal to file blocks to move from right
                checksum += r_ID * (leftBlocks*leftBlocks + 2*leftBlocks*sum - leftBlocks) / 2;
                sum += leftBlocks;
                l++;
                r--;
                r_ID--;
                free = 0;
                rem = 0;
            }
            else if(leftBlocks < rightBlocks) {
                // we can only move part of the blocks
                // so don't move the right pointer, but update the remainin blocks to move..
                checksum += r_ID * (leftBlocks*leftBlocks + 2*leftBlocks*sum - leftBlocks) / 2;
                sum += leftBlocks;
                l++;
                rem = rightBlocks - leftBlocks;
                free = 0;
            }
            else {
                // we can move all the blocks, and still have free space at the left. (right < left)
                checksum += r_ID * (rightBlocks*rightBlocks + 2*rightBlocks*sum - rightBlocks) / 2;
                sum += rightBlocks;
                r--;
                r_ID--;
                free = leftBlocks - rightBlocks;
                rem = 0;
            }
        }

    }
    checksum += r_ID * (rem*rem + 2*rem*sum - rem) / 2;
    return checksum;
}

int main() {
    
    std::ifstream file("input.txt");
    std::string s; 
    std::getline(file, s);

    long long checksum = compact(s);
    std::cout << checksum << std::endl;

    //long long checksum2 = compactPart2(s);
    //std::cout << checksum2 << std::endl;
    return 0;
}

//6334655979668
