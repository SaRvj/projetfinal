#include "gd.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdio.h>

int callback(void *NotUsed, int argc, char **argv,char **azColName) {
    NotUsed = 0;
    /*for (int i = 0; i < argc; i++) {
    }*/
    printf("\n");
    return 0;
}

int main() {
    sqlite3 *db;
    char *err_msg = 0;
        int retenirvalue = sqlite3_open("/home/ajc/projetfinal/velibdata.db", &db);
        if (retenirvalue != SQLITE_OK) {
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return 1;
        }
        char *sql = "SELECT * FROM (SELECT * FROM station_status INNER JOIN station_information ON station_status.station_id = station_information.station_id) ORDER BY last_reported DESC LIMIT 15";
        retenirvalue = sqlite3_exec(db, sql, callback, 0, &err_msg);
        if (retenirvalue != SQLITE_OK ) {
            fprintf(stderr, "Failed to select data\n");
            fprintf(stderr, "SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_close(db);
            return 1;
        }
        //créer une variable de type "int" pour stocker le code de retour pour chaque appel : retenirvalue
        char   *query;
        //préparer l'accès aux tables
        sqlite3_stmt *stmt;
        //lire le contenu de la table
        query = "SELECT * FROM (SELECT * FROM station_status LEFT JOIN station_information ON station_status.station_id = station_information.station_id) WHERE station_id = 13007 ORDER BY last_reported DESC LIMIT 1500";
        retenirvalue = sqlite3_prepare_v2(db,query,-1,&stmt,0);
        if(retenirvalue)
        {
            printf("Impossible de lire la table\n");
            return   -1;
        }
        //lire le nombre de colonnes récupérées
        int cols = sqlite3_column_count(stmt);
        printf("%d \n", cols);
        //écrire de l'entête des colonnes
        int col;
        for(col=0 ; col<cols;col++) 
           printf("%s| ",sqlite3_column_name(stmt,col));
           printf("\n");
        //écrire   des données
        //initialiser nombre de lignes = Nbr_lignes
        int Nbr_lignes = 0;
        while(1)
        {
            //récupérer le status de la ligne (contient de données ou fin de table)
            retenirvalue = sqlite3_step(stmt);
            if(retenirvalue == SQLITE_ROW)
            {
                //la ligne contient des données
                int   col;
                for(col=0 ; col<cols;col++)
                {
                    //sqlite3_column_text retourne un const void* => cast en const char*
                    const char *val =   (const char*)sqlite3_column_text(stmt,col);
                    printf("%s| ",val);
                }
                printf("\n");
                Nbr_lignes = Nbr_lignes +1;
             }
             else if(retenirvalue == SQLITE_DONE)
             {
                 //plus de données
                 printf("Fin de la table\n");
                 break;
             }
             else
             {
                 //erreur
                 printf("Erreur lors de l'accès aux données\n");
                 return -1;
             }
        }
        printf("Nombre de lignes de la base velib.bd : %d \n",Nbr_lignes);
        //imprimer les valeurs de la table
        int numDocsAvailable[Nbr_lignes];
        int num_bikes_available[Nbr_lignes];
        int num_bikes_available_ebike[Nbr_lignes];
        char last_reported[Nbr_lignes];
        int num_ligne = 0; //numero de ligne
        //écrire les données
        while(1)
        {
            //récupérer le status de la ligne (contient de données ou fin de table)
            retenirvalue = sqlite3_step(stmt);
            if(retenirvalue == SQLITE_ROW)
            {
             //la ligne contient des données
             //sqlite3_column_double : renvoie un double pas un pointeur vers un double
             numDocsAvailable[num_ligne] = sqlite3_column_double(stmt,5);
             num_bikes_available[num_ligne] = sqlite3_column_double(stmt,6);
             last_reported[num_ligne] = sqlite3_column_double(stmt,9);
             //printf("numero de ligne   : %d  \t\t |", num_ligne);
             printf("numDocsAvailable :%d  \t\t | ", numDocsAvailable[num_ligne]);
             printf("num_bikes_available :%d ", num_bikes_available[num_ligne]);
             printf("last_reported :%d ", last_reported[num_ligne]);
             printf("\n");
             //printf("num_ligne  : %d \n",num_ligne);
             num_ligne = num_ligne + 1;
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
        //fermeture de db pour libérer de la mémoire
        sqlite3_close(db);

//printf("numDocsAvailable :%d  \t\t | ", numDocsAvailable[0]);
//printf("num_bikes_available :%d ", num_bikes_available[0]);
//printf("last_reported :%d  \t\t | \n ", last_reported[0]);

//************************GD************************************
    /* Declare the image */
    gdImagePtr im;
    /* Declare output files */
    FILE *pngout, *jpegout;
    /* Declare color indexes */
    int bleu;
    int navy;
    int red;
    int hauteur = 250;
    /* Allocate the image: 64 pixels across by 64 pixels tall */
    //im = gdImageCreate(64, 64);
    im = gdImageCreate(Nbr_lignes, hauteur);
    /* Allocate the color black (red, green and blue all minimum). 
       Since this is the first color in a new image, it will be the background color. */
    bleu = gdImageColorAllocate(im, 153, 204, 255);
    /* Allocate the color white (red, green and blue all maximum). */
    navy = gdImageColorAllocate(im, 0, 0, 102);
    red = gdImageColorAllocate(im, 255, 0, 0);
    /* Draw a line from the upper left to the lower right,
    using white color index. */
    //gdImageLine(im, 0, 0, 63, 63, white);
    int i = 0;
    for(i = 0; i<Nbr_lignes; i++){
        gdImageLine(im, i, (numDocsAvailable[i]+(hauteur/2)), (i+1), (numDocsAvailable[i+1]+(hauteur/2)), navy);
        gdImageLine(im, i, (num_bikes_available[i]+(hauteur/2)), (i+1), (num_bikes_available[i+1]+(hauteur/2)), red); 
        //gdImageLine(im, numDocsAvailable[0], last_reported[0], numDocsAvailable[1], numDocsAvailable[1], navy);
        //gdImageLine(im, num_bikes_available[0], last_reported[0], num_bikes_available[1], last_reported[1], navy);
    }
    /* Open a file for writing. "wb" means "write binary", important under MSDOS, harmless under Unix. */
    pngout = fopen("velib.png", "wb");
    /* Do the same for a JPEG-format file. */
    jpegout = fopen("velib.jpg", "wb");
    /* Output the image to the disk file in PNG format. */
    gdImagePng(im, pngout);
    /* Output the same image in JPEG format, using the default JPEG quality setting. */
    gdImageJpeg(im, jpegout, -1);
    /* Close the files. */
    fclose(pngout);
    fclose(jpegout);
    /* Destroy the image in memory. */
    gdImageDestroy(im);
}

