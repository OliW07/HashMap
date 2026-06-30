#include <string>

#include "hashtable.h"

int main() {
    HashTable<std::string, size_t> hashTable;
    hashTable.insert(std::string("apples"), 5);
}
