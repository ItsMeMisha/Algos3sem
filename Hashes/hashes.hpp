#include <string> //std::string
#include <functional> //std::hash
#include <cstddef> //size_t

//template <typename Key>
//size_t hashName::operator(const Key& key) const;

namespace hashes {

//==============================================================================
//          DECLARATIONS
//==============================================================================

    template <typename Key>
    class Hash {
    public: 
        virtual size_t operator() (const Key& key) const;
    };

//MD5 Block begins
    template <typename Key>
    class MD5 : public Hash<Key> {};

    template <>
    class MD5<int> : public Hash<int> {
    public:
        size_t operator() (const int& key) const override;
    };

    template <>
    class MD5<std::string> : public Hash<std::string> {
    public:
        size_t operator() (const std::string& key) const override;
    };
//MD5 Block ends

//SHA256 Block begins
    template <typename Key>
    class SHA256 : public Hash<Key> {};

    template <>
    class SHA256<int> : public Hash<int> {
    public:
        size_t operator() (const int& key) const override;
    };

    template <>
    class SHA256<std::string> : public Hash<std::string> {
    public:
        size_t operator() (const std::string& key) const override;
    };
//SHA256 Block ends

//DNA Block begins
    //Hash from task 1.2
    template <typename Key>
    class DNA : public Hash<Key> {};

    template <>
    class DNA<std::string> : public Hash<std::string> {
    public:
        size_t operator() (const std::string& key) const override;
    };
//DNA Block ends

//Rolling Block begins
    //From Rabin-Karp algorithm
    template <typename Key>
    class RollingHash : public Hash<Key> {};

    template <>
    class RollingHash<std::string> : public Hash<std::string> {
    public:
        size_t operator() (const std::string& key) const override;
    };
//Rolling Block ends

//Murmur3 Block begins
    template <typename Key>
    class Murmur3 : public Hash<Key> {};

    template <>
    class Murmur3<int> : public Hash<int> {
    public:
        size_t operator() (const int& key) const override;
    };

    template <>
    class Murmur3<std::string> : public Hash<std::string> {
    public:
        size_t operator() (const std::string& key) const override;
    };
//Murmur3 Block ends

//Tabulation Block begins
    template <typename Key>
    class Tabulation : public Hash<Key> {};

    template <>
    class Tabulation<int> : public Hash<int> {
    public:
        size_t operator() (const int& key) const override;
    };
//Tabulation Block ends

//Simple Block begins
    template <typename Key>
    class Simple : public Hash<Key> {};

    template <>
    class Simple<int> : public Hash<int> {
    public:
        size_t operator() (const int& key) const override;
    };
//Simple Block ends

//==============================================================================
//          DEFINITIONS
//==============================================================================

    template <typename Key>
    size_t Hash<Key>::operator() (const Key& key) const {
        return sizeof(key);
    }
}
