/* Example code for Software Systems at Olin College.

Copyright 2014 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// VALUE: represents a value in a key-value pair

/* Here's one way of making a polymorphic object in C */

typedef struct val{
    enum Type {INT, STRING} type;
    union {
	int i;
	char *s;
    };
    void (*free) (struct val*);
} Value;


void free_int_value(Value *value) {
  free(value);
}


void free_string_value(Value *value)
{
  if (value->s != NULL) {
    free(value->s);
  }
  free(value);
}


void free_value(Value *value) {
  value->free(value);
  value = NULL;
}


/* Makes a Value object that contains an int. */
Value *make_int_value(int i) 
{
    Value *value = (Value *) malloc (sizeof (Value));
    value->type = INT;
    value->i = i;
    value->free = free_int_value;
    return value;
}


/* Makes a Value object that contains a string. */
Value *make_string_value(char *s) 
{
    Value *value = (Value *) malloc (sizeof (Value));
    value->type = STRING;
    value->s = s;
    value->free = free_string_value;
    return value;
}


/* Prints a value object. */
void print_value (Value *value) 
{
    if (value == NULL) {
        printf ("%p", value);
	return;
    }
    switch (value->type) {
    case INT:
	printf ("%d", value->i);
	break;
    case STRING:
	printf ("%s", value->s);
	break;
    }
}


// HASHABLE: Represents a key in a key-value pair.

/* Here's another way to make a polymorphic object.

The key can be any pointer type.  It's stored as a (void *), so
when you extract it, you have to cast it back to whatever it is.

hash is a pointer to a function that knows how to hash the key.
equal is a pointer to a function that knows how to compare keys.

 */

typedef struct Hashable {
    void *key;
    int (*hash) (void *);
    int (*equal) (void *, void *);
} Hashable;


/* Makes a Hashable object. */
Hashable *make_hashable(void *key, 
			int (*hash) (void *),
			int (*equal) (void *, void *)
			)
{
    Hashable *hashable = (Hashable *) malloc (sizeof (Hashable));
    hashable->key = key;
    hashable->hash = hash;
    hashable->equal = equal;
    return hashable;
}

void free_hashable (Hashable *hashable) {
  if (hashable != NULL) {
    free(hashable->key);
    free(hashable);
    hashable = NULL;
  }
}


/* Prints a Hashable object. */
void print_hashable(Hashable *hashable)
{
    printf ("key %p:\t", hashable->key);
    printf ("hash %p\n", hashable->hash);
}


/* Hashes an integer. */
int hash_int(void *p)
{
    return *(int *)p;
}


/* Hashes a string. */
int hash_string(void *p)
{
    char *s = (char *) p;
    int total = 0;
    int i = 0;

    while (s[i] != 0) {
	total += s[i];
	i++;
    }
    return total;
}


/* Hashes any Hashable. */
int hash_hashable(Hashable *hashable)
{
    return hashable->hash (hashable->key);
}


/* Compares integers. */
int equal_int (void *ip, void *jp)
{
    return (*((int *) ip)) == (*((int *) jp));
}


/* Compares strings. */
int equal_string (void *s1, void *s2)
{
     return strcmp((char *) s1,(char *) s2) == 0;
}


/* Compares Hashables. */
int equal_hashable(Hashable *h1, Hashable *h2)
{
  // Make sure we're using the same definition of equality.
  // We could compare them using both methods, but we can't be sure they're the
  // same type.
  if (h1->equal == h2->equal) {
    return h1->equal (h1->key, h2->key);
  }
//   Alternate, more pedantic, implementation:
  // If two hashables don't share the same equal function, odds are good that
  // they're incompatible types. If their hash functions are different, they
  // probably won't have the same distribution of hashes. If their keys
  // are different, they're pretty clearly not equal.
//   if (h1->equal == h2->equal) && (h1->hash == h2->hash) {
//     return h1->equal (h1->key, h2->key);
//   }
  return 0;
}


/* Makes a Hashable int. 

Allocates space and copies the int.
*/
Hashable *make_hashable_int (int x)
{
    int *p = (int *) malloc (sizeof (int));
    *p = x;
    return make_hashable((void *) p, hash_int, equal_int);
}


/* Makes a Hashable string. 

Stores a reference to the string (not a copy).
*/
Hashable *make_hashable_string (char *s)
{
    return make_hashable((void *) s, hash_string, equal_string);
}



// NODE: a node in a list of key-value pairs

typedef struct node {
    Hashable *key;
    Value *value;
    struct node *next;
} Node;


/* Makes a Node. */
Node *make_node(Hashable *key, Value *value, Node *next)
{
    Node *node = (Node *) malloc (sizeof (Node));
    node->key = key;
    node->value = value;
    node->next = next;
    return node;
}

void free_node(Node *node)
{
  if (node != NULL) {
    free_hashable(node->key);
    free_value(node->value);
    free(node);
    node = NULL;
  }
}

void free_list(Node *node)
{
  if (node->next != NULL) {
    free_list(node->next);
  }
  free_node(node);
}


/* Prints a Node. */
void print_node(Node *node)
{
    print_hashable(node->key);
    printf ("value: %p\n", node->value);
    printf ("next: %p\n", node->next);
}


/* Prints all the Nodes in a list. */
void print_list(Node *node)
{
  print_node(node);
  if (node->next != NULL) {
    print_list(node->next);
  }
}


/* Prepends a new key-value pair onto a list.

This is actually a synonym for make_node.
 */
Node *prepend(Hashable *key, Value *value, Node *rest)
{
    return make_node(key, value, rest);
}


/* Looks up a key and returns the corresponding value, or NULL */
Value *list_lookup(Node *list, Hashable *key)
{
    while (list != NULL) {
      if (equal_hashable(list->key,key)) {
	return list->value;
      }
      list = list->next;
    }
    return NULL;
}


// MAP: a map is a list of key-value pairs

typedef struct map {
    int n;
    Node **lists;
    int entries;
    void* index;
} Map;

int get_index(Map* map, Hashable* key) {
  return key->hash (key->key) % (map->n);
}

/* Makes a Map with n lists. */
Map *make_map(int n)
{
    Map *m = (Map *) malloc(sizeof(Map));
    m->n = n;
    m->lists = (Node **) malloc(sizeof(Node *)*n);
    int i;
    for (i = 0; i < n; i++) {
      m->lists[i] = NULL;
    }
    m->entries = 0;
    m->index = get_index;
    return m;
}

void free_map(Map *map)
{
  if (map != NULL) {
    int i;
    for (i=0; i<map->n; i++) {
      if (map->lists[i] != NULL) {
	  free_list(map->lists[i]);
      }
    }
    free(map);
    map = NULL;
  }
}


/* Prints a Map. */
void print_map(Map *map)
{
    int i;
    printf("Map %p\t n = %d\n",map,map->n);
    for (i=0; i<map->n; i++) {
	printf ("\nlist %d\t", i);
	if (map->lists[i] != NULL) {
	    print_list (map->lists[i]);
	} else {
	  printf("Null list: %p\n", NULL);
	}
    }
}


/* Adds a key-value pair to a map. */
void map_add(Map *map, Hashable *key, Value *value)
{
    int index = get_index(map, key);
//     Node *list = map->lists[index];	// node *, may be NULL
    map->lists[index] = prepend(key, value, map->lists[index]);
//     map->entries++;
//     if (map->entries == map->n) {
//       map *new_Map = make_map(2*map->n);
//     }
}


/* Looks up a key and returns the corresponding value, or NULL. */
Value *map_lookup(Map *map, Hashable *key)
{
    int index = get_index(map, key);
    return list_lookup(map->lists[index], key);
}


/* Prints the results of a test lookup. */
void print_lookup(Value *value)
{
    printf ("Lookup returned ");
    print_value (value);
    printf ("\n");
}


int main ()
{
    Hashable *hashable1 = make_hashable_int (1);
    Hashable *hashable2 = make_hashable_string ("Allen");
    Hashable *hashable3 = make_hashable_int (2);

    // make a list by hand
    Value *value1 = make_int_value (17);
    Node *node1 = make_node(hashable1, value1, NULL);
    printf("Node 1: key1 = ");
    print_hashable(hashable1);
    printf("\t value1 = %p",value1);
    printf("\t next = NULL\n print_node returns:\n");
    print_node (node1);
    printf("\n");

    Value *value2 = make_string_value ("Downey");
    Node *list = prepend(hashable2, value2, node1);
    printf("Node 2: key2 = ");
    print_hashable(hashable2);
    printf("\t value2 = %p",value2);
    printf("\t next = Node 1\n print_list returns:\n");
    print_list (list);
    printf("\n");

    // run some test lookups
    Value *value = list_lookup (list, hashable1);
    printf("Looking for (1,17)\n");
    print_lookup(value);
    printf("\n");

    value = list_lookup (list, hashable2);
    printf("Looking for (Allen, Downey)\n");
    print_lookup(value);
    printf("\n");

    value = list_lookup (list, hashable3);
    printf("Looking for (2, NULL)\n");
    print_lookup(value);
    printf("\n");

    // make a map
    Map *map = make_map(10);
    map_add(map, hashable1, value1);
    map_add(map, hashable2, value2);

    print_map(map);

    // run some test lookups
    value = map_lookup(map, hashable1);
    print_lookup(value);
    printf("\n");

    value = map_lookup(map, hashable2);
    print_lookup(value);
    printf("\n");

    value = map_lookup(map, hashable3);
    print_lookup(value);

    return 0;
}
