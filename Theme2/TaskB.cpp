#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

template <typename T>
struct Matrix {

    Matrix(const int sizeX, const int sizeY);
    Matrix(const int size) : Matrix(size, size) {};
    std::vector<std::vector<T>> data;
    std::vector<T>& operator[](const int indx);
    const std::vector<T>& operator[](const int indx) const;

    const int sizeX = 0;
    const int sizeY = 0;

};

void dump(const Matrix<int>& matr) {

    for (auto i : matr.data) {
        for (auto j : i)
            std::cout << j << ' ';

        std::cout << '\n';
    }
}


//==============================================================================

const int INF = -1;

void minPath(Matrix<int>& graph, std::vector<int>& path);
int maxFlow(Matrix<int> web);
void readSpecialGraph(Matrix<int>& graph, int numOfPeople, int numOfTeaTypes);
int getMaxDuration(Matrix<int> graph, int numOfTeaTypes);

int main() {

    int numOfPeople = 0, numOfTeaTypes = 0;
    std::cin >> numOfPeople >> numOfTeaTypes;
    Matrix<int> graph(numOfPeople + numOfTeaTypes + 2);
    readSpecialGraph(graph, numOfPeople, numOfTeaTypes);
    std::cout << getMaxDuration(graph, numOfTeaTypes);
}

void readSpecialGraph(Matrix<int>& graph, int numOfPeople, int numOfTeaTypes) {

    int teaAmount = 0;
    for (int i = numOfPeople + 1; i < graph.sizeX - 1; ++i) {
        std::cin >> teaAmount;
        graph[i][graph.sizeX - 1] = teaAmount;
    }
    
    for (int i = 1; i <= numOfPeople; ++i) {
        int numOfPrefered = 0;
        std::cin >> numOfPrefered;
        for (int j = 0; j < numOfPrefered; ++j) {
            int type = 0;
            std::cin >> type;
            graph[i][numOfPeople + type] = INF;
        }
    }
}

int getMaxDuration(Matrix<int> graph, int numOfTeaTypes) {
    int numOfPeople = graph.sizeX - numOfTeaTypes - 2;

    int leftBound = 0;
    int rightBound = 0;

    for (int i = numOfPeople + 1; i < graph.sizeX - 1; ++i)
        rightBound += graph[i][graph.sizeX - 1];
    rightBound /= numOfPeople;

    while (leftBound < rightBound - 1) {
        int middle = (leftBound + rightBound) / 2;

        for (int i = 1; i <= numOfPeople; ++i)
            graph[0][i] = middle;

        if (maxFlow(graph) < middle * numOfPeople)
            rightBound = middle;
        else
            leftBound = middle;
    }

    for (int i = 1; i <= numOfPeople; ++i)
        graph[0][i] = rightBound;

    if (maxFlow(graph) == rightBound * numOfPeople)
        return rightBound;
    else
        return leftBound;
}

void minPath(Matrix<int>& graph, std::vector<int>& path) {

    int curPos = 0;
    std::vector<bool> visited(graph.sizeX, false);
    std::vector<int> reversedPath(graph.sizeX, -1);
    
    visited[curPos] = true;
    std::queue<int> nextVertexes;
    nextVertexes.push(0);

    do {
        if (!nextVertexes.empty()) {
            curPos = nextVertexes.front();
            nextVertexes.pop();
        }

        for (int i = 0; i < graph.sizeX; ++i)
            if (((graph[curPos][i] > 0) || (graph[curPos][i] == INF)) && !visited[i]) {
                visited[i] = true;
                nextVertexes.push(i);
                reversedPath[i] = curPos;
            }

    } while (!visited[graph.sizeX - 1] && !nextVertexes.empty());

    if (visited[graph.sizeX - 1]) {
        int i = graph.sizeX - 1;
        for (int i = graph.sizeX - 1; i != 0 && i != -1; i = reversedPath[i]) 
            path[reversedPath[i]] = i;
    }
/*std::cout << '\n' << "path 0"<<path[0] << '\n';
for (int i = 0, j = path[0]; j != 0; i = j, j = path[i])
    std::cout << i << "->" << j << '\n';*/

}


int maxFlow(Matrix<int> web) {

    int result = 0;
    std::vector<int> path(web.sizeX);
    minPath(web, path);

    while(path[0] != 0) {

/*  std::cout << "Before:" << '\n';
dump(web);
std::cout << '\n';   */
        int minCap = web[0][path[0]];
        for (int i = 0, j = path[0]; j != 0; i = j, j = path[i])
            if (web[i][j] != INF)
                minCap = std::min(web[i][j], minCap);
        
        for (int i = 0, j = path[0]; j != 0; i = j, j = path[i]) {
            if (web[i][j] != INF) {
                web[i][j] -= minCap;}
                web[j][i] += minCap;
            
        }

        result += minCap;
        path.clear();
        path.resize(web.sizeX, 0);
/*std::cout << "After:" << '\n';
dump(web);
std::cout << '\n'; */
        minPath(web, path);
//std::cout << '\n' << "Path 0: "<< path[0] << '\n';
    }

    return result;
}

//==============================================================================

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



