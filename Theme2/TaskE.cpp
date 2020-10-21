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

void minPath(Matrix<int>& graph, std::vector<int>& path, const int start, const int finish);
void findTwoPaths (Matrix<int> web, const int start, const int finish);
void readGraph(Matrix<int>& graph, const int numOfEdges);
int maxFlow(Matrix<int> web, const int start, const int finish);
 
int main() {

    int size = 0, numOfEdges = 0;
    std::cin >> size >> numOfEdges;
    Matrix<int> web(size, size);

    int start = 0, finish = 0;    
    std::cin >> start >> finish;
    readGraph(web, numOfEdges);

    if (maxFlow(web, start - 1, finish - 1) < 2) {

        std::cout << "NO" << '\n';
        return 0;
    }

    findTwoPaths(web, start - 1, finish - 1);
}

void readGraph(Matrix<int>& graph, const int numOfEdges) {

    int from = 0, to = 0;
    for (int i = 0; i < numOfEdges; ++i) {
        std::cin >> from >> to;
        graph[from - 1][to - 1]++;
    }
}

void minPath(Matrix<int>& graph, std::vector<int>& path, const int start, const int finish) {

    int curPos = start;
    std::vector<bool> visited(graph.sizeX, false);
    std::vector<int> reversedPath(graph.sizeX, -1);
    
    visited[curPos] = true;
    std::queue<int> nextVertexes;
    nextVertexes.push(start);

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

    } while (!visited[finish] && !nextVertexes.empty());
   
    if (visited[finish]) {
        for (int i = finish; i != start && i != -1; i = reversedPath[i]) 
            path[reversedPath[i]] = i;
    }
}

void findTwoPaths(Matrix<int> web, const int start, const int finish) {

    std::vector<int> paths[2];
    for (int i = 0; i < 2; ++i)
        paths[i].resize(web.sizeX, start);

    minPath(web, paths[0], start, finish);

    if (paths[0][start] != start) {

        for (int i = start, j = paths[0][start]; j != start; i = j, j = paths[0][i]) {

            web[i][j]--;
        }

        minPath(web, paths[1], start, finish);

        if (paths[1][start] != start) {
            std::cout << "YES" << '\n';
            for (int i = 0; i < 2; ++i) {
                std::cout << start + 1 << ' ';
                for (int j = paths[i][start]; j != start; j = paths[i][j])
                    std::cout << j + 1 << ' ';
                std::cout << '\n';
            }
            return;
        }
    }

    std::cout << "NO";
}

int maxFlow(Matrix<int> web, const int start, const int finish) {

    int result = 0;
    
    std::vector<int> path(web.sizeX, start);
    minPath(web, path, start, finish);

    while(path[start] != start) {

        int minCap = web[start][path[start]];
        for (int i = start, j = path[start]; j != start; i = j, j = path[i])  {

            minCap = std::min(web[i][j], minCap);
        }
        
        for (int i = start, j = path[start]; j != start; i = j, j = path[i]) {

            web[i][j] -= minCap;
            web[j][i] += minCap;
        }

        result += minCap;
        path.clear();
        path.resize(web.sizeX, start);
        minPath(web, path, start, finish);
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

