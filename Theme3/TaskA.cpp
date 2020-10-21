#include <iostream>
#include <string>
#include <vector>
#include <cstddef>

using std::string;
using std::vector;

class PrefixFunction {

public:
    PrefixFunction(const string& str);
    PrefixFunction() = default;

    void operator() (const string& string);
    int operator[] (const size_t indx) const;
    size_t size() const;

private:
    vector<int> values;
};

class SubstringFinder {

public: 
    SubstringFinder(const string& stringS, const string& stringT) :
        _stringS(stringS), _stringT(stringT) {};

    SubstringFinder() = default;

    void printStandings();
    void findSubstringPositions();
    void readStrings();

    const string& stringS() const;
    const string& stringT() const;

private:

    string _stringS;
    string _stringT;
    vector<int> standings;
    bool found = false;
};

int main () {

    SubstringFinder finder;
    finder.readStrings();
    finder.printStandings();
    return 0;
}

PrefixFunction::PrefixFunction(const string& str) {
    this->operator() (str);
}

void PrefixFunction::operator() (const string& str) {
    size_t length = str.length();
    values.push_back(0);

    for (int i = 1; i < length; ++i){
        int maxPref = 0;
        for (maxPref = values[i-1]; 
            maxPref > 0 && str[i] != str[maxPref]; 
                maxPref = values[maxPref-1]);

        if (str[i] == str[maxPref])
            ++maxPref;

        values.push_back(maxPref);
    }
}

int PrefixFunction::operator[] (const size_t indx) const {
    return values[indx];
}

size_t PrefixFunction::size() const {
    return values.size();
}

void SubstringFinder::printStandings() {

    if (!found) {
        findSubstringPositions();
    }
    
    for (auto pos : standings)
        std::cout << pos << ' ';
    std::cout << '\n';
}

void SubstringFinder::findSubstringPositions() {

    PrefixFunction pi(_stringT);
    size_t requiredLength = _stringT.length();

    int maxPref = 0;
    for (int i = 0; i < _stringS.length(); ++i){
        for (maxPref; 
            maxPref > 0 && _stringS[i] != _stringT[maxPref]; 
                maxPref = pi[maxPref-1]);

        if (_stringS[i] == _stringT[maxPref])
            ++maxPref;

        if (maxPref == requiredLength)
            standings.push_back(i - requiredLength + 1);
    }
}

void SubstringFinder::readStrings() {
    std::cin >> _stringS >> _stringT;
}

const string& SubstringFinder::stringS() const {return _stringS;}

const string& SubstringFinder::stringT() const {return _stringT;}

