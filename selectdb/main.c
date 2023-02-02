#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>


typedef int (*sqlite3_callback)(
   void*,    /* données fournies */
   int,      /* le nombre de colonne dans la ligne */
   char**,   /* un tableau de chaînes représentant les champs de la ligne */
   char**    /* un tableau de chaînes représentant les noms de colonnes */
);

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);

   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }

   printf("\n");
   return 0;
}

//int argc, char* argv[]
int main() {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "fonction callback appelée : données des stations";

   /* ouvrir base de données */
   rc = sqlite3_open("/home/ajc/projetfinal/velibdata.db", &db);

   if( rc ) {
      fprintf(stderr, "base de données ne peut s'ouvrir: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "base de données ouvert avec succès\n");
   }

   /* Create SQL statement */
   //sql = "SELECT * from station_status WHERE station_id = 15202";
   sql = "SELECT * from station_status INNER JOIN station_information ON station_status.station_id = station_information.station_id";


   /* exécuter une instruction sql */
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

   if( rc != SQLITE_OK ) {
      fprintf(stderr, "erreur sql: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "instruction exécutée\n");
   }
   sqlite3_close(db);
   return 0;
}
