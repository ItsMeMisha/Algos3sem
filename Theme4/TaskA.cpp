#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstddef>

using std::vector;
using std::string;

class SuffixArray {
public:
    SuffixArray() = default;
    SuffixArray(const string& str);
    vector<int> operator() (const string& str);
    vector<int> getLastResult();
private:
    struct Suffix {
        Suffix(const size_t position, const size_t eqClass) : position(position), eqClass(eqClass) {};
        size_t position = 0;
        size_t eqClass = 0;
        bool operator< (const Suffix& suf);
        bool operator== (const Suffix& suf);
    };

    struct SufPair {
        SufPair(const Suffix& suf1, const Suffix& suf2) : suf1(suf1), suf2(suf2) {};
        Suffix suf1;
        Suffix suf2;
        bool operator< (const SufPair& second) {
            return (suf1 == second.suf1 ? suf2 < second.suf2 : suf1 < second.suf1);
        }
        bool operator!= (const SufPair& second) {
            return (suf1.eqClass != second.suf1.eqClass || suf2.eqClass != second.suf2.eqClass);
        }
    };

    void sortAll();
    void sortInit();
//Need to clean this section from unused members:
    vector<vector<int>> classes;
    vector<Suffix> sufs;
    vector<int> array; //--- map for suffixes and positions
    string _str;
    size_t size;
};

class Solution {
public:
    Solution() = default;
    void read();
    void calculate();
    void write();
private:
    string str;
    SuffixArray suffixes;
    vector<int> result;
};

int main() {

    Solution solution;
    solution.read();
    solution.calculate();
    solution.write();
}

void Solution::read() {
    std::cin >> str;
}

void Solution::write() {
    for (auto i : result)
        std::cout << i + 1 << ' ';
    std::cout <<'\n';
}

void Solution::calculate() {
    result = suffixes(str);
}

SuffixArray::SuffixArray(const string& str){
    this->operator() (str);
}

vector<int> SuffixArray::operator() (const string& str) {
    _str = str;
    _str.push_back('$');
    size = _str.size();
    _str.append(_str);
    
    for (int i = 0; i < size; ++i)
        sufs.emplace_back(i, 0);
    sortAll();
    array.erase(array.begin());

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

        for (int i = 0; i < pairs.size(); ++i)
            array[i] = pairs[i].suf1.position;

        int curClass = 0;
        newClasses[array[0]] = 0;
        
        for (int i = 1; i < array.size(); ++i) {
            if (unsorted[array[i]] != unsorted[array[i-1]])
                ++curClass;

            newClasses[array[i]] = curClass;
            sufs[array[i]].eqClass = curClass;
        }
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
    for (auto& sufVector: classes) {
        if (curClass == 0) {
            array.push_back(size - 1);
            ++curClass;

        } else if (!sufVector.empty()) {
            for (auto& suf: sufVector) {
                array.push_back(sufs[suf].position);
                sufs[suf].eqClass = curClass;
            }
            ++curClass;
        }
    }
}

vector<int> SuffixArray::getLastResult() {
    return array;
}

bool SuffixArray::Suffix::operator< (const SuffixArray::Suffix& suf) {
    return (this->eqClass < suf.eqClass);
}

bool SuffixArray::Suffix::operator== (const SuffixArray::Suffix& suf) {
    return (this->eqClass == suf.eqClass);
}


