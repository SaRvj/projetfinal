#include "gd.h"
#include <string.h>
#include <stdio.h>
#include <sqlite3.h>
#include <gdfonts.h>
#include <gdfontl.h>
#include <gdfontg.h>
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
        int retenirvalue = sqlite3_open("/home/ajc/projetfinal/velibdatacopie.db", &db);
        if (retenirvalue != SQLITE_OK) {
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return 1;
        }
        char *sql = "SELECT name, capacity, is_renting, is_returning,numDocsAvailable,num_bikes_available,num_bikes_available_ebike, num_bikes_available_mechanical, last_reported FROM (SELECT * FROM station_status LEFT JOIN station_information ON station_status.station_id = station_information.station_id) WHERE station_id = 13007 GROUP BY last_reported"; //LIMIT 1500

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

        query = "SELECT name, capacity, is_renting, is_returning,numDocsAvailable,num_bikes_available,num_bikes_available_ebike, num_bikes_available_mechanical, last_reported FROM (SELECT * FROM station_status LEFT JOIN station_information ON station_status.station_id = station_information.station_id) WHERE station_id = 13007 GROUP BY last_reported"; //LIMIT 1500
        //query = "SELECT name, capacity, is_renting, is_returning,numDocsAvailable,num_bikes_available,num_bikes_available_ebike, num_bikes_available_mechanical, last_reported FROM station_status INNER JOIN station_information ON station_status.station_id = station_information.station_id WHERE station_status.station_id = 5110";
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
        char last_reported[200][Nbr_lignes];
        char name[200][Nbr_lignes];
        int capacity[Nbr_lignes];
        char is_renting[200][Nbr_lignes];
        char is_returning[200][Nbr_lignes];
        int num_bikes_available_mechanical[Nbr_lignes];
        int num_lignes = 0; //numero de ligne
        //écrire les données
        while(1)
        {
            //récupérer le status de la ligne (contient de données ou fin de table)
            retenirvalue = sqlite3_step(stmt);
            if(retenirvalue == SQLITE_ROW)
            {
             //la ligne contient des données
             //sqlite3_column_double : renvoie un double pas un pointeur vers un double
             numDocsAvailable[num_lignes] = sqlite3_column_double(stmt,4);
             printf("numDocsAvailable :%d  \t\t | ", numDocsAvailable[num_lignes]);

             num_bikes_available[num_lignes] = sqlite3_column_double(stmt,5);
             printf("num_bikes_available :%d ", num_bikes_available[num_lignes]);

             strcpy(last_reported[num_lignes],sqlite3_column_text(stmt,8));
             printf("last_reported :%s ", last_reported[num_lignes]);

             num_bikes_available_ebike[num_lignes] = sqlite3_column_double(stmt,6);
             printf("num_bikes_available_ebike :%d ", num_bikes_available_ebike[num_lignes]);

             strcpy(name[num_lignes],sqlite3_column_text(stmt,0));
             printf("name :%s ", name[num_lignes]);
             printf("\n %ld \n",strlen(name[num_lignes]));
             //printf("%ld \n",strlen(sqlite3_column_text(stmt,0)));

             capacity[num_lignes] = sqlite3_column_double(stmt,1);
             printf("capacity :%d ", capacity[num_lignes]);

             strcpy(is_renting[num_lignes],sqlite3_column_text(stmt,2));
             printf("is_renting :%s ", is_renting[num_lignes]);

             strcpy(is_returning[num_lignes],sqlite3_column_text(stmt,3));
             printf("is_returning :%s ", is_returning[num_lignes]);

             num_bikes_available_mechanical[num_lignes] = sqlite3_column_double(stmt,7);
             printf("num_bikes_available_mechanical :%d ", num_bikes_available_mechanical[num_lignes]);
             //printf("numero de ligne   : %d  \t\t |", num_lignes);

             printf("\n");
             printf("num_lignes  : %d \n",num_lignes);
             num_lignes += 1;
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

//************************GD************************************
    //image1: num docs pour station 13007
    /* Declare the image */
    gdImagePtr im;
    /* Declare output files */
    FILE *pngout, *jpegout;
    /* Declare color indexes */
    int cyan, navy, red, green, black, white, yellow;
    int abscisse_x=1400;
    int ordonnee_y=900;


    /* Allocate the image: 64 pixels across by 64 pixels tall */
    //im = gdImageCreate(1000, 1000);
    im = gdImageCreate(abscisse_x, ordonnee_y);

    //im = gdImageCreate(hauteur,Nbr_lignes);
    /* Allocate the color black (red, green and blue all minimum).
       Since this is the first color in a new image, it will be the background color. */
    black = gdImageColorAllocate(im, 00, 00, 00);

    /* Allocate the color white (red, green and blue all maximum). */
    cyan = gdImageColorAllocate(im, 0, 255, 255);
    navy = gdImageColorAllocate(im, 0, 0, 102);
    red = gdImageColorAllocate(im, 255, 0, 0);
    green = gdImageColorAllocate(im, 0, 255, 0);
    white = gdImageColorAllocate(im, 255, 255, 255);
    yellow = gdImageColorAllocate(im, 255, 255, 0);
    /* Draw a line from the upper left to the lower right,
    using white color index. */

    //gdImageFilledRectangle(im, 250, 250, 500, 500, red);
    int sommes = 0;
    for(int i = 0; i<Nbr_lignes; i++){
        sommes = sommes + num_bikes_available[i];
        printf("%d \n", num_bikes_available[i]);

    }


    printf("%s \n", last_reported[0]);



    int X = 100;
    int eppaisseur = 25;
    int espacement = 5;

    for(int k=0;k<Nbr_lignes;k++){
        gdImageFilledRectangle(im, X, 500, X+eppaisseur, 500-(5*numDocsAvailable[k]), red);
        gdImageStringUp(im, gdFontGetLarge(), X, 705, last_reported[k], white);
        X = X + eppaisseur + espacement;

    }
    gdImageLine(im, 100, 500, 100, 100, white);
    gdImageLine(im, 100, 500, 1350, 500, white);

    gdImageString(im, gdFontGetLarge(), 700, 100, name[0], white );
    gdImageString(im, gdFontGetLarge(), 700, 200, "Nombre de bornettes disponible", white );
    gdImageString(im, gdFontGetLarge(), 50, 100, "Nombre bornettes", white );
    gdImageString(im, gdFontGetLarge(), 75, 200, "50", white );
    gdImageString(im, gdFontGetLarge(), 75, 350, "25", white );




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

    //image2: nombre de velo dispo station 13007: ebike - mecanique
    /* Declare the image */
    gdImagePtr im2;
    /* Declare output files */
    FILE *pngout2, *jpegout2;
    /* Declare color indexes */
    int cyan2, navy2, red2, green2, black2, white2, yellow2;
    int abscisse2_x=1400;
    int ordonnee2_y=900;


    /* Allocate the image: 64 pixels across by 64 pixels tall */
    im2 = gdImageCreate(abscisse2_x, ordonnee2_y);


    black2 = gdImageColorAllocate(im2, 00, 00, 00);
    cyan2 = gdImageColorAllocate(im2, 0, 255, 255);
    navy2 = gdImageColorAllocate(im2, 0, 0, 102);
    red2 = gdImageColorAllocate(im2, 255, 0, 0);
    green2 = gdImageColorAllocate(im2, 0, 255, 0);
    white2 = gdImageColorAllocate(im2, 255, 255, 255);
    yellow2 = gdImageColorAllocate(im2, 255, 255, 0);

    int X2 = 100;
    int eppaisseur2 = 25;
    int espacement2 = 5;
    for(int k=0;k<Nbr_lignes;k++){
        gdImageFilledRectangle(im2, X2, 500, X2+eppaisseur2, 500-(5*num_bikes_available_mechanical[k]), green2);
        gdImageFilledRectangle(im2, X2, 500, X2+eppaisseur2, 500-(5*num_bikes_available_ebike[k]), cyan2);
        gdImageStringUp(im2, gdFontGetLarge(), X2, 705, last_reported[k], white2);
        X2 = X2 + eppaisseur2 + espacement2;

    }
    gdImageLine(im2, 100, 500, 100, 100, white2);
    gdImageLine(im2, 100, 500, 1350, 500, white2);

    gdImageString(im2, gdFontGetLarge(), 700, 100, name[0], white2 );
    gdImageString(im2, gdFontGetLarge(), 700, 200, "Nombre de vélos disponibles", white );
    gdImageString(im2, gdFontGetLarge(), 50, 100, "ordo1", white2 );
    gdImageString(im2, gdFontGetLarge(), 75, 250, "50", white2 );
    gdImageString(im2, gdFontGetLarge(), 75, 300, "40", white2 );
    gdImageString(im2, gdFontGetLarge(), 75, 350, "30", white2 );
    gdImageString(im2, gdFontGetLarge(), 75, 400, "20", white2 );
    gdImageString(im2, gdFontGetLarge(), 75, 450, "10", white2 );

    gdImageLine(im2, 100, 500, 1350, 500, yellow2);


    gdImagePtr im3;
    FILE *in;
    in = fopen("/home/ajc/essaiessai/testimage2/icon.png", "rb");
    im3 = gdImageCreateFromPng(in);
    gdImageCopyResized(im2, im3,150,200,0,0,300,140,180,140);
    /* Open a file for writing. "wb" means "write binary", important under MSDOS, harmless under Unix. */
    pngout2 = fopen("velib2.png", "wb");
    /* Do the same for a JPEG-format file. */
    jpegout2 = fopen("velib2.jpg", "wb");
    /* Output the image to the disk file in PNG format. */
    gdImagePng(im2, pngout2);
    /* Output the same image in JPEG format, using the default JPEG quality setting. */
    gdImageJpeg(im2, jpegout2, -1);
    /* Close the files. */
    fclose(pngout2);
    fclose(jpegout2);
    /* Destroy the image in memory. */
    gdImageDestroy(im2);
}

