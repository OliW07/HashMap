#pragma once

#include <bit>
#include <stdexcept>
#include <vector>
#include <concepts>

#include "detail/states.h"

template <typename T> 
concept Hashable = requires(T a){
    { std::hash<T>{}(a) } -> std::convertible_to<size_t>;
};

template <typename K, typename V>
requires Hashable<K>

class HashMap {
    friend class HashMapTest_Resize_Test;

private:
    
    struct Bucket {
        size_t hash;
        K key;
        V value;
        State state = State::empty;
    };

    static constexpr float LOAD_FACTOR_THRESHOLD = 0.7f;
    static constexpr float RESIZE_FACTOR = 2.0;
    static constexpr size_t DEFAULT_CAPACITY = 1 << 4;
    static_assert(std::has_single_bit(DEFAULT_CAPACITY), "DEFAULT_CAPACITY must be a power of two");
    const size_t INITIAL_CAPACITY_;

	size_t capacity_ = DEFAULT_CAPACITY;
    size_t size_ = 0;
	std::vector<Bucket> data_;

    size_t getIndex(K key) const {
        size_t hash = std::hash<K>{}(key);
        return hash & (capacity_ - 1);
    }
    size_t getIndex(size_t hash) const {
        return hash & (capacity_ - 1);
    }

    const Bucket* findEntry(size_t hash, const K& key) const {
        size_t index = getIndex(hash);
        while (data_[index].state != State::empty) {
            if (data_[index].state == State::occupied &&
                    data_[index].hash == hash &&
                    data_[index].key == key)
                return &data_[index];
            index = (index + 1) & (capacity_ - 1);
        }
        return nullptr;
    }

    static Bucket* findSlot(Bucket* data, size_t capacity, size_t hash) {
        size_t index = hash & (capacity - 1);
        while (data[index].state == State::occupied)
            index = (index + 1) & (capacity - 1);
        return &data[index];
    }

    void resize(size_t capacity){
        if(capacity <= capacity_){
            throw std::logic_error("Must resize to a capacity larger than current capacity");
        }

        capacity_ = capacity;
        
        std::vector<Bucket> newData;
        newData.resize(capacity_);

        for(Bucket &bucket : data_){

            if(bucket.state != State::occupied) continue;

            bucket.hash = std::hash<K>{}(bucket.key);
            Bucket* slot = findSlot(newData.data(), capacity_, bucket.hash);
            *slot = std::move(bucket);

        }

        data_ = std::move(newData);
    }

public:
    HashMap(size_t capacity = DEFAULT_CAPACITY) : INITIAL_CAPACITY_(capacity), capacity_(capacity){
        if(!std::has_single_bit(capacity))
            throw std::runtime_error("Capacity must be a power of two");
        
        data_.resize(capacity_);
    }

    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    size_t initialCapacity() const { return INITIAL_CAPACITY_; }

    bool contains(K key) const {
        size_t hash = std::hash<K>{}(key);
        return findEntry(hash, key) != nullptr; 
    }
    void insert(K key, V value) {
        size_t hash = std::hash<K>{}(key);
        if(findEntry(hash, key))
            throw std::runtime_error("Cannot insert duplciate key");
        
        if(size_ + 1 > (float)capacity_ * LOAD_FACTOR_THRESHOLD)
            resize(size_t(capacity_ * RESIZE_FACTOR));
        size_++;

        Bucket *bucket = findSlot(data_.data(), capacity_, hash);

        bucket->state = State::occupied;
        bucket->hash = hash;
        bucket->key = key;
        bucket->value = value;
        
    }

    void clear() {
        for(Bucket &bucket : data_)
            bucket.state = State::empty;
        
        data_.resize(INITIAL_CAPACITY_);
        data_.shrink_to_fit();
        capacity_ = INITIAL_CAPACITY_;
        size_ = 0;
    }
    const V& at(K key) const {
        size_t hash = std::hash<K>{}(key);
        const Bucket* bucket = findEntry(hash, key);
                
        if(!bucket)
            throw std::out_of_range("Cannot return value for an undefined key");

        return bucket->value;
    }

};
