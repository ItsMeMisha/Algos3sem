#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstddef>
#include <cmath>

using std::vector;
using std::string;
/*
template <typename T>
class SparseTable {

    struct Matrix {

        Matrix(const int size);
        vector<std::vector<T>> data;
        vector<T>& operator[](const int indx);
        const int size;
        const int height;

    };

    const int size;
    const T& (*func)(const T&, const T&);
    Matrix values;
    void preCount(const int line = 1);
    
public:

    SparseTable(vector<T> data, const T& (*func)(const T&, const T&));
    T getFunctionValue(const int left, const int right);
void dump() {
for (const auto& i: values.data) {
    for(const auto& j: i)
        std::cout << j << ' ';
    std::cout << '\n';
    }
}
};
*/
//=======================================================
class SuffixArray {
public:
    SuffixArray() = default;
    SuffixArray(const string& str);
    vector<size_t> getLCPArray();
    string unique(size_t pos);
    vector<int> operator() (const string& str);
    vector<int> getLastResult();
private:
    struct Suffix {
        Suffix(const size_t position, const size_t eqClass) : position(position), eqClass(eqClass) {};
        size_t position = 0;
        size_t eqClass = 0;
        bool operator< (const Suffix& suf) const;
        bool operator== (const Suffix& suf) const;
        
    };

    struct SufPair {
        SufPair(const Suffix& suf1, const Suffix& suf2) : suf1(suf1), suf2(suf2) {};
        Suffix suf1;
        Suffix suf2;
        bool operator< (const SufPair& second) const {
            return (suf1 == second.suf1 ? suf2 < second.suf2 : suf1 < second.suf1);
        }
        bool operator!= (const SufPair& second) const {
            return (suf1.eqClass != second.suf1.eqClass || suf2.eqClass != second.suf2.eqClass);
        }
    };

    void sortAll();
    void sortInit();
//Need to clean this section from unused members:
    vector<vector<int>> classes;
    vector<Suffix> sufs;
    vector<int> array; //--- map for suffixes and positions
    vector<int> reverseArray;
    string _str;
    size_t size;
    vector<size_t> LCP;
    void generateLCP();
//    void generateLCP(const vector<SufPair>& sortedPairs);
    size_t len(const Suffix& suf) {
        size_t len = 0;
        size_t indx = suf.position;
        while (_str[indx] != '$') {
            ++len; ++indx;
        }
        return len;
    }

};

class Solution {
public:
    Solution() = default;
    void read();
    void calculate();
    void write();
private:
    string str;
    size_t pos;
    SuffixArray suffixes;
    string result;
};

int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    Solution solution;
    solution.read();
    solution.calculate();
    solution.write();
}

void Solution::read() {
    std::cin >> str >> pos;
}

void Solution::write() {
    std::cout << result << '\n';
}

void Solution::calculate() {
    suffixes(str);
    result = suffixes.unique(pos);
}

SuffixArray::SuffixArray(const string& str){
    this->operator() (str);
}

vector<size_t> SuffixArray::getLCPArray() {
    return LCP;
}

string SuffixArray::unique(size_t pos) {
    size_t curPos = 0;
    string result;
    for (size_t i = 1; i < LCP.size(); ++i) {
        if (pos - curPos <= len(sufs[array[i]]) - LCP[i-1]) {
            result.assign(_str, array[i], LCP[i-1] + pos - curPos);
            return result;
        }
        curPos += len(sufs[array[i]]) - LCP[i-1];
    }
    result.assign(_str, array[array.size() - 1], len(sufs[array[array.size() - 1]]));
    return result;
}

void SuffixArray::generateLCP() {
    int curLCP = 0;
    
    for (int i = 0; i < size; ++i) {
        if (reverseArray[i] == size - 1) {
            curLCP = 0;
            continue;
        }

        int next = array[reverseArray[i] + 1];
        while (std::max(i + curLCP, next + curLCP) < size) {
            if (_str[i + curLCP] != _str[next + curLCP])
                break;
            ++curLCP;
        }
        LCP[reverseArray[i]] = curLCP;
        if (curLCP > 0)
            --curLCP;
    }
}

/*void SuffixArray::generateLCP(const vector<SufPair>& sortedPairs) {
    SparseTable<size_t> table = SparseTable<size_t>(LCP, std::min);
    for (int i = 0; i < LCP.size() - 1; ++i)
        if (sortedPairs[i].suf1 == sortedPairs[i+1].suf1) {
            int left = std::min(reverseArray[sortedPairs[i].suf2.position], reverseArray[sortedPairs[i+1].suf2.position]);
            int right = std::max(reverseArray[sortedPairs[i].suf2.position], reverseArray[sortedPairs[i+1].suf2.position]);
            LCP[i] += table.getFunctionValue(left, right - 1);
        }
}*/

vector<int> SuffixArray::operator() (const string& str) {
    _str = str;
    _str.push_back('$');
    size = _str.size();
    LCP.resize(size, 0);
    _str.append(_str);
    
    for (int i = 0; i < size; ++i)
        sufs.emplace_back(i, 0);
    sortAll();
//    array.erase(array.begin()); --- $-suffix
    generateLCP();
    return array;
}

/* Sorts All Suffixes of _str */
void SuffixArray::sortAll() {

    sortInit();
    vector<int> newClasses(array.size(), 0);
    size_t curLenAdd = 1;
    while (curLenAdd <= size) {

        vector<SufPair> pairs;
        for (int i = 0; i < array.size(); ++i)
            pairs.emplace_back(sufs[i], sufs[(i + curLenAdd) % size]);

        auto unsorted = pairs;
        std::sort(pairs.begin(), pairs.end());
        for (int i = 0; i < pairs.size(); ++i) {
            array[i] = pairs[i].suf1.position;
            reverseArray[pairs[i].suf1.position] = i;
        }

        int curClass = 0;
        newClasses[array[0]] = 0;
        
        for (int i = 1; i < array.size(); ++i) {
            if (unsorted[array[i]] != unsorted[array[i-1]])
                ++curClass;

            newClasses[array[i]] = curClass;
            sufs[array[i]].eqClass = curClass;
        }
//        generateLCP(pairs);//may be earlier?
        curLenAdd *= 2;
    }
}

/* Initial sort, sorts lengths eq 1 */
void SuffixArray::sortInit() {
//count sort
    classes.resize('z' - 'a' + 2, vector<int>(0));
    for (auto suf: sufs) {
        if (_str[suf.position] == '$')
            classes[0].push_back(suf.position);

        else
            classes[_str[suf.position] - 'a' + 1].push_back(suf.position);
    }
    
    size_t curClass = 0;
    reverseArray.resize(size);
    for (auto& sufVector: classes) {
        if (curClass == 0) {
            array.push_back(size - 1);
            reverseArray[size - 1] = 0;
            ++curClass;

        } else if (!sufVector.empty()) {
            for (auto& suf: sufVector) {
                array.push_back(sufs[suf].position);
                reverseArray[sufs[suf].position] = array.size() - 1;
                sufs[suf].eqClass = curClass;
            }
            ++curClass;
        }
    }
}

vector<int> SuffixArray::getLastResult() {
    return array;
}

bool SuffixArray::Suffix::operator< (const SuffixArray::Suffix& suf) const {
    return (this->eqClass < suf.eqClass);
}

bool SuffixArray::Suffix::operator== (const SuffixArray::Suffix& suf) const {
    return (this->eqClass == suf.eqClass);
}

//======================================================
/*
template <typename T>
SparseTable<T>::Matrix::Matrix(const int size) : size(size), height(log2(size)+1) {

    data.resize(height);
    data.shrink_to_fit();
    for (auto& line : data) {

        line.resize(size);
        line.shrink_to_fit();

    }

}

template <typename T>
std::vector<T>& SparseTable<T>::Matrix::operator[](const int indx) {
    return data[indx];
}

template <typename T>
SparseTable<T>::SparseTable(vector<T> data, const T& (*func)(const T&, const T&)) : func(func), 
                                                                                  values(data.size()),
                                                                                  size(data.size()) {

    values[0] = data;
    preCount();

}

template <typename T>
void SparseTable<T>::preCount(const int line) {

    const int length = 1 << line;
    for (int i = 0; i < size - (length - 1); ++i)
        values[line][i] = func(values[line - 1][i], values[line - 1][i + length / 2]);

    if (line == values.height)
        return;

    preCount(line + 1);
}

template <typename T>
T SparseTable<T>::getFunctionValue(const int left, const int right) {

    int line = log2(right - left + 1);
    int length = 1 << line;

    return func(values[line][left], values[line][right - length + 1]);

}
*/
