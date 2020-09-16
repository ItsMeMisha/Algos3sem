#include <iostream>
#include <vector>
#include <algorithm>

using MatrixInt = std::vector<std::vector<int>>;

const int NotInMatching = -1;

void readSize (int& setASize, int& setBSize);
void readEdges (MatrixInt& adjacencyMatrix, const int& setASize);
int maxMatches (MatrixInt& adjacencyMatrix, std::vector<int>& matchings);
bool DFSChains (const int vertex, MatrixInt& adjacencyMatrix, std::vector<int>& matchings, std::vector<bool>& visited);

int main() {

    int setASize = 0;
    int setBSize = 0;

    readSize(setASize, setBSize);
    
    MatrixInt adjacencyMatrix (setASize);
    readEdges (adjacencyMatrix, setASize);

    std::vector<int> matchings (setBSize, NotInMatching);

    int sizeOfMaxMatching = maxMatches (adjacencyMatrix, matchings);
    
    std::cout << sizeOfMaxMatching << '\n';
    for (int vertexB = 0; vertexB < setBSize; ++vertexB)
        if (matchings[vertexB] != NotInMatching)
            std::cout << matchings[vertexB] + 1 << " " << vertexB + 1 << '\n';

    return 0;
}

void readSize (int& setASize, int& setBSize) {

    std::cin >> setASize >> setBSize;
}

void readEdges (MatrixInt& adjacencyMatrix, const int& setASize) {

    int vertex = 0;
    for (int i = 0; i < setASize; ++i)
        for (std::cin >> vertex; vertex != 0; std::cin >> vertex)
            adjacencyMatrix[i].push_back (vertex);
           
}

int maxMatches (MatrixInt& adjacencyMatrix, std::vector<int>& matchings) {

    std::vector<bool> visited (adjacencyMatrix.size());
    for (int vertex = 0; vertex < adjacencyMatrix.size(); ++vertex) {

        for (int i = 0; i < adjacencyMatrix.size(); ++i)
            visited[i] = false;
        DFSChains (vertex, adjacencyMatrix, matchings, visited);
    }

    int numOfMatches = 0;

    for (auto match : matchings)
        if (match != NotInMatching)
            ++numOfMatches;

    return numOfMatches;
}

bool DFSChains (const int vertex, MatrixInt& adjacencyMatrix, std::vector<int>& matchings, std::vector<bool>& visited) {

    if (visited[vertex])
        return false;

    visited[vertex] = true;
    for (auto connectedVertex : adjacencyMatrix[vertex])
        if (matchings[connectedVertex - 1] == NotInMatching || DFSChains (matchings[connectedVertex - 1], adjacencyMatrix, matchings, visited)) {

            matchings[connectedVertex - 1] = vertex;
            return true;
        }

    return false;
}

