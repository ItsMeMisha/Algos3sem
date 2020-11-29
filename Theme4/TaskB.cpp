#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstddef>
#include <cmath>
#include <cstdint>

using std::vector;
using std::string;

class SuffixArray {
public:
    SuffixArray() = default;
    SuffixArray(const string& str);
    vector<uint64_t> getLCPArray();
    string unique(uint64_t pos);
    vector<uint64_t> operator() (const string& str);
    vector<uint64_t> getLastResult();
private:
    struct Suffix {
        Suffix(const uint64_t position, const uint64_t eqClass) : position(position), eqClass(eqClass) {};
        uint64_t position = 0;
        uint64_t eqClass = 0;
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
    vector<vector<uint64_t>> classes;
    vector<Suffix> sufs;
    vector<uint64_t> array; //--- map for suffixes and positions
    vector<uint64_t> reverseArray;
    string _str;
    uint64_t size;
    vector<uint64_t> LCP;
    void generateLCP();
    uint64_t len(const Suffix& suf) {
        return size - 1 - suf.position;
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
    uint64_t pos;
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

vector<uint64_t> SuffixArray::getLCPArray() {
    return LCP;
}

string SuffixArray::unique(size_t pos) {
    uint64_t curPos = 0;
    string result;
    for (uint64_t i = 1; i < LCP.size(); ++i) {
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
    uint64_t curLCP = 0;
    
    for (uint64_t i = 0; i < size; ++i) {
        if (reverseArray[i] == size - 1) {
            curLCP = 0;
            continue;
        }

        uint64_t next = array[reverseArray[i] + 1];
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

vector<uint64_t> SuffixArray::operator() (const string& str) {
    _str = str;
    _str.push_back('$');
    size = _str.size();
    LCP.resize(size, 0);
    _str.append(_str);
    
    for (uint64_t i = 0; i < size; ++i)
        sufs.emplace_back(i, 0);
    sortAll();
//    array.erase(array.begin()); --- $-suffix
    generateLCP();
    return array;
}

/* Sorts All Suffixes of _str */
void SuffixArray::sortAll() {

    sortInit();
    vector<uint64_t> newClasses(array.size(), 0);
    uint64_t curLenAdd = 1;
    while (curLenAdd <= size) {

        vector<SufPair> pairs;
        for (uint64_t i = 0; i < array.size(); ++i)
            pairs.emplace_back(sufs[i], sufs[(i + curLenAdd) % size]);

        auto unsorted = pairs;
        std::sort(pairs.begin(), pairs.end());
        for (uint64_t i = 0; i < pairs.size(); ++i) {
            array[i] = pairs[i].suf1.position;
            reverseArray[pairs[i].suf1.position] = i;
        }

        uint64_t curClass = 0;
        newClasses[array[0]] = 0;
        
        for (uint64_t i = 1; i < array.size(); ++i) {
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
    classes.resize('z' - 'a' + 2, vector<uint64_t>(0));
    for (auto suf: sufs) {
        if (_str[suf.position] == '$')
            classes[0].push_back(suf.position);

        else
            classes[_str[suf.position] - 'a' + 1].push_back(suf.position);
    }
    
    uint64_t curClass = 0;
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

vector<uint64_t> SuffixArray::getLastResult() {
    return array;
}

bool SuffixArray::Suffix::operator< (const SuffixArray::Suffix& suf) const {
    return (this->eqClass < suf.eqClass);
}

bool SuffixArray::Suffix::operator== (const SuffixArray::Suffix& suf) const {
    return (this->eqClass == suf.eqClass);
}

