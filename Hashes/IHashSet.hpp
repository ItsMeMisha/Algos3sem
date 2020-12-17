template<typename Key, typename Value>
class IHashSet {
public:
    virtual void add(const Key& key, const Value& val) = 0;
    virtual void remove(const Key& key) = 0;
    virtual Value find(const Key& key, const Value& defVal) = 0;
};
