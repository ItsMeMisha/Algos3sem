#include <iostream>
#include <vector>
#include <fstream>

using MatrixVertexes = std::vector<std::vector<int>>;

template <typename T>
struct Matrix {

    Matrix(const int sizeX, const int sizeY);

    std::vector<std::vector<T>> data;
    std::vector<T>& operator[](const int indx);
    const std::vector<T>& operator[](const int indx) const;

    const int sizeX = 0;
    const int sizeY = 0;

};


const int NotInMatching = -1;
const char FreeCell = '*';
const char OccupiedCell = '.';

const char* input = "dominoes.in";
const char* output = "dominoes.out";

void readParameters (int& sizeN, int& sizeM, int& bigDominoCost, int& smallDominoCost, std::ifstream& file);
int readFieldAndCountFreeCells (Matrix<char>& field, std::ifstream& file);
int separateCells (Matrix<char>& field, MatrixVertexes& adjacencyMatrix); //returnes size of the second set
void checkPositionAndAddVertex (Matrix<char>& field, MatrixVertexes& adjacencyMatrix, Matrix<int>& relation, int& setBSize, const int posI, const int posJ, const int setASize); 
int maxMatches (MatrixVertexes& adjacencyMatrix, std::vector<int>& matchings);
bool DFSChains (const int vertex, MatrixVertexes& adjacencyMatrix, std::vector<int>& matchings, std::vector<bool>& visited);

/*  //DEBUG

template <typename T>
void dump(const T& matr) {

    for (auto i : matr) {
        for (auto j : i)
            std::cout << j << ' ';

        std::cout << '\n';
    }
}
*/
int main() {

    int sizeN = 0;
    int sizeM = 0;
    int bigDominoCost = 0;
    int smallDominoCost = 0;

    std::ifstream inputFile;
    inputFile.open (input);

    readParameters(sizeN, sizeM, bigDominoCost, smallDominoCost, inputFile);
    
    Matrix<char> field (sizeM, sizeN);

    int numOfFreeCells = readFieldAndCountFreeCells (field, inputFile);
    inputFile.close();

    std::ofstream outputFile;

    if (bigDominoCost >= 2 * smallDominoCost) {

        outputFile.open (output);
        outputFile << numOfFreeCells * smallDominoCost;
        outputFile.close();
        return 0;
    }

    int setASize = 0;
    MatrixVertexes adjacencyMatrix;
    
    int setBSize = separateCells (field, adjacencyMatrix);

//    dump(adjacencyMatrix);        //DEBUG

    std::vector<int> matchings (setBSize, NotInMatching);

    int sizeOfMaxMatching = maxMatches (adjacencyMatrix, matchings);

    outputFile.open (output);
    outputFile << sizeOfMaxMatching * bigDominoCost + (numOfFreeCells - 2 * sizeOfMaxMatching) * smallDominoCost;
    outputFile.close();
    return 0;
}

void readParameters (int& sizeN, int& sizeM, int& bigDominoCost, int& smallDominoCost, std::ifstream& file) {

    file >> sizeN >> sizeM >> bigDominoCost >> smallDominoCost;
}

int readFieldAndCountFreeCells (Matrix<char>& field, std::ifstream& file) {

    char cell = 0;
    int numOfFreeCells = 0;

    for (auto& line : field.data)
        for (auto& elem : line) {

            file >> cell;
            elem = cell;
            if (cell == FreeCell)
                ++numOfFreeCells;
        }

    return numOfFreeCells;
}

int separateCells (Matrix<char>& field, MatrixVertexes& adjacencyMatrix) {

    Matrix<int> PositionToNumberRelation (field.sizeX, field.sizeY);
    auto& relation = PositionToNumberRelation;    

    int setASize = 0;
    int setBSize = 0;

    for (int i = 0, j = 0; i < field.sizeY; ++i)
        for (j = (i % 2 == 1) ? 1 : 0; j < field.sizeX; j += 2) {

            if (field[i][j] == FreeCell) {

                adjacencyMatrix.push_back (std::vector<int>());
                relation[i][j] = setASize;
                ++setASize;
                
                for (int nearI = i - 1; nearI < i + 2; nearI += 2)
                    checkPositionAndAddVertex (field, adjacencyMatrix, relation, setBSize, nearI, j, setASize);

                for (int nearJ = j - 1; nearJ < j + 2; nearJ += 2)
                    checkPositionAndAddVertex (field, adjacencyMatrix, relation, setBSize, i, nearJ, setASize);
            }
        }


/*      //DEBUG
    dump(relation.data);
    dump(field.data);
*/
    return setBSize;
} 

void checkPositionAndAddVertex (Matrix<char>& field, MatrixVertexes& adjacencyMatrix, Matrix<int>& relation, int& setBSize, const int posI, const int posJ, const int setASize) {

    if (posI >= 0 && posI < field.sizeY && posJ >= 0 && posJ < field.sizeX)
        if (field[posI][posJ] == FreeCell) {
            
            if (relation[posI][posJ] == 0) {
                ++setBSize;
                relation[posI][posJ] = setBSize;
            }

            adjacencyMatrix[setASize - 1].push_back(relation[posI][posJ]);
        }            
}

int maxMatches (MatrixVertexes& adjacencyMatrix, std::vector<int>& matchings) {

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

bool DFSChains (const int vertex, MatrixVertexes& adjacencyMatrix, std::vector<int>& matchings, std::vector<bool>& visited) {

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

//=================================================================================================

template <typename T>
Matrix<T>::Matrix(const int sizeX, const int sizeY) : sizeX(sizeX), sizeY(sizeY) {

    data.resize(sizeY);
    data.shrink_to_fit();
    for (auto& line : data) {

        line.resize(sizeX);
        line.shrink_to_fit();

    }

}

template <typename T>
std::vector<T>& Matrix<T>::operator[](const int indx) {
    return data[indx];
}

template <typename T>
const std::vector<T>& Matrix<T>::operator[](const int indx) const {
    return data[indx];
}

