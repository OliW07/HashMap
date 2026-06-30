#include <stdexcept>
#include <vector>
#include <concepts>

template <typename T> 
concept Hashable = requires(T a){
    { std::hash<T>{}(a) } -> std::convertible_to<size_t>;
};

template <typename K, typename V>
requires Hashable<K>

class HashTable {
private:
    enum class State { empty, occupied, deleted };
    struct Bucket {
        size_t hash;
        K key;
        V value;
        State state;
    };

	size_t capacity_ = 1 << 4;
    size_t size_ = 0;
    const float LOAD_FACTOR_THRESHOLD = 0.7;
	std::vector<Bucket> data_;

    size_t getIndex(K key) {
        size_t hash = std::hash<K>{}(key);
        return hash & (capacity_ - 1);
    }
    size_t getIndex(size_t hash){
        return hash & (capacity_ - 1);
    }

    void resize(size_t capacity){
        if(capacity <= capacity_){
            throw std::logic_error("Must resize to a capacity larger than current capacity");
        }

        capacity_ = capacity;
        
        std::vector<Bucket> newData;
        newData.resize(capacity_);

        for(const Bucket &bucket : data_){

            bucket.hash = std::hash<K>{}(bucket->key);
            newData[getIndex(bucket.hash)] = std::move(bucket);

        }

        data_ = std::move(newData);
    }

public:
	HashTable() { data_.reserve(capacity_); }


    void insert(K key, V value) {
        if(size_ + 1 > capacity_ * LOAD_FACTOR_THRESHOLD){
            resize();
        }
        
    }
    const V& at(K key) const {
        return data_[getIndex(key)];
    }
};

