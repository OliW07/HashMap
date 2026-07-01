#include <gtest/gtest.h>
#include <stdexcept>
#include "../hashtable.h"

class HashTableTest : public testing::Test {
    protected:
        HashTableTest(){
            hashTable.clear();
        }

        HashTable<std::string, int> hashTable;
};

TEST_F(HashTableTest, IsEmptyInitially){
    EXPECT_EQ(hashTable.size(), 0);
}

TEST_F(HashTableTest, InsertCheck){
    hashTable.insert("Apples", 10);
    EXPECT_EQ(hashTable.at("Apples"), 10);
    EXPECT_EQ(hashTable.contains("Apples"), true);
}

TEST_F(HashTableTest, DuplicateKeyThrow){
    hashTable.insert("Orange", 5);
    EXPECT_THROW(hashTable.insert("Orange", 10), std::runtime_error);
}

TEST_F(HashTableTest, ClearTable){
    hashTable.insert("Banana", 69);
    hashTable.insert("Apple", 21);
    hashTable.clear();
    EXPECT_EQ(hashTable.contains("Apple"), false);
    EXPECT_EQ(hashTable.contains("Banana"), false);
    EXPECT_EQ(hashTable.size(), 0);
}

TEST_F(HashTableTest, Resize){
    size_t resizeThresholdCapacity = static_cast<size_t>(hashTable.initialCapacity() * HashTable<std::string, int>::LOAD_FACTOR_THRESHOLD);
    for(size_t i = 0; i < resizeThresholdCapacity; ++i){
        hashTable.insert(std::to_string(i), i);
    }

    size_t capacityBeforeResize = hashTable.capacity();
    hashTable.insert("overflow", 1);
    size_t expectedNewCapacity = capacityBeforeResize * HashTable<std::string,int>::RESIZE_FACTOR;
    EXPECT_EQ(hashTable.capacity(), expectedNewCapacity);

}

TEST_F(HashTableTest, SquareBrackets){
    hashTable["Apples"] = 10;
    ASSERT_EQ(hashTable.at("Apples"), 10);
    ASSERT_EQ(hashTable["Apples"], 10);

    hashTable["Apples"]++;
    ASSERT_EQ(hashTable.at("Apples"), 11);
    ASSERT_EQ(hashTable["Apples"], 11);
}


int main(int argc, char ** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
