#include<stdio.h>
#include<sqlite3.h>
#include<stdlib.h>

int main(int argc, char** args)
{
    /*int numDocsAvailable;
    int num_bikes_available;
    int last_reported;*/
    //Créez   une variable de type int pour stocker le code de retour pour chaque appel
    int   retenirvalue;
    char   *query;
    //   Préparer l'accès aux tables
    sqlite3_stmt *stmt;
    // Créer   un handle pour la connexion à base de données
    sqlite3   *handle;
    // Essai   de connexion à la base de données
    retenirvalue =   sqlite3_open("/home/ajc/projetfinal/velibdata.db",&handle);//Si la connexion ne marche pas, handle retourne NULL
    if(retenirvalue)
    {
        printf("Impossible de se connecter à la base de données\n");
        return   -1;
    }
    printf("Connexion réussie\n");
    // Lire le   contenu de la table
    query = "SELECT * FROM (SELECT * FROM station_status INNER JOIN station_information ON station_status.station_id = station_information.station_id) ORDER BY last_reported DESC LIMIT 15";
    retenirvalue =   sqlite3_prepare_v2(handle,query,-1,&stmt,0);
    if(retenirvalue)
    {
        printf("Impossible de lire la table\n");
        return   -1;
    }
    // Lire le   nombre de lignes récupérées
    int cols = sqlite3_column_count(stmt); //nbr de colonnes argc
    // Ecriture de l'entête des colonnes
    int col;
    //for(col=0 ; col<cols;col++) 
        printf("%s \t\t | %s \t\t | %s \t\t",sqlite3_column_name(stmt,5),sqlite3_column_name(stmt,6),sqlite3_column_name(stmt,9)); //chq nom de col
        printf("\n");
    //Ecriture   des données
    while(1) //boucle infinie
    {
        //   récupérer le status de la ligne (contient de données ou fin de table)
        retenirvalue = sqlite3_step(stmt);
        if(retenirvalue == SQLITE_ROW)
        {
            //   La ligne contient des données
            int   col;
            //for(col=0 ; col<cols;col++)
            //{
                // sqlite3_column_text retourne un const   void* => cast en const char*
                const char *val1 =   (const char*)sqlite3_column_text(stmt,5);
                const char *val2 =   (const char*)sqlite3_column_text(stmt,6);
                const char *val3 =   (const char*)sqlite3_column_text(stmt,9);
                printf("%s \t\t | %s \t\t | %s \t\t",val1,val2,val3);
                /*printf("%p \t\t | %p \t\t | %p \t\t",&val1,&val2,&val3);*/
                /*numDocsAvailable = &val1;
                num_bikes_available = &val2;
                last_reported = &val3;*/
                
            printf("\n");
        }
        else if(retenirvalue == SQLITE_DONE)
        {
            //   Plus de données
            printf("Fin de la table\n");
            break;
        }
        else
        {
            //   Erreur
            printf("Erreur lors de l'accès aux données\n");
            return -1;
        }
    }
    //   Fermeture du handle pour libérer la mémoire
    sqlite3_close(handle);
    //printf("%d \t\t | %d \t\t | %d \t\t",numDocsAvailable,num_bikes_available,last_reported);
    return 0;
}
