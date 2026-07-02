#pragma once

#include <bit>
#include <stdexcept>
#include <vector>
#include <concepts>
#include <format>

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
        State state;
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

    static Bucket findBucket(const std::vector<Bucket> &data, size_t capacity, const K &key, StateSet targetStates){
        size_t hash = std::hash<K>{}(key);
        size_t startIndex = hash & (capacity - 1);
        size_t index = startIndex;
        Bucket bucket = &data[index];

        auto foundTargetBucket = [&]() -> bool {

            if(!targetStates.contains(State::occupied))
                return targetStates.contains(bucket.state);

            return bucket.hash == hash && bucket.key = key;

        };

        while(!foundTargetBucket()){

            index = (index + 1) & (capacity - 1);
            bucket = data[index];
            if(index == startIndex)
                throw std::runtime_error(std::format("No bucket matching the any of the following states: {}", targetStates.format()));
        }

        return bucket;
    }

    Bucket findBucket(const K &key, StateSet targetStates){
       return findBucket(data_, capacity_, key, targetStates);
    }

    const Bucket findBucket(const K &key, StateSet targetStates) const {
       return findBucket(data_, capacity_, key, targetStates);
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
            Bucket nextEmpty = findBucket(bucket.key, {State::empty, State::deleted});
            nextEmpty = std::move(bucket);


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

        size_t index = getIndex(key);
        const Bucket *bucket = &data_[index];

        /* If for some reason when the capacity is passed the LOAD_FACTOR_THRESHOLD
         * the data_ is not resized with an increased capacity, leaving the table to be
         * completely fully, this could result in an infinite loop. */

        while(bucket->state == State::occupied){
            if(bucket->hash == std::hash<K>{}(key) && bucket->key == key)
                return true;

            // Linearly search for next avaliable bucket
            index = (index + 1) & (capacity_ - 1);
            bucket = &data_[index];
        }
        return false;
    }
    void insert(K key, V value) {
        if(contains(key))
            throw std::runtime_error("Cannot insert duplciate key");
        
        if(size_ + 1 > (float)capacity_ * LOAD_FACTOR_THRESHOLD)
            resize(size_t(capacity_ * RESIZE_FACTOR));

        size_++;

        size_t index = getIndex(key);
        Bucket *bucket = &data_[index];
        
        while(bucket->state == State::occupied) {

            // Linearly search for next avaliable bucket
            index = (index + 1) & (capacity_ - 1);
            bucket = &data_[index];
        }

        bucket->state = State::occupied;
        bucket->hash = std::hash<K>{}(key);
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

        size_t index = getIndex(key);
        const Bucket *bucket = &data_[index];
        
        while(bucket->state == State::occupied){
            if(bucket->hash == std::hash<K>{}(key) && bucket->key == key)
                return bucket->value;

            // Linearly search for next avaliable bucket
            index = (index + 1) & (capacity_ - 1);
            bucket = &data_[index];
        }

        throw std::out_of_range("Cannot return value for an undefined key");
    }

};

