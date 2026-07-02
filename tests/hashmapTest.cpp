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
    for(size_t i = 0; i < resizeThresholdCapacity; ++i){
        hashMap.insert(std::to_string(i), i);
    }

    size_t capacityBeforeResize = hashMap.capacity();
    hashMap.insert("overflow", 1);
    size_t expectedNewCapacity = capacityBeforeResize * HashMap<std::string,int>::RESIZE_FACTOR;
    EXPECT_EQ(hashMap.capacity(), expectedNewCapacity);

}

TEST_F(HashMapTest, SquareBrackets){
    /*
    hashMap["Apples"] = 10;
    ASSERT_EQ(hashMap.at("Apples"), 10);
    ASSERT_EQ(hashMap["Apples"], 10);

    hashMap["Apples"]++;
    ASSERT_EQ(hashMap.at("Apples"), 11);
    ASSERT_EQ(hashMap["Apples"], 11);
    */

}

int main(int argc, char ** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
