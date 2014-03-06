/* Header file for highly lucrative "Project Bacon"
 * 
 */
// Connect to a mongo database
#include <stdio.h>
#include "mongo.h"

#define MDB_IP "127.0.0.1"
#define MDB_PORT 27017

void mconnect(mongo *conn) {
  int status = mongo_client( conn, MDB_IP, MDB_PORT );

  if( status != MONGO_OK ) {
      switch ( conn->err ) {
        case MONGO_CONN_NO_SOCKET:  printf( "no socket\n" ); exit(1);
        case MONGO_CONN_FAIL:       printf( "connection failed\n" ); exit(1);
        case MONGO_CONN_NOT_MASTER: printf( "not master\n" ); exit(1);
      }
  }
  
}