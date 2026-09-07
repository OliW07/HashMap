#include <gtest/gtest.h>
#include <stdexcept>
#include "hashmap/hashmap.h"

class HashMapTest : public testing::Test {
    protected:
        HashMapTest(){
            hashMap.clear();
        }

        HashMap<std::string, int> hashMap;
};

TEST_F(HashMapTest, IsEmptyInitially){
    EXPECT_EQ(hashMap.size(), 0);
}

TEST_F(HashMapTest, InsertCheck){
    hashMap.insert("Apples", 10);
    EXPECT_EQ(hashMap.at("Apples"), 10);
    EXPECT_EQ(hashMap.contains("Apples"), true);
}

TEST_F(HashMapTest, DuplicateKeyThrow){
    hashMap.insert("Orange", 5);
    EXPECT_THROW(hashMap.insert("Orange", 10), std::runtime_error);
}

TEST_F(HashMapTest, ClearTable){
    hashMap.insert("Banana", 69);
    hashMap.insert("Apple", 21);
    hashMap.clear();
    EXPECT_EQ(hashMap.contains("Apple"), false);
    EXPECT_EQ(hashMap.contains("Banana"), false);
    EXPECT_EQ(hashMap.size(), 0);
}

TEST_F(HashMapTest, Resize){
    size_t resizeThresholdCapacity = static_cast<size_t>(hashMap.initialCapacity() * HashMap<std::string, int>::LOAD_FACTOR_THRESHOLD);
    for(size_t i = 0uz; i < resizeThresholdCapacity; ++i){
        hashMap.insert(std::to_string(i), i);
    }

    size_t capacityBeforeResize = hashMap.capacity();
    hashMap.insert("overflow", 1);
    size_t expectedNewCapacity = capacityBeforeResize * HashMap<std::string,int>::RESIZE_FACTOR;
    EXPECT_EQ(hashMap.capacity(), expectedNewCapacity);

}

TEST_F(HashMapTest, SquareBrackets){

    hashMap["Apples"] = 10;
    ASSERT_EQ(hashMap.at("Apples"), 10);
    ASSERT_EQ(hashMap["Apples"], 10);

    hashMap["Apples"]++;
    ASSERT_EQ(hashMap.at("Apples"), 11);
    ASSERT_EQ(hashMap["Apples"], 11);

}
/*
TEST_F(HashMapTest, Erase){
    hashMap["Apples"] = 42;
    ASSERT_EQ(hashMap.contains("Apples"), true);
    hashMap.erase("Apples");
    ASSERT_EQ(hashMap.contains("Apples"), false);
}

*/

TEST_F(HashMapTest, Collision){

    auto CursedHash = [](const std::string& s) -> size_t {
        return 0zu;
    q};
    
    HashMap<std::string, int, decltype(CursedHash)> cursedHashMap(8);

    cursedHashMap.insert("a", 1);
    cursedHashMap.insert("b", 2);
    cursedHashMap.insert("c", 3);
    cursedHashMap.insert("d", 4);
    cursedHashMap.insert("e", 5);

    // All entries must be found despite same hash
    
    EXPECT_EQ(cursedHashMap.at("a"), 1);
    EXPECT_EQ(cursedHashMap.at("b"), 2);
    EXPECT_EQ(cursedHashMap.at("c"), 3);
    EXPECT_EQ(cursedHashMap.at("d"), 4);
    EXPECT_EQ(cursedHashMap.at("e"), 5);
    
    EXPECT_TRUE(cursedHashMap.contains("a"));
    EXPECT_TRUE(cursedHashMap.contains("b"));
    EXPECT_TRUE(cursedHashMap.contains("c"));
    EXPECT_TRUE(cursedHashMap.contains("d"));
    EXPECT_TRUE(cursedHashMap.contains("e"));
    
    // Should still throw error for duplicate key even if hidden at a different address

    EXPECT_THROW(cursedHashMap.insert("b", 21), std::runtime_error);

    EXPECT_EQ(cursedHashMap.size(), 5);

}



int main(int argc, char ** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
