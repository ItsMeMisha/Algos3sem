#include <iostream>
#include <string>
#include <vector>

using std::vector;
using std::string;

const int childrenNum = 26;

class Trie {

public:
    Trie();
    ~Trie();

    void addWord(const std::string& word);
    
    struct Node {
        char value = 0;
        Node* children[childrenNum + 1] = {};
        Node* parent = nullptr;
        bool isTerminate = false;
        vector<int> wordsIndx;

        Node* sufLink = nullptr;
        Node* shorLink = nullptr;
    };

private:
    Node* root = nullptr;
    std::string curWord;
    Node* position;
   
    void deleteBranch(Node* node);
    friend AhoKorasik;
};


class AhoKorasik {

public:
    AhoKorasik() = delete;
    AhoKorasik(const vector(string)& words);
    
    void operator() (const vector<string>& words);
    vector<vector<int>>* operator() (const string& str);

private:
    Trie bor;
    vector<string> dict;
    vector<vector<int>> positions;

    void createSufLinks();

};

class Solution() {

public:
    void read();
    void calculate();
    void write();

private:
    std::string data;
    vector<string> substrings;
    AhoKorasik bor;
};

int main() {

    Solution solution;
    solution.read();
    solution.calculate();
    solution.write();
    return 0;
}

AhoKorasik::AhoKorasik(const vector(string)& words) {
    this->operator() (words);
}

void operator() (const vector<string>& words) {
    for(auto word : words) {
        bor.addWord(word);
        dict.push_back(word);
        positions.push_back(vector<int>());
    }

    createSufLinks();
}

vector<vector<int>>* operator() (const string& str) {
    
}

void createSufLinks() {

    
}

void Solution::read() {

    std::cin >> data;
    int num = 0;
    std::coin >> num;
    substrings.resize(num);
    for (int i = 0; i < num; ++i)
        std::cin >> substring[i];
}

void Solution::write() {

}

void Solution::calculate() {

}

//========================================================================

Trie::Trie() : root(new Node) {
    position = root;
}

Trie::~Trie() {
    deleteBranch(root);
    root = nullptr;
    
}

void Trie::addWord(const std::string& word) {

    unsigned int length = word.size();
    if (length == 0)
        return;

    Node* curNode = root;
    char curChar = word[0];
    for (int i = 1; i <= length; ++i) {
       
        if (curNode->children[curChar - 'a'] == nullptr) {

            curNode->children[curChar - 'a'] = new Node;
            curNode->children[curChar - 'a']->parent = curNode;
        }

        curNode = curNode->children[curChar - 'a'];
        curNode->value = curChar;
        curChar = word[i];
    }

    curNode->isTerminate = true;
    curNode->repeat++;
}

void Trie::deleteBranch(Trie::Node* node) {

    for (int i = 0; i < childrenNum; ++i) {
        if (node->children[i] != nullptr)
            deleteBranch(node->children[i]);

        node->children[i] = nullptr;
    }

    if (node == root)
        root = nullptr;
    
    delete node;
}

