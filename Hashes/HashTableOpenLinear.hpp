#include "IHashSet.hpp"
#include <cstddef>
#include <vector>

template<typename Key, typename Value, class Hash>
class LinearHashTable : public IHashSet<Key, Value> {
public: void dump();
    LinearHashTable(const Hash& hash);
    void add(const Key& key, const Value& val) override;
    void remove(const Key& key) override;
    Value find(const Key& key, const Value& defVal) override;
    ~LinearHashTable();
private:
    struct Node {
        Node(const Key& key, const Value& val) : key(key), val(val) {}
        Key key;
        Value val;
    };
    const void* Tombstone;
    const size_t baseSize = 1024;
    size_t curSize = 0;
    const size_t resizeCoef = 2;
    void resize();
    void rehash(const size_t coef);
    size_t findPos(const Key& key);
    const Hash hashFunc;
    std::vector<Node*> table;
    size_t run(const size_t hash, const size_t iteration);
};

template<typename Key, typename Value, class Hash>
void LinearHashTable<Key, Value, Hash>::dump() {
    for (auto i : table) {
        if (i == nullptr)
            std::cout << "none\n";
        else if (i == Tombstone)
            std::cout << "Tombstone/none\n";
        else std::cout << i->key << ' ' << i->val << '\n';
    }
    std::cout << Tombstone << '\n';
}

template<typename Key, typename Value, class Hash>
LinearHashTable<Key, Value, Hash>::LinearHashTable(const Hash& hash) : 
        hashFunc(hash), table(baseSize, nullptr), Tombstone(&hash) {}

template<typename Key, typename Value, class Hash>
LinearHashTable<Key, Value, Hash>::~LinearHashTable() {
    for (auto elem : table)
        if (elem != nullptr && elem != Tombstone)
            delete elem;
}

template<typename Key, typename Value, class Hash>
void LinearHashTable<Key, Value, Hash>::add(const Key& key, const Value& val) {
    if (curSize == table.size() - 1)
        resize();
    ++curSize;
    size_t hash = hashFunc(key);
    size_t pos = hash % table.size();
    for (size_t i = 0; table[pos] != nullptr && table[pos] != Tombstone; pos = run(hash, ++i)) {
        if (table[pos]->key == key) {
            delete table[pos];
            break;
        }
    }
    table[pos] = new Node(key, val);
}

template<typename Key, typename Value, class Hash>
void LinearHashTable<Key, Value, Hash>::remove(const Key& key) {
    size_t hash = hashFunc(key);
    size_t pos = findPos(key);
    if (table[pos] == nullptr)
        return;

    --curSize;
    delete table[pos];
    size_t nextPos = (pos + 1) % table.size();
    if (table[nextPos] != nullptr)
        table[pos] = (Node*) Tombstone;
    else table[pos] = nullptr;
}

template<typename Key, typename Value, class Hash>
Value LinearHashTable<Key, Value, Hash>::find(const Key& key, const Value& defVal) {
    size_t pos = findPos(key);
    if (table[pos] == nullptr)
        return defVal;
    return table[pos]->val;
}

template<typename Key, typename Value, class Hash>
size_t LinearHashTable<Key, Value, Hash>::findPos(const Key& key) {
    size_t hash = hashFunc(key);
    size_t pos = hash % table.size();
    for (size_t i = 0;
        table[pos] != nullptr && (table[pos] != Tombstone ? table[pos]->key != key : true);
        pos = run(hash, ++i)) {

        }

    return pos;
}


template<typename Key, typename Value, class Hash>
size_t LinearHashTable<Key, Value, Hash>::run(const size_t hash, const size_t iteration) {
    return (hash + iteration) % table.size();
}

template<typename Key, typename Value, class Hash>
void LinearHashTable<Key, Value, Hash>::resize() {
    rehash(resizeCoef);
}


template<typename Key, typename Value, class Hash>
void LinearHashTable<Key, Value, Hash>::rehash(const size_t coeff) {
    std::vector<Node*> oldTable = table;
    size_t oldSize = table.size();
    table.clear();
    table.resize(oldSize * coeff, nullptr);
    curSize = 0; 
    for (auto elem : oldTable)
        if (elem != nullptr && elem != Tombstone) {
            add(elem->key, elem->val);
            delete elem;
        }   
}


