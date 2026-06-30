#include <gtest/gtest.h>
#include "../hashtable.h"


template <typename K, typename V>
requires Hashable<K>

class HashTableTest : public testing::Test {
    protected:
        HashTableTest(){
            hashTable.clear();
        }

        HashTable<K,V> hashTable;
};


int main(int argc, char ** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
