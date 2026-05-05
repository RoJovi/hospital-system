#ifndef HASHMAP_H
#define HASHMAP_H

#include "utils.h"

unsigned int hash(const char *key);
void initHashMap(HashMap *map);
void hashMapInsert(HashMap *map, const char *key, void *data);
void* hashMapGet(HashMap *map, const char *key);
void hashMapRemove(HashMap *map, const char *key);
void hashMapClear(HashMap *map);

#endif // HASHMAP_H
