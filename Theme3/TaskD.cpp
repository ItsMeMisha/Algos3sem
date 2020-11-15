#include <iostream>
#include <string>

#include <stdio.h>

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
        bool visited = false;
        unsigned long long repeat = 0;
    };

    std::string getNextWord();    
private:
    Node* root = nullptr;
    std::string curWord;
    Node* position;
   
    void deleteBranch(Node* node);
};

class Solution {

public:
    void read();
    void sort();
    void write();

private:

    void fillTrie(Trie& trie);

    std::string data;
    std::string sorted;

    const char point = '.';
};

int main() {
    
    Solution solution;
    solution.read();
    solution.sort();
    solution.write();

    return 0;
}

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

std::string Trie::getNextWord() {

    if (position->repeat > 0) {
        position->repeat--;
        return curWord;
    }

    position->isTerminate = false;
    int i = 0;
    while (position->children[i] == nullptr && position != root) {
        if (i == childrenNum) {
            i = position->value - 'a' + 1;
            position = position->parent;
            curWord.pop_back();

        } else ++i;
    }

    while (!(position->isTerminate)) {
        for (i; position->children[i] == nullptr && i < childrenNum; ++i);
            if (i >= childrenNum) 
                return curWord;

            position = position->children[i];
            curWord.push_back(position->value);
            i = 0;
    }
    position->repeat--;
    return curWord;
}

void Solution::read() {
    std::cin >> data;
}

void Solution::write() {
    std::cout << sorted << '\n';
}

void Solution::sort() {
    Trie trie;
    fillTrie(trie);

    for (int i = 0; i < data.size();) {

        if (data[i] == point) {
            sorted.push_back(point);
            ++i;

        } else {
            std::string word = trie.getNextWord();
            sorted += word;
            while (data[i] != point && i < data.size())
                ++i;
        }
    }
}

void Solution::fillTrie(Trie& trie) {

    for (unsigned int i = 0; i < data.size(); ++i) {
        if (data[i] != point) {
            std::string word;
            while(data[i] != point && i < data.size()) {
                word.push_back(data[i]);
                ++i;
            }    
            trie.addWord(word);
        }        
    }
}

