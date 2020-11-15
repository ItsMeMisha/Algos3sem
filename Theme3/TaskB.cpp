#include <iostream>
#include <vector>
#include <algorithm>
#include <cstddef>

#pragma GCC optimize("Ofast")
#pragma GCC optimize(2)
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#pragma GCC optimize("-fgcse")
#pragma GCC optimize("-fgcse-lm")
#pragma GCC optimize("-fipa-sra")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("unroll-loops")
using std::vector;

class ZFunction {

public:
    ZFunction(const vector<int>& array);
    ZFunction() = default;

    void operator() (const vector<int>& array);
    int operator[] (const size_t indx) const;

private:
    int trivial(const vector<int>& array, int pos, int initVal = 0);
    vector<int> values;    
};

class Solution {

public:
    void read();
    void calculate();
    vector<int> data;
    int size;
};

int main() {

    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    Solution solution;
    solution.read();
    solution.calculate();
}

void Solution::read() {
    int numOfColors = 0;
    std::cin >> size >> numOfColors;
    data.resize(size * 2 + 1, -1);
    int buf = 0;
    for (int i = 0; i < size; ++i) {
        std::cin >> buf;
        data[i] = buf;
        data[2 * size - i] = buf;
    }
}

void Solution::calculate() {

    ZFunction zFunc(data);
    for (int i = size + 1; i < data.size(); ++i) 
        if (zFunc[i] == data.size() - i && zFunc[i] % 2 == 0) 
            std::cout << size - zFunc[i]/2 << ' ';
    
    std::cout << size << '\n';
}

ZFunction::ZFunction(const vector<int>& array) {
    this->operator()(array);
}

void ZFunction::operator() (const vector<int>& array) {

    int curRight = 0;
    int curLeft = 0;
    int size = array.size();
    values.push_back(size);

    for (int i = 1; i < size; ++i) {
        values.push_back(0);
        int val = std::max(0, std::min(curRight - i, values[i - curLeft]));
        val = trivial(array, i, val);
        
        if (val + i > curRight) {
            curLeft = i;
            curRight = i + val;
        }
        values[i] = val;
    
    } 
}

int ZFunction::trivial(const vector<int>& array, int pos, int initVal) {
    if (pos <= 0)
        return 0;
    
    int val = initVal;
    while ((pos + val < array.size()) && (array[pos + val] == array[val]))
        ++val;

    return val;
}

int ZFunction::operator[] (const size_t indx) const {
    return values[indx];
}



