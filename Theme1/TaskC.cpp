#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>

const int MinInHrs = 60;
const int NotInMatching = -1;

using MatrixVertexes = std::vector<std::vector<int>>;

struct Time {

    Time (const int& hours, const int& minutes) : 
        hours(hours), minutes(minutes), total(hours * MinInHrs + minutes) {}

    Time (const int& total = 0) :
        hours(total / MinInHrs), minutes (total % MinInHrs), total (total) {}

    int hours = 0;
    int minutes = 0;
    int total = 0;

    bool operator < (const int& rightOp) const { return (total < rightOp); }
    bool operator < (const Time& rightOp) const { return (total < rightOp.total); }

    Time operator + (const Time& rightOp) const { return Time(total + rightOp.total); }
    Time operator + (const int& rightOp) const { return Time(total + rightOp); }

    operator int() const { return total; }

#ifdef __DEBUG
    void print () const {
        std::cout << hours << ':' << minutes << ' ';
    }
#endif

};

struct Place {

    Place (const int& x, const int& y) : x(x), y(y) {}
    int x = 0;
    int y = 0;

    int distanceTo (const Place& destination) const {

        return (std::abs (x - destination.x) + std::abs (y - destination.y));
    }

#ifdef __DEBUG
    void print () const {
        std::cout << x << ' ' << y << ' ';
    }
#endif
};

struct Order {

    Order (const Time& start, const Place& from, const Place& to) : 
        start(start), from(from), to(to) {

        duration = Time(from.distanceTo(to));
        total = Time(start + duration);
    }

    Time start;
    Time duration;
    Time total;

    Place from;
    Place to;

    bool ableTakeNext (const Order& next) const {

#ifdef __DEBUG

        total.print();
        std::cout << to.distanceTo(next.from) << ' ';
        next.start.print();
        std::cout << '\n';

#endif

        if (total + to.distanceTo(next.from) + 1 <= next.start)
            return true;

        return false;
    }

#ifdef __DEBUG

    void print () const {
        start.print();
        from.print();
        to.print();
        std::cout << '\n';
    }

#endif

};

void readOrders (std::vector<Order>& orders);
void buildGraph (const std::vector<Order>& orders, MatrixVertexes& adjacencyMatrix);
int maxMatches (MatrixVertexes& adjacencyMatrix, std::vector<int>& matchings);
bool DFSChains (const int vertex, MatrixVertexes& adjacencyMatrix, std::vector<int>& matchings, std::vector<bool>& visited);


int main () {

    std::vector<Order> orders;
    readOrders (orders);

    MatrixVertexes adjacencyMatrix (orders.size());
    buildGraph (orders, adjacencyMatrix);
    
    std::vector<int> matchings (orders.size(), NotInMatching);
    int maxMatch = maxMatches (adjacencyMatrix, matchings);

    std::cout << orders.size() - maxMatch;
/*
    for (auto& i : orders)
        i.print();
*/
    return 0;
}

void readOrders (std::vector<Order>& orders) {

    int numOfOrders = 0;
    scanf ("%d", &numOfOrders);

    int hours = 0, minutes = 0,
        fromX = 0, fromY = 0,
        toX = 0, toY = 0;

    for (int i = 0; i < numOfOrders; ++i) {

        scanf ("%d:%d %d %d %d %d", &hours, &minutes, &fromX, &fromY, &toX, &toY);
        orders.emplace_back (Time(hours, minutes), Place(fromX, fromY), Place(toX, toY));
    }        
}

void buildGraph (const std::vector<Order>& orders, MatrixVertexes& adjacencyMatrix) {

    const int numOfOrders = orders.size();
    for (int i = 0; i < numOfOrders - 1; ++i)
        for (int j = i + 1; j < numOfOrders; ++j)
            if (orders[i].ableTakeNext(orders[j]))
                adjacencyMatrix[i].push_back(j);
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


