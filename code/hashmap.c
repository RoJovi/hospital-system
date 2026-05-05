#include "utils.h"
#include <string.h>

// 哈希函数
unsigned int hash(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31 + (*key)) % HASH_SIZE;
        key++;
    }
    return hash;
}

// 初始化哈希表
void initHashMap(HashMap *map) {
    for (int i = 0; i < HASH_SIZE; i++) {
        map->buckets[i] = NULL;
    }
}

// 插入键值对
void hashMapInsert(HashMap *map, const char *key, void *data) {
    unsigned int index = hash(key);
    
    // 检查是否已存在
    HashEntry *curr = map->buckets[index];
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            curr->data = data;  // 更新数据
            return;
        }
        curr = curr->next;
    }
    
    // 创建新节点
    HashEntry *newEntry = (HashEntry*)malloc(sizeof(HashEntry));
    strcpy(newEntry->key, key);
    newEntry->data = data;
    newEntry->next = map->buckets[index];
    map->buckets[index] = newEntry;
}

// 查找键对应的值
void* hashMapGet(HashMap *map, const char *key) {
    unsigned int index = hash(key);
    
    HashEntry *curr = map->buckets[index];
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            return curr->data;
        }
        curr = curr->next;
    }
    return NULL;
}

// 删除键值对
void hashMapRemove(HashMap *map, const char *key) {
    unsigned int index = hash(key);
    
    HashEntry *curr = map->buckets[index];
    HashEntry *prev = NULL;
    
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            if (prev) {
                prev->next = curr->next;
            } else {
                map->buckets[index] = curr->next;
            }
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

// 清空哈希表（不释放data指向的数据）
void hashMapClear(HashMap *map) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry *curr = map->buckets[i];
        while (curr) {
            HashEntry *temp = curr;
            curr = curr->next;
            free(temp);
        }
        map->buckets[i] = NULL;
    }
}
