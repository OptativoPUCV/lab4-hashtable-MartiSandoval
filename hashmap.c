#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  Pair *par = createPair(key, value);
  long pos = hash(key, map->capacity);

  if (map->buckets[pos] == NULL) {
    map->buckets[pos] = par;
    map->size++;
  }
  else {
    while (map->buckets[pos] != NULL) {
      if (is_equal(map->buckets[pos]->key, par->key)) {
        map->buckets[pos] = par;
        return;
      }
      pos = (pos + 1) % map->capacity;  
    }
    map->buckets[pos] = par;
    map->size++;
  }
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair *aux;
  aux = (Pair *) malloc(sizeof(Pair) * map->capacity);
  aux = map->buckets;
  long nuevaC = map->capacity * 2;

  Pair *nuevo = (Pair *) malloc(sizeof(Pair) * nuevaC);

  for (long i = 0; i < nuevaC; i++) {
    nuevo[i] = NULL;
  }
  map->buckets = nuevo;
  map->size = 0;
  for(long i = 0; i < nuevaC; i++)
  {
    if(aux[i] != NULL)
      insertMap(map, aux[i].key, aux[i].value);
  }
  map->capacity = nuevaC;
  free(aux);
}


HashMap * createMap(long capacity) {
  HashMap *map = (HashMap *) malloc(sizeof(HashMap));
  map->buckets = (Pair **) malloc(sizeof(Pair *) * capacity);
  map->current = -1;
  map->capacity = capacity;
  map->size = 0;
  
  return map;
}

void eraseMap(HashMap * map,  char * key) {
  Pair* aux = searchMap(map, key);
  if (aux != NULL) {
    aux->key = NULL;
    map->size--;
  }
}

Pair * searchMap(HashMap * map,  char * key) {   
  long pos = hash(key, map->capacity);
  
  if (map->buckets[pos] != NULL && is_equal(map->buckets[pos]->key, key)) {
    map->current = pos;
    return map->buckets[pos];
  }
  else {
    while (map->buckets[pos] != NULL) {
      if (is_equal(map->buckets[pos]->key, key)) {
        map->current = pos;
        return map->buckets[pos];
      }
      pos = (pos + 1) % map->capacity;
    }
  }
  return NULL;
}

Pair * firstMap(HashMap * map) {
  if (map->size == 0) return NULL;
  
  long i = 0;
  while (map->buckets[i] == NULL || map->buckets[i]->key == NULL) {
    i = (i + 1) % map->capacity;
    if (i == 0) {
      return NULL;
    } 
  }
  map->current = i;
  return map->buckets[i];
 }

Pair * nextMap(HashMap * map) {
  if (map->size == 0 || map->current == -1 || map->capacity == 0) 
    return NULL;  
  
  long aux = map->current + 1;
  while (map->buckets[aux] == NULL || map->buckets[aux]->key == NULL) {
    if (aux == map->capacity) {
      return NULL;
    }
    aux++;
  }
  map->current = aux;
  return map->buckets[map->current];
}
