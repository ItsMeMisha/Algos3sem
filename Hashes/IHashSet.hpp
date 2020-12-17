template<typename T>
class IHashSet {
public:
    void add(const T& key) = 0;
    void remove(const T& key) = 0;
    bool find(const T& key) = 0;
};
