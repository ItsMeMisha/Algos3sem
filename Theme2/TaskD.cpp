#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
//#include <utility>

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

struct Edge {

    Edge(int from, int to, int curCap) : 
        from(from), to(to), curCap(curCap) {};

    int from = 0;
    int to = 0;
    int curCap = 0;
};

struct Graph {
    Graph(int numOfVertexes) : size(numOfVertexes) {
        vertexes = new std::vector<int>[size];
    }
    
    Graph(const Graph& other) : size(other.size) {
        vertexes = new std::vector<int>[size];
        for (int i = 0; i < size; ++i)
            vertexes[i] = other.vertexes[i];
        edges = other.edges;
    }

    Graph& operator= (const Graph& other) {
        for (int i = 0; i < size; ++i)
            vertexes[i] = other.vertexes[i];
        edges = other.edges;
    }
    
    ~Graph() {
        delete[] vertexes;
    }
    
    const int size = 0;
    std::vector<int>* vertexes = nullptr;
    std::vector<Edge> edges;

    void addEdge(const Edge& edge) {
        edges.push_back(edge);
        vertexes[edge.from].push_back(edges.size() - 1);
    }
};
//==============================================================================

const char EmptyCell = '.';

void minPath(Graph& graph, std::vector<int>& path);
int maxFlow(Graph web);
int readTable(Matrix<char>& graph);
void buildGraph(const Matrix<char>& source, Graph& resultGraph);
int numOfConnections(const char Elem);

int main() {
    int sizeY = 0, sizeX = 0;
    std::cin >> sizeY >> sizeX;
    Matrix<char> table(sizeX, sizeY);
    int sumOfConnections = readTable(table);
    if (sumOfConnections <= 1) {
        std::cout << "Invalid";
        return 0;
    }

    Graph graph (sizeY * sizeX * 2 + 2);
    buildGraph(table, graph);

    int flow = maxFlow(graph);
    if (flow == sumOfConnections)
        std::cout << "Valid";
    else std::cout << "Invalid";
}

int numOfConnections(const char Elem) {

    switch (Elem) {

    case 'H': return 1;
    case 'O': return 2;
    case 'N': return 3;
    case 'C': return 4;
    default: return 0;

    }
}

void buildGraph(const Matrix<char>& source, Graph& resultGraph) {

    for (int i = 0; i < source.sizeY; ++i)
        for (int j = 0; j < source.sizeX; ++j) {
            if (source[i][j] == EmptyCell) continue;
            int connections = numOfConnections(source[i][j]);
            int elemIndx = i * source.sizeX + j + 1;
            int doubleElemIndx = elemIndx + source.sizeX * source.sizeY;
            resultGraph.addEdge(Edge(0, elemIndx, connections));
            resultGraph.addEdge(Edge(elemIndx, 0, 0));
           
            resultGraph.addEdge(Edge(doubleElemIndx, resultGraph.size - 1, connections));
            resultGraph.addEdge(Edge(resultGraph.size - 1, doubleElemIndx, 0));
           
            int xShift[4] = {1, -1, 0, 0};
            int yShift[4] = {0, 0, 1, -1};
            for (int k = 0; k < 4; ++k) {
                int nearY = i + yShift[k];
                int nearX = j + xShift[k];
                if (nearY < source.sizeY && nearY >= 0)
                    if (nearX < source.sizeX && nearX >= 0)
                        if (source[nearY][nearX] != EmptyCell) {
                            int doubleNearElem = nearY * source.sizeX + nearX + 1 + source.sizeX * source.sizeY;
                            resultGraph.addEdge(Edge(elemIndx, doubleNearElem, 1));
                            resultGraph.addEdge(Edge(doubleNearElem, elemIndx, 0));
                        }
            }
        }
}

int readTable(Matrix<char>& graph) {
    char symbol = 0;
    int sum = 0;
    for (int i = 0; i < graph.sizeY; ++i) 
        for (int j = 0; j < graph.sizeX; ++j) {
            std::cin >> symbol;
            sum += numOfConnections(symbol);
            graph[i][j] = symbol;
        }
    return sum;
}

void minPath(Graph& graph, std::vector<int>& path) {

    int curPos = 0;
    std::vector<bool> visited(graph.size, false);
    std::vector<int> reversedPath(graph.size, -1);
    
    visited[curPos] = true;
    std::queue<int> nextVertexes;
    nextVertexes.push(0);

    do {
        if (!nextVertexes.empty()) {
            curPos = nextVertexes.front();
            nextVertexes.pop();
        }

        for (auto edge : graph.vertexes[curPos]) {
            auto curEdge = graph.edges[edge];
            if (!visited[curEdge.to]) {
                visited[curEdge.to] = true;
                nextVertexes.push(curEdge.to);
                reversedPath[curEdge.to] = edge;
            }
        }

    } while (!visited[graph.size - 1] && !nextVertexes.empty());
   
    if (visited[graph.size - 1]) {
        for (int i = graph.size - 1; i != 0 && i != -1; i = graph.edges[reversedPath[i]].from) 
            path[graph.edges[reversedPath[i]].from] = reversedPath[i];
    }
}


int maxFlow(Graph web) {

    int result = 0;
    
    std::vector<int> path(web.size);
    minPath(web, path);

    while(path[0] != 0) {

        int minCap = web.edges[path[0]].curCap;
        for (int i = path[0]; i != web.size - 1; i = path[web.edges[i].to]) 
            minCap = std::min(web.edges[i].curCap, minCap);
        
        for (int i = path[0]; i != web.size - 1; i = path[web.edges[i].to]) {

            web.edges[i].curCap -= minCap;
            web.edges[i+1].curCap += minCap;
        }

        result += minCap;
        path.clear();
        path.resize(web.size, 0);
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



