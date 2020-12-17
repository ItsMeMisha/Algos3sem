template<typename T>
class IHashSet {
public:
    void add(T key);
    void remove(T key);
    bool find(T key);
};
