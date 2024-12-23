#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

void findMaximalClique(const std::unordered_map<std::string, std::unordered_set<std::string>>& adj,
                        std::unordered_set<std::string>& R,     // Current clique being built
                        std::unordered_set<std::string>& P,     // Potential vertices that can extend the clique
                        std::unordered_set<std::string>& X,     // Excluded - vertices already checked
                        std::vector<std::string>& max_clique) {

    // If P and X are empty, then R is a maximal clique
    // Then we check if it is the largest clique found so far
    // X tells us if the clique could have been extended earlier, so it has to be empty for R to be a maximal clique
    if (P.empty() && X.empty()) {
        if (R.size() > max_clique.size()) {
            max_clique.clear();
            max_clique.insert(max_clique.end(), R.begin(), R.end());
        }
        return;
    }

    // Choose a pivot vertex with the most connections
    // Reduces the recursive calls
    // The key is that, any maximal clique must contain either:
    // a) The pivot vertex, OR
    // b) At least one vertex NOT connected to the pivot
    // If a clique doesn't contain the pivot AND doesn't contain any non-neighbors of the pivot, it could be made larger by adding the pivot
    // (which would be connected to everything in the clique)

    std::string pivot;
    size_t max_connections = 0;
    for (const auto& v : P) {
        if (adj.at(v).size() > max_connections) {
            max_connections = adj.at(v).size();
            pivot = v;
        }
    }

    // make a copy of P since we will be modifying it
    std::unordered_set<std::string> P_copy = P;

    for (const auto& v : P_copy) {
        // Only consider vertices that are NOY connected to the pivot
        // Connected ones will be considered implicitly in the recursive calls since they will stay in P.
        if (adj.at(pivot).find(v) == adj.at(pivot).end()) {

            // Add vertex to current clique
            R.insert(v);
            
            // Create new candidate and excluded sets
            std::unordered_set<std::string> new_P, new_X;

            // Fill new_P with neighbors of v that are in P
            for (const auto& n : P) {
                if (adj.at(v).find(n) != adj.at(v).end()) {
                    new_P.insert(n);
                }
            }

            // Fill new_X with neighbors of v that are in X
            for (const auto& n : X) {
                if (adj.at(v).find(n) != adj.at(v).end()) {
                    new_X.insert(n);
                }
            }
            
            // Recursive call with the new candidate and excluded sets
            // Since new_P and new_X are subsets of P and X, the recursive call will eventually reach the base case
            findMaximalClique(adj, R, new_P, new_X, max_clique);
            
            // Move v from P to X
            R.erase(v);
            P.erase(v);
            X.insert(v);
        }
    }
}

int main() {
    std::ifstream file("input.txt");
    std::string line;
    std::unordered_map<std::string, std::unordered_set<std::string>> adj;
    while(std::getline(file, line)) {
        std::string node1 = line.substr(0, 2);
        std::string node2 = line.substr(3);
        adj[node1].insert(node2);
        adj[node2].insert(node1);
    }

    int result = 0;
    std::unordered_set<std::string> triangles;
    for(auto& [node, neighbors] : adj) {
        for(const auto& n1 : neighbors) {
            for(const auto& n2 : neighbors) {
                if(n1 == n2) continue;  // Skip duplicates and same node
                if(adj[n1].contains(n2)) {
                    std::vector<std::string> triangle = {node, n1, n2};
                    std::sort(triangle.begin(), triangle.end());
                    triangles.insert(triangle[0] + triangle[1] + triangle[2]);
                
                }
            }
        }
    }

    for(const auto& triangle : triangles) {
        if(triangle[0] == 't' || triangle[2] == 't' || triangle[4] == 't')
        result++;
    }

    std::cout << result << std::endl;

    // Bron-Kerbosch algorithm to find maximal clique
    std::unordered_set<std::string> R, P, X;
    std::vector<std::string> max_clique; // will store the result
    
    // Initialize P with all vertices
    for (const auto& [node, _] : adj) {
        P.insert(node);
    }
    
    findMaximalClique(adj, R, P, X, max_clique);
    std::sort(max_clique.begin(), max_clique.end());

    std::string password;
    for (size_t i = 0; i < max_clique.size(); ++i) {
        if (i > 0) password += ",";
        password += max_clique[i];
    }
    
    std::cout << password << std::endl;
    return 0;
}