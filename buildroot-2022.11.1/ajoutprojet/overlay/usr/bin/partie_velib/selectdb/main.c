#include "gd.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <sqlite3.h>
#include <gdfonts.h>
#include <gdfontl.h>
#include <gdfontg.h>
#include <stdio.h>

/*
** Connexion à la base de donnée velibdata.db
** Récupération des données
** Utilisation des données en paramètre dans gd
**
*/

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;
    printf("\n");
    return 0;
}

int main() {
    sqlite3 *db;
    char *err_msg = 0;
        int retenirvalue = sqlite3_open("/var/db_velib/velibdata.db", &db);
        if (retenirvalue != SQLITE_OK) {
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return 1;
        }
        char *sql = "SELECT name, capacity, is_renting, is_returning,numDocsAvailable,num_bikes_available,num_bikes_available_ebike, num_bikes_available_mechanical, last_reported FROM (SELECT * FROM station_status LEFT JOIN station_information ON station_status.station_id = station_information.station_id) WHERE station_id = 13007 GROUP BY last_reported"; 
        
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
        query = "SELECT name, capacity, is_renting, is_returning,numDocsAvailable,num_bikes_available,num_bikes_available_ebike, num_bikes_available_mechanical, last_reported FROM (SELECT * FROM station_status LEFT JOIN station_information ON station_status.station_id = station_information.station_id) WHERE station_id = 13007 GROUP BY last_reported";

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

        //écrire   des données:
        //initialiser nombre de lignes = Nbr_lignes
        int Nbr_lignes = 0;
        while(1)
        {
            //récupérer le status de la ligne (contient de données ou fin de table)
            retenirvalue = sqlite3_step(stmt);
            if(retenirvalue == SQLITE_ROW)
            {
                //ici, la ligne contient des données
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
                 printf("Fin de la table\n");
                 break;
             }
             else
             {
                 //renvoyer une erreur
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
             //la ligne contient des données:
             //sqlite3_column_double : renvoie un double, pas un pointeur vers un double
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

             printf("\n");
             printf("num_lignes  : %d \n",num_lignes);
             num_lignes += 1;
            }
            else if(retenirvalue == SQLITE_DONE)
            {
                printf("Fin de la table\n");
                break;
            }
            else
            {
                //renvoyer une erreur
                printf("Erreur lors de l'accès aux données\n");
                return -1;
            }
        }
        //fermeture de db pour libérer de la mémoire
        sqlite3_close(db);

//**************création des images dans GD************************************
    /*image1: nombre de bornettes disponibles au station Le Brun Gobelin (station_id = 13007*/
    /* déclarer l'image */
    gdImagePtr im;

    /* déclarer les fichiers de sortie */
    FILE *pngout;

    /* déclarer l'index des couleurs */
    int cyan, navy, red, green, black, white, yellow;

    /* définir les fonts à utiliser */
    char *medium = "/usr/share/fonts/truetype/lato/Lato-Regular.ttf";
    char *italic = "/usr/share/fonts/truetype/lato/Lato-LightItalic.ttf";
    char *regular = "/usr/share/fonts/truetype/lato/Lato-Medium.ttf ";

    /* définir la largeur et la hauteur de l'image */
    int abscisse_x=1800;
    int ordonnee_y=900;
    im = gdImageCreate(abscisse_x, ordonnee_y);

    /* définir la couleur noire comme background image (première couleur définie sert de backgound color) */
    black = gdImageColorAllocate(im, 00, 00, 00);

    /* définir les couleurs utilisées pour les images */
    cyan = gdImageColorAllocate(im, 0, 255, 255);
    navy = gdImageColorAllocate(im, 0, 0, 102);
    red = gdImageColorAllocate(im, 255, 0, 0);
    green = gdImageColorAllocate(im, 0, 255, 0);
    white = gdImageColorAllocate(im, 255, 255, 255);
    yellow = gdImageColorAllocate(im, 255, 255, 0);

    /* tracer les graphes */
    int X = 100;
    int eppaisseur = 25;
    int espacement = 5;

    for(int k=0;k<Nbr_lignes;k++){
        gdImageFilledRectangle(im, X, 500, X+eppaisseur, 500-(5*numDocsAvailable[k]), red);
        gdImageStringUp(im, gdFontGetLarge(), X, 705, last_reported[k], white);
        X = X + eppaisseur + espacement;
    }

    /* tracer les axes abscisse et ordonnée */
    gdImageLine(im, 100, 500, 100, 100, white);
    gdImageLine(im, 100, 500, 1550, 500, white);

    /* ajouter les titres et graduation */
    gdImageStringFT(im,NULL,yellow,medium,18,0,700, 150, name[0]);
    gdImageStringFT(im,NULL,white,medium,18,0,650, 100, "Nombre de bornettes disponible");
    gdImageStringFT(im,NULL,white,italic,12,0,75, 200, "50");
    gdImageStringFT(im,NULL,white,italic,12,0,75, 350, "25");
    
    gdImageFilledRectangle(im, 1250, 85, 1200, 100, red);
    gdImageStringFT(im,NULL,red,medium,12,0,1260,100, "Nombre de bornettes disponible par heure");

    /* ouvrir un fichier pour l'écriture: "wb" (write binary) */
    pngout = fopen("/var/www/html/velib.png", "wb");

    /* exporter l'image vers le fichier disque au format PNG */
    gdImagePng(im, pngout);

    /* fermer le fichier */
    fclose(pngout);

    /* détruit l'image dans la mémoire */
    gdImageDestroy(im);

    /* image2: nombre de velo dispo station 13007: ebike - mecanique */
    /* déclarer l'image */
    gdImagePtr im2;

    /* déclarer les fichiers de sortie */
    FILE *pngout2;

    /* déclarer l'index des couleurs */
    int cyan2, navy2, red2, green2, black2, white2, yellow2;

    /* définir la largeur et la hauteur de l'image */
    int abscisse2_x=1800;
    int ordonnee2_y=900;
    im2 = gdImageCreate(abscisse2_x, ordonnee2_y);

    /* définir les couleurs utilisées pour les images */
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
    gdImageLine(im2, 100, 500, 1550, 500, white2);

    gdImageStringFT(im2,NULL,white2,medium,18,0,700, 100, "Nombre de vélos disponibles");
    gdImageStringFT(im2,NULL,yellow2,medium,18,0,750, 200, name[0]);
    gdImageStringFT(im2,NULL,white2,italic,12,0,75, 250, "50");
    gdImageStringFT(im2,NULL,white2,italic,12,0,75, 300, "40");
    gdImageStringFT(im2,NULL,white2,italic,12,0,75, 350, "30");
    gdImageStringFT(im2,NULL,white2,italic,12,0,75, 400, "20");
    gdImageStringFT(im2,NULL,white2,italic,12,0,75, 450, "10");
    
    gdImageFilledRectangle(im2, 1200, 50, 1220, 75, cyan2);
    gdImageStringFT(im2,NULL,cyan2,medium,12,0,1230,75, "Nombre de vélos électriques disponibles");
    gdImageFilledRectangle(im2, 1200, 75, 1220, 100, green2);
    gdImageStringFT(im2,NULL,green2,medium,12,0,1230,100, "Nombre de vélos mécaniques disponibles");

    /* déclarer l'image */
    /* insérer icon de velib */
    gdImagePtr im3;
    FILE *in;
    in = fopen("/var/www/html/icon.png", "rb");
    im3 = gdImageCreateFromPng(in);
    gdImageCopyResized(im2, im3,150,200,0,0,300,140,180,140);

    /* ouvrir un fichier pour l'écriture: "wb" (write binary) */
    pngout2 = fopen("/var/www/html/velib2.png", "wb");


    /* exporter l'image vers le fichier disque au format PNG */
    gdImagePng(im2, pngout2);

    /* fermer le fichier */
    fclose(pngout2);

    /* détruit l'image dans la mémoire */
    gdImageDestroy(im2);

    /* image3: répartition part vélo: vélo mécanique et ebike */
    /* déclarer l'image */
    gdImagePtr im4;

    /* déclarer les fichiers de sortie */
    FILE *pngout4;

    /* déclarer l'index des couleurs */
    int cyan4, navy4, red4, green4, black4, white4, yellow4;

    /* définir la largeur et la hauteur de l'image */
    int abs_x=1000;
    int ord_y=1000;
    im4 = gdImageCreate(abs_x, ord_y);

    /* définir la couleur noire comme background image (première couleur définie sert de backgound color)*/
    black4 = gdImageColorAllocate(im4, 00, 00, 00);

    /* définir les couleurs utilisées pour les images */
    cyan4 = gdImageColorAllocate(im4, 0, 255, 255);
    navy4 = gdImageColorAllocate(im4, 0, 0, 102);
    red4 = gdImageColorAllocate(im4, 255, 0, 0);
    green4 = gdImageColorAllocate(im4, 0, 255, 0);
    white4 = gdImageColorAllocate(im4, 255, 255, 255);
    yellow4 = gdImageColorAllocate(im4, 255, 255, 0);

    /* tracer les graphes */
    int c_x = (abs_x/2);
    int c_y = (ord_y/2);
    int w = 420;
    int h = 360;

    gdImageFilledArc(im4, c_x, c_y, w, h, 0,((num_bikes_available_ebike[Nbr_lignes-1]*360)/num_bikes_available[Nbr_lignes-1]), yellow, gdArc);
    gdImageFilledArc(im4, c_x, c_y, w, h, (0+((num_bikes_available_ebike[Nbr_lignes-1]*360)/num_bikes_available[Nbr_lignes-1])),((num_bikes_available_ebike[Nbr_lignes-1]*360)/num_bikes_available[Nbr_lignes-1]) + ((num_bikes_available_mechanical[Nbr_lignes-1]*360)/num_bikes_available[Nbr_lignes-1]), red, gdArc);

    int mecanical = round(((num_bikes_available_mechanical[Nbr_lignes-1]*100)/(num_bikes_available_ebike[Nbr_lignes-1]+num_bikes_available_mechanical[Nbr_lignes-1])));
    char str[40]; //spécifie emplacement mémoire dans lequel stocker la nouvelle valeur
    sprintf(str, "%d",mecanical);
    gdImageStringFT(im4,NULL,red,medium,14,0,750, 550, strcat(str," % de vélo mecanique"));
    int ebike = round(((num_bikes_available_ebike[Nbr_lignes-1]*100)/(num_bikes_available_ebike[Nbr_lignes-1]+num_bikes_available_mechanical[Nbr_lignes-1])));
    char str1[40]; //spécifie emplacement mémoire dans lequel stocker la nouvelle valeur
    sprintf(str1, "%d",ebike);

    /* récupérer la date au format YYYY-MM-DD */
    char date[11];
    for(int i=0; i<10; i++){
        date[i] = last_reported[Nbr_lignes-1][i];
    }
    date[10] = 0; //fin de string
    printf("%s \n", date);

    /* récupérer l'heure au format HH-mm */
    char heure[6];
    for(int i=0; i<5; i++){
        heure[i] = last_reported[Nbr_lignes-1][i+11];
    }
    heure[5] = 0; //fin de string
    printf("%s \n", heure);

    printf("%s \n", last_reported[0]);

    gdImageStringFT(im4,NULL,yellow,medium,14,0,750, 600, strcat(str1," % de vélo ebike"));
    gdImageString(im4, gdFontGetGiant(), 460, 750, heure, white);
    gdImageString(im4, gdFontGetGiant(), 450, 250, date, white);
    gdImageStringFT(im4,NULL,green,italic,18,0,300, 200, "REPARTITION DES VELOS DISPONIBLES");

    /* ouvrir un fichier pour l'écriture: "wb" (write binary) */
    pngout4 = fopen("/var/www/html/velib3.png", "wb");

    /* exporter l'image vers le fichier disque au format PNG */
    gdImagePng(im4, pngout4);

    /* fermer le fichier */
    fclose(pngout4);
  
    /* détruit l'image dans la mémoire */
    gdImageDestroy(im4);
}

