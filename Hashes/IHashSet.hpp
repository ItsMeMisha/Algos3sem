template<typename T>
class IHashSet {
public:
    virtual void add(const T& key) = 0;
    virtual void remove(const T& key) = 0;
    virtual bool find(const T& key) = 0;
};
