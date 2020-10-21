#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

template <typename T>
struct Matrix {

    Matrix(const int sizeX, const int sizeY);
    std::vector<std::vector<T>> data;
    std::vector<T>& operator[](const int indx);
    const std::vector<T>& operator[](const int indx) const;

    const int sizeX = 0;
    const int sizeY = 0;

};

//==============================================================================

void minPath(Matrix<int>& graph, std::vector<int>& path);
int maxFlow(Matrix<int> web);
void readGraph(Matrix<int>& graph);

int main() {

    int size = 0;
    std::cin >> size;
    Matrix<int> web(size, size);
    readGraph(web);

    int flow = maxFlow(web);
    std::cout << flow;
}

void readGraph(Matrix<int>& graph) {
    int m = 0;
    int from = 0, to = 0, cap = 0;
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        std::cin >> from >> to >> cap;
        graph[from - 1][to - 1] = cap;
    }
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
            if (graph[curPos][i] > 0 && !visited[i]) {
                visited[i] = true;
                nextVertexes.push(i);
                reversedPath[i] = curPos;
            }

    } while (!visited[graph.sizeX - 1] && !nextVertexes.empty());
   
    if (visited[graph.sizeX - 1]) {
        for (int i = graph.sizeX - 1; i != 0 && i != -1; i = reversedPath[i]) 
            path[reversedPath[i]] = i;
    }
}


int maxFlow(Matrix<int> web) {

    int result = 0;
    
    std::vector<int> path(web.sizeX);
    minPath(web, path);

    while(path[0] != 0) {

        int minCap = web[0][path[0]];
        for (int i = 0, j = path[0]; j != 0; i = j, j = path[i]) 
            minCap = std::min(web[i][j], minCap);
        
        for (int i = 0, j = path[0]; j != 0; i = j, j = path[i]) {

            web[i][j] -= minCap;
            web[j][i] += minCap;
        }

        result += minCap;
        path.clear();
        path.resize(web.sizeX, 0);
        minPath(web, path);
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



