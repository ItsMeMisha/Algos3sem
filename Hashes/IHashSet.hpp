template<typename T>
class IHashSet {
public:
    void add(T key) = 0;
    void remove(T key) = 0;
    bool find(T key) = 0;
};
