/* Find an actor's Bacon number by breadth first search:
 * 	If two actors act in the same movie, there is a distance of 1 between them.
 */
#include <stdio.h>
#include "mongo.h"
#include "cbacon.h"

/* coll has the form "[db].[collection]"
 * field is the document field to search (eg. "_id", "age", "name", "movies")
 * val is the field value to match
 */ 
static void get_movies( mongo *conn, char *coll, char *id_field,  char *id_val) {
  bson query[1];
  mongo_cursor cursor[1];

  bson_init( query );
  bson_append_string( query, id_field, id_val );
  bson_finish( query );

  mongo_cursor_init( cursor, conn, coll );
  mongo_cursor_set_query( cursor, query );
  
  while( mongo_cursor_next( cursor ) == MONGO_OK ) {
    bson b[1];
    bson_iterator iterator[1], sub[1];
    if ( bson_find( iterator, mongo_cursor_bson( cursor ), "movies" )) {
      bson_iterator_subobject_init( iterator, b, 0 );
      printf("%i\n", bson_size( b ));
      bson_iterator_subiterator( iterator, sub );
      while (bson_iterator_next( sub ) != BSON_EOO) {
	printf("%s\n",  bson_iterator_string(sub) );
      }
    }
  }

  bson_destroy( query );
  mongo_cursor_destroy( cursor );
}

// Get actor(_id or name?): find_one, return bson 
// Get movies: find_one -> subiterator

int main() {
  mongo conn[1];
  mconnect(conn);
  
  // Find Bacon
  get_movies( conn, "dumbacon.actors", "_id", "Kevin Bacon" );
  
  mongo_destroy( conn );

  return 0;
}

/* A breadth-first search from Bacon guarantees the lowest Bacon number for all actors.
 * The lowest Bacon number B = 0 for Bacon himself. 
 * 1, the minimal Bacon number, is reserved for actors who worked with Bacon directly:
 * these can be found by expanding the actors list for each movie and assigning them
 * B' = B+1.
 * For each actor, we expand the actors list for each movie they participate. If an actor
 * has a non-Null B, leave it (it should be equal to or +-1 from the current actor).
 * If an actor has a Null B, B' = B+1.*
 * As long as we expand all actors at one level before starting on the next, we can be
 * sure that our numbers will be consistent.
 * *Index by B = Null? Or have a bacon_Numbers collection?
 */