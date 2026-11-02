/* Editeur de formes SVG - version simple en langage C. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FORMES 100
#define MAX_POINTS 20
#define MAX_GROUPES 50
#define MAX_MEMBRES 20

typedef struct
{
    int x, y, rayon;
} Cercle;
typedef struct
{
    int x, y, rayonX, rayonY;
} Ellipse;
typedef struct
{
    int x, y, cote;
} Carre;
typedef struct
{
    int x, y, largeur, hauteur;
} Rectangle;
typedef struct
{
    int x1, y1, x2, y2;
} Ligne;

typedef struct
{
    int nombrePoints;
    int x[MAX_POINTS];
    int y[MAX_POINTS];
} Points;

enum TypeForme
{
    TYPE_CERCLE = 1,
    TYPE_ELLIPSE,
    TYPE_CARRE,
    TYPE_RECTANGLE,
    TYPE_LIGNE,
    TYPE_MULTILIGNE,
    TYPE_POLYGONE,
    TYPE_TRAJECTOIRE
};

typedef struct
{
    int type;
    int numero; /* numero de la forme dans son tableau, a partir de 1 */
} MembreGroupe;

typedef struct
{
    int nombreMembres;
    MembreGroupe membres[MAX_MEMBRES];
} Groupe;

Cercle cercles[MAX_FORMES];
int nbCercles = 0;
Ellipse ellipses[MAX_FORMES];
int nbEllipses = 0;
Carre carres[MAX_FORMES];
int nbCarres = 0;
Rectangle rectangles[MAX_FORMES];
int nbRectangles = 0;
Ligne lignes[MAX_FORMES];
int nbLignes = 0;
Points multilignes[MAX_FORMES];
int nbMultilignes = 0;
Points polygones[MAX_FORMES];
int nbPolygones = 0;
Points trajectoires[MAX_FORMES];
int nbTrajectoires = 0;
Groupe groupes[MAX_GROUPES];
int nbGroupes = 0;

int lireEntier(const char *message)
{
    int valeur, resultat, c;
    while (1)
    {
        printf(  "  ❯ "  "%s", message);
        resultat = scanf("%d", &valeur);
        if (resultat == 1)
        {
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            return valeur;
        }
        if (resultat == EOF)
            return 0;
        printf( "  Entree invalide : entrez un entier.\n" );
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }
    }
}

void saisirPoints(Points *forme, int minimum)
{
    int i;
    char message[80];
    do
    {
        forme->nombrePoints = lireEntier("Nombre de points : ");
        if (forme->nombrePoints < minimum || forme->nombrePoints > MAX_POINTS)
            printf("Entrez entre %d et %d points.\n", minimum, MAX_POINTS);
    } while (forme->nombrePoints < minimum || forme->nombrePoints > MAX_POINTS);

    for (i = 0; i < forme->nombrePoints; i++)
    {
        snprintf(message, sizeof(message), "x du point %d : ", i + 1);
        forme->x[i] = lireEntier(message);
        snprintf(message, sizeof(message), "y du point %d : ", i + 1);
        forme->y[i] = lireEntier(message);
    }
}

void afficherPoints(const Points *p, int fermer)
{
    int i;
    for (i = 0; i < p->nombrePoints; i++)
    {
        printf("(%d,%d)", p->x[i], p->y[i]);
        if (i < p->nombrePoints - 1)
            printf(" -> ");
    }
    if (fermer && p->nombrePoints > 0)
        printf(" -> (%d,%d) [ferme]", p->x[0], p->y[0]);
    printf("\n");
}

void creerCercle(void)
{
    Cercle f;
    if (nbCercles >= MAX_FORMES)
    {
        puts("Tableau de cercles plein.");
        return;
    }
    f.x = lireEntier("Centre x : ");
    f.y = lireEntier("Centre y : ");
    f.rayon = lireEntier("Rayon : ");
    if (f.rayon <= 0)
    {
        puts("Le rayon doit etre positif.");
        return;
    }
    
    cercles[nbCercles++] = f;
    puts("Cercle cree.");
}

void creerEllipse(void)
{
    Ellipse f;
    if (nbEllipses >= MAX_FORMES)
    {
        puts("Tableau d'ellipses plein.");
        return;
    }
    f.x = lireEntier("Centre x : ");
    f.y = lireEntier("Centre y : ");
    f.rayonX = lireEntier("Rayon horizontal : ");
    f.rayonY = lireEntier("Rayon vertical : ");
    if (f.rayonX <= 0 || f.rayonY <= 0)
    {
        puts("Les rayons doivent etre positifs.");
        return;
    }
    
    ellipses[nbEllipses++] = f;
    puts("Ellipse creee.");
}

void creerCarre(void)
{
    Carre f;
    if (nbCarres >= MAX_FORMES)
    {
        puts("Tableau de carres plein.");
        return;
    }
    f.x = lireEntier("Position x : ");
    f.y = lireEntier("Position y : ");
    f.cote = lireEntier("Cote : ");
    if (f.cote <= 0)
    {
        puts("Le cote doit etre positif.");
        return;
    }
    
    carres[nbCarres++] = f;
    puts("Carre cree.");
}

void creerRectangle(void)
{
    Rectangle f;
    if (nbRectangles >= MAX_FORMES)
    {
        puts("Tableau de rectangles plein.");
        return;
    }
    f.x = lireEntier("Position x : ");
    f.y = lireEntier("Position y : ");
    f.largeur = lireEntier("Largeur : ");
    f.hauteur = lireEntier("Hauteur : ");
    if (f.largeur <= 0 || f.hauteur <= 0)
    {
        puts("Dimensions invalides.");
        return;
    }
    
    rectangles[nbRectangles++] = f;
    puts("Rectangle cree.");
}

void creerLigne(void)
{
    Ligne f;
    if (nbLignes >= MAX_FORMES)
    {
        puts("Tableau de lignes plein.");
        return;
    }
    f.x1 = lireEntier("x du premier point : ");
    f.y1 = lireEntier("y du premier point : ");
    f.x2 = lireEntier("x du deuxieme point : ");
    f.y2 = lireEntier("y du deuxieme point : ");
    
    lignes[nbLignes++] = f;
    puts("Ligne creee.");
}

void creerPoints(Points tableau[], int *nombre, const char *nom, int minimum)
{
    if (*nombre >= MAX_FORMES)
    {
        printf("Tableau de %s plein.\n", nom);
        return;
    }
    printf("Creation : %s\n", nom);
    saisirPoints(&tableau[*nombre], minimum);
    
    (*nombre)++;
    printf("%s cree(e).\n", nom);
}

void afficherFormes(void)
{
    char info[256];
    
    printf("\n%s (%d)\n", "CERCLES", nbCercles);
    for (int i = 0; i < nbCercles; i++)
    {
        snprintf(info, sizeof info, "Centre (%d,%d)  •  Rayon %d", cercles[i].x, cercles[i].y, cercles[i].rayon);
        printf("  %d. %s\n", i + 1, info);
    }
    printf("\n%s (%d)\n", "ELLIPSES", nbEllipses);
    for (int i = 0; i < nbEllipses; i++)
    {
        snprintf(info, sizeof info, "Centre (%d,%d)  •  Rayons %d / %d", ellipses[i].x, ellipses[i].y, ellipses[i].rayonX, ellipses[i].rayonY);
        printf("  %d. %s\n", i + 1, info);
    }
    printf("\n%s (%d)\n", "CARRES", nbCarres);
    for (int i = 0; i < nbCarres; i++)
    {
        snprintf(info, sizeof info, "Position (%d,%d)  •  Cote %d", carres[i].x, carres[i].y, carres[i].cote);
        printf("  %d. %s\n", i + 1, info);
    }
    printf("\n%s (%d)\n", "RECTANGLES", nbRectangles);
    for (int i = 0; i < nbRectangles; i++)
    {
        snprintf(info, sizeof info, "Position (%d,%d)  •  %d x %d", rectangles[i].x, rectangles[i].y, rectangles[i].largeur, rectangles[i].hauteur);
        printf("  %d. %s\n", i + 1, info);
    }
    printf("\n%s (%d)\n", "LIGNES", nbLignes);
    for (int i = 0; i < nbLignes; i++)
    {
        snprintf(info, sizeof info, "(%d,%d)  ───→  (%d,%d)", lignes[i].x1, lignes[i].y1, lignes[i].x2, lignes[i].y2);
        printf("  %d. %s\n", i + 1, info);
    }
    printf("\n%s (%d)\n", "MULTILIGNES", nbMultilignes);
    for (int i = 0; i < nbMultilignes; i++)
    {
        snprintf(info, sizeof info, "Multiligne avec %d points", multilignes[i].nombrePoints);
        printf("  %d. %s\n", i + 1, info);
        printf("       ");
        afficherPoints(&multilignes[i], 0);
    }
    printf("\n%s (%d)\n", "POLYGONES", nbPolygones);
    for (int i = 0; i < nbPolygones; i++)
    {
        snprintf(info, sizeof info, "Polygone avec %d sommets", polygones[i].nombrePoints);
        printf("  %d. %s\n", i + 1, info);
        printf("       ");
        afficherPoints(&polygones[i], 1);
    }
    printf("\n%s (%d)\n", "TRAJECTOIRES", nbTrajectoires);
    for (int i = 0; i < nbTrajectoires; i++)
    {
        snprintf(info, sizeof info, "Trajectoire avec %d points", trajectoires[i].nombrePoints);
        printf("  %d. %s\n", i + 1, info);
        printf("       ");
        afficherPoints(&trajectoires[i], 0);
    }
    printf("\n%s (%d)\n", "GROUPES", nbGroupes);
    for (int i = 0; i < nbGroupes; i++)
    {
        snprintf(info, sizeof info, "Groupe de %d forme(s)", groupes[i].nombreMembres);
        printf("  %d. %s\n", i + 1, info);
        for (int j = 0; j < groupes[i].nombreMembres; j++)
            printf( "       ↳ "  "type %d, numero %d\n", groupes[i].membres[j].type, groupes[i].membres[j].numero);
    }
    
}

int nombrePourType(int type)
{
    switch (type)
    {
    case TYPE_CERCLE:
        return nbCercles;
    case TYPE_ELLIPSE:
        return nbEllipses;
    case TYPE_CARRE:
        return nbCarres;
    case TYPE_RECTANGLE:
        return nbRectangles;
    case TYPE_LIGNE:
        return nbLignes;
    case TYPE_MULTILIGNE:
        return nbMultilignes;
    case TYPE_POLYGONE:
        return nbPolygones;
    case TYPE_TRAJECTOIRE:
        return nbTrajectoires;
    default:
        return 0;
    }
}

const char *nomType(int type)
{
    switch (type)
    {
    case TYPE_CERCLE:
        return "cercle";
    case TYPE_ELLIPSE:
        return "ellipse";
    case TYPE_CARRE:
        return "carre";
    case TYPE_RECTANGLE:
        return "rectangle";
    case TYPE_LIGNE:
        return "ligne";
    case TYPE_MULTILIGNE:
        return "multiligne";
    case TYPE_POLYGONE:
        return "polygone";
    case TYPE_TRAJECTOIRE:
        return "trajectoire";
    default:
        return "inconnu";
    }
}

void modifierPoints(Points tableau[], int nombre, const char *nom, int minimum)
{
    int n;
    if (nombre == 0)
    {
        printf("Aucun(e) %s.\n", nom);
        return;
    }
    n = lireEntier("Numero a modifier : ");
    if (n < 1 || n > nombre)
    {
        puts("Numero invalide.");
        return;
    }
    saisirPoints(&tableau[n - 1], minimum);
    puts("Modification effectuee.");
}

void modifierForme(void)
{
    int t, n;
    puts("1 Cercle | 2 Ellipse | 3 Carre | 4 Rectangle");
    puts("5 Ligne | 6 Multiligne | 7 Polygone | 8 Trajectoire");
    t = lireEntier("Type : ");
    if (t < TYPE_CERCLE || t > TYPE_TRAJECTOIRE)
    {
        puts("Type invalide.");
        return;
    }
    if (nombrePourType(t) == 0)
    {
        printf("Aucun(e) %s a modifier.\n", nomType(t));
        return;
    }
    if (t == TYPE_MULTILIGNE)
    {
        modifierPoints(multilignes, nbMultilignes, "multiligne", 2);
        return;
    }
    if (t == TYPE_POLYGONE)
    {
        modifierPoints(polygones, nbPolygones, "polygone", 3);
        return;
    }
    if (t == TYPE_TRAJECTOIRE)
    {
        modifierPoints(trajectoires, nbTrajectoires, "trajectoire", 2);
        return;
    }
    n = lireEntier("Numero de la forme : ");
    if (n < 1 || n > nombrePourType(t))
    {
        puts("Numero invalide.");
        return;
    }
    n--;
    switch (t)
    {
    case TYPE_CERCLE:
        cercles[n].x = lireEntier("Centre x : ");
        cercles[n].y = lireEntier("Centre y : ");
        cercles[n].rayon = lireEntier("Rayon : ");
        if (cercles[n].rayon <= 0)
            puts("Attention : rayon invalide.");
        break;
    case TYPE_ELLIPSE:
        ellipses[n].x = lireEntier("Centre x : ");
        ellipses[n].y = lireEntier("Centre y : ");
        ellipses[n].rayonX = lireEntier("Rayon horizontal : ");
        ellipses[n].rayonY = lireEntier("Rayon vertical : ");
        if (ellipses[n].rayonX <= 0 || ellipses[n].rayonY <= 0)
            puts("Attention : rayons invalides.");
        break;
    case TYPE_CARRE:
        carres[n].x = lireEntier("Position x : ");
        carres[n].y = lireEntier("Position y : ");
        carres[n].cote = lireEntier("Cote : ");
        if (carres[n].cote <= 0)
            puts("Attention : cote invalide.");
        break;
    case TYPE_RECTANGLE:
        rectangles[n].x = lireEntier("Position x : ");
        rectangles[n].y = lireEntier("Position y : ");
        rectangles[n].largeur = lireEntier("Largeur : ");
        rectangles[n].hauteur = lireEntier("Hauteur : ");
        if (rectangles[n].largeur <= 0 || rectangles[n].hauteur <= 0)
            puts("Attention : dimensions invalides.");
        break;
    case TYPE_LIGNE:
        lignes[n].x1 = lireEntier("x premier point : ");
        lignes[n].y1 = lireEntier("y premier point : ");
        lignes[n].x2 = lireEntier("x deuxieme point : ");
        lignes[n].y2 = lireEntier("y deuxieme point : ");
        break;
    }
    puts("Modification terminee.");
}

/* Met a jour les references des groupes apres la suppression d'une forme. */
void mettreAJourGroupesApresSuppression(int type, int numero)
{
    int g, i, j;
    for (g = 0; g < nbGroupes; g++)
    {
        i = 0;
        while (i < groupes[g].nombreMembres)
        {
            MembreGroupe *m = &groupes[g].membres[i];
            if (m->type == type && m->numero == numero)
            {
                for (j = i; j < groupes[g].nombreMembres - 1; j++)
                    groupes[g].membres[j] = groupes[g].membres[j + 1];
                groupes[g].nombreMembres--;
                continue;
            }
            if (m->type == type && m->numero > numero)
                m->numero--;
            i++;
        }
    }
}

/* Supprime une forme d'un tableau de structures simples. */
void supprimerForme(void)
{
    int t, n;
    puts("1 Cercle | 2 Ellipse | 3 Carre | 4 Rectangle");
    puts("5 Ligne | 6 Multiligne | 7 Polygone | 8 Trajectoire");
    t = lireEntier("Type : ");
    if (t < TYPE_CERCLE || t > TYPE_TRAJECTOIRE)
    {
        puts("Type invalide.");
        return;
    }
    n = lireEntier("Numero de la forme : ");
    if (n < 1 || n > nombrePourType(t))
    {
        puts("Numero invalide.");
        return;
    }
    n--;
    mettreAJourGroupesApresSuppression(t, n + 1);
    switch (t)
    {
    case TYPE_CERCLE:
        for (int k = n; k < nbCercles - 1; k++)
            cercles[k] = cercles[k + 1];
        nbCercles--;
        break;
    case TYPE_ELLIPSE:
        for (int k = n; k < nbEllipses - 1; k++)
            ellipses[k] = ellipses[k + 1];
        nbEllipses--;
        break;
    case TYPE_CARRE:
        for (int k = n; k < nbCarres - 1; k++)
            carres[k] = carres[k + 1];
        nbCarres--;
        break;
    case TYPE_RECTANGLE:
        for (int k = n; k < nbRectangles - 1; k++)
            rectangles[k] = rectangles[k + 1];
        nbRectangles--;
        break;
    case TYPE_LIGNE:
        for (int k = n; k < nbLignes - 1; k++)
            lignes[k] = lignes[k + 1];
        nbLignes--;
        break;
    case TYPE_MULTILIGNE:
        for (int k = n; k < nbMultilignes - 1; k++)
            multilignes[k] = multilignes[k + 1];
        nbMultilignes--;
        break;
    case TYPE_POLYGONE:
        for (int k = n; k < nbPolygones - 1; k++)
            polygones[k] = polygones[k + 1];
        nbPolygones--;
        break;
    case TYPE_TRAJECTOIRE:
        for (int k = n; k < nbTrajectoires - 1; k++)
            trajectoires[k] = trajectoires[k + 1];
        nbTrajectoires--;
        break;
    }
    puts("Forme supprimee.");
}

/* Cree un groupe en enregistrant les types et numeros des formes choisies. */
void creerGroupe(void)
{
    int nb, i, t, n;
    Groupe g;
    if (nbGroupes >= MAX_GROUPES)
    {
        puts("Nombre maximal de groupes atteint.");
        return;
    }
    nb = lireEntier("Combien de formes dans le groupe (1 a 20) ? ");
    if (nb < 1 || nb > MAX_MEMBRES)
    {
        puts("Nombre invalide.");
        return;
    }
    g.nombreMembres = 0;
    for (i = 0; i < nb; i++)
    {
        printf("\nForme %d du groupe\n", i + 1);
        puts("1 Cercle | 2 Ellipse | 3 Carre | 4 Rectangle");
        puts("5 Ligne | 6 Multiligne | 7 Polygone | 8 Trajectoire");
        t = lireEntier("Type : ");
        if (t < TYPE_CERCLE || t > TYPE_TRAJECTOIRE || nombrePourType(t) == 0)
        {
            puts("Type invalide ou aucune forme de ce type. Forme ignoree.");
            continue;
        }
        n = lireEntier("Numero de cette forme : ");
        if (n < 1 || n > nombrePourType(t))
        {
            puts("Numero invalide. Forme ignoree.");
            continue;
        }
        g.membres[g.nombreMembres].type = t;
        g.membres[g.nombreMembres].numero = n;
        g.nombreMembres++;
    }
    if (g.nombreMembres == 0)
    {
        puts("Groupe non cree : aucune forme valide.");
        return;
    }
    groupes[nbGroupes++] = g;
    puts("Groupe cree.");
}

static void detailsMembre(MembreGroupe m)
{
    int i = m.numero - 1;
    printf("    "  "%-14s"  " n°%-3d "  "│ " , nomType(m.type), m.numero);
    if (i < 0 || i >= nombrePourType(m.type))
    {
        printf( "reference inexistante\n" );
        return;
    }
    switch (m.type)
    {
    case TYPE_CERCLE:
        printf("centre=(%d,%d) rayon=%d", cercles[i].x, cercles[i].y, cercles[i].rayon);
        break;
    case TYPE_ELLIPSE:
        printf("centre=(%d,%d) rayons=(%d,%d)", ellipses[i].x, ellipses[i].y, ellipses[i].rayonX, ellipses[i].rayonY);
        break;
    case TYPE_CARRE:
        printf("position=(%d,%d) cote=%d", carres[i].x, carres[i].y, carres[i].cote);
        break;
    case TYPE_RECTANGLE:
        printf("position=(%d,%d) taille=%dx%d", rectangles[i].x, rectangles[i].y, rectangles[i].largeur, rectangles[i].hauteur);
        break;
    case TYPE_LIGNE:
        printf("(%d,%d) -> (%d,%d)", lignes[i].x1, lignes[i].y1, lignes[i].x2, lignes[i].y2);
        break;
    case TYPE_MULTILIGNE:
    case TYPE_POLYGONE:
    case TYPE_TRAJECTOIRE:
    {
        Points *p = m.type == TYPE_MULTILIGNE ? &multilignes[i] : m.type == TYPE_POLYGONE ? &polygones[i]
                                                                                          : &trajectoires[i];
        printf("%d points : ", p->nombrePoints);
        for (int j = 0; j < p->nombrePoints; j++)
            printf("(%d,%d)%s", p->x[j], p->y[j], j + 1 < p->nombrePoints ? " -> " : "");
        break;
    }
    }
    printf("\n");
}
void afficherGroupe(void)
{
    if (nbGroupes == 0)
    {
        printf( "  Aucun groupe : cree d'abord un groupe (option 9).\n" );
        return;
    }
    printf( "  Groupes disponibles :\n" );
    for (int i = 0; i < nbGroupes; i++)
        printf("    "  "[%d]"  " %d membre(s)\n", i + 1, groupes[i].nombreMembres);
    int n = lireEntier( "  Numero du groupe a afficher > " );
    if (n < 1 || n > nbGroupes)
    {
        printf( "  Numero invalide.\n" );
        return;
    }
    Groupe *g = &groupes[n - 1];
    puts("COMPOSITION DETAILLEE DU GROUPE");
    printf( "  GROUPE #%d  "   "• %d forme(s)\n" , n, g->nombreMembres);
    printf( "  ────────────────────────────────────────────────────────────\n" );
    for (int i = 0; i < g->nombreMembres; i++)
    {
        printf( "  ✦ Membre %d/%d"  "\n", i + 1, g->nombreMembres);
        detailsMembre(g->membres[i]);
    }
    printf( "  ────────────────────────────────────────────────────────────\n" );
}

void supprimerGroupe(void)
{
    int n, i;
    if (nbGroupes == 0)
    {
        puts("Aucun groupe.");
        return;
    }
    n = lireEntier("Numero du groupe a supprimer : ");
    if (n < 1 || n > nbGroupes)
    {
        puts("Numero invalide.");
        return;
    }
    for (i = n - 1; i < nbGroupes - 1; i++)
        groupes[i] = groupes[i + 1];
    nbGroupes--;
    puts("Groupe supprime.");
}

/* Exporte les formes dans un fichier SVG ouvrable avec un navigateur. */
void exporterSVG(void)
{
    FILE *f = fopen("dessin.svg", "w");
    int i, j;

    if (f == NULL)
    {
        puts("Impossible de creer dessin.svg.");
        return;
    }

    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"600\" viewBox=\"0 0 800 600\">\n");
    fprintf(f, "  <rect width=\"800\" height=\"600\" fill=\"white\"/>\n");

    for (i = 0; i < nbCercles; i++)
        fprintf(f, "  <circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"lightblue\" stroke=\"black\" stroke-width=\"2\"/>\n", cercles[i].x, cercles[i].y, cercles[i].rayon);

    for (i = 0; i < nbEllipses; i++)
        fprintf(f, "  <ellipse cx=\"%d\" cy=\"%d\" rx=\"%d\" ry=\"%d\" fill=\"lightpink\" stroke=\"black\" stroke-width=\"2\"/>\n", ellipses[i].x, ellipses[i].y, ellipses[i].rayonX, ellipses[i].rayonY);

    for (i = 0; i < nbCarres; i++)
        fprintf(f, "  <rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"lightgreen\" stroke=\"black\" stroke-width=\"2\"/>\n", carres[i].x, carres[i].y, carres[i].cote, carres[i].cote);

    for (i = 0; i < nbRectangles; i++)
        fprintf(f, "  <rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"khaki\" stroke=\"black\" stroke-width=\"2\"/>\n", rectangles[i].x, rectangles[i].y, rectangles[i].largeur, rectangles[i].hauteur);

    for (i = 0; i < nbLignes; i++)
        fprintf(f, "  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" stroke-width=\"2\"/>\n", lignes[i].x1, lignes[i].y1, lignes[i].x2, lignes[i].y2);

    for (i = 0; i < nbMultilignes; i++)
    {
        fprintf(f, "  <polyline points=\"");
        for (j = 0; j < multilignes[i].nombrePoints; j++)
            fprintf(f, "%s%d,%d", j ? " " : "", multilignes[i].x[j], multilignes[i].y[j]);
        fprintf(f, "\" fill=\"none\" stroke=\"blue\" stroke-width=\"2\"/>\n");
    }

    for (i = 0; i < nbPolygones; i++)
    {
        fprintf(f, "  <polygon points=\"");
        for (j = 0; j < polygones[i].nombrePoints; j++)
            fprintf(f, "%s%d,%d", j ? " " : "", polygones[i].x[j], polygones[i].y[j]);
        fprintf(f, "\" fill=\"plum\" stroke=\"black\" stroke-width=\"2\"/>\n");
    }

    for (i = 0; i < nbTrajectoires; i++)
    {
        fprintf(f, "  <path d=\"");
        for (j = 0; j < trajectoires[i].nombrePoints; j++)
            fprintf(f, "%c %d %d", j == 0 ? 'M' : 'L', trajectoires[i].x[j], trajectoires[i].y[j]);
        fprintf(f, "\" fill=\"none\" stroke=\"red\" stroke-width=\"2\"/>\n");
    }

    fprintf(f, "</svg>\n");
    fclose(f);
    puts("Dessin exporte dans dessin.svg. Ouvre ce fichier avec Safari ou Chrome.");
}


/* Le SVG est un format d'affichage, pas un format de sauvegarde editable. */
typedef struct
{
    Cercle cercles[MAX_FORMES];
    int nc;
    Ellipse ellipses[MAX_FORMES];
    int ne;
    Carre carres[MAX_FORMES];
    int nk;
    Rectangle rectangles[MAX_FORMES];
    int nr;
    Ligne lignes[MAX_FORMES];
    int nl;
    Points multilignes[MAX_FORMES];
    int nm;
    Points polygones[MAX_FORMES];
    int np;
    Points trajectoires[MAX_FORMES];
    int nt;
    Groupe groupes[MAX_GROUPES];
    int ng;
} Sauvegarde;

static int ecrirePoints(FILE *f, const Points *p)
{
    if (fprintf(f, "%d", p->nombrePoints) < 0)
        return 0;
    for (int i = 0; i < p->nombrePoints; i++)
        if (fprintf(f, " %d %d", p->x[i], p->y[i]) < 0)
            return 0;
    return fprintf(f, "\n") >= 0;
}
static int lirePoints(FILE *f, Points *p, int minimum)
{
    if (fscanf(f, "%d", &p->nombrePoints) != 1 || p->nombrePoints < minimum || p->nombrePoints > MAX_POINTS)
        return 0;
    for (int i = 0; i < p->nombrePoints; i++)
        if (fscanf(f, "%d %d", &p->x[i], &p->y[i]) != 2)
            return 0;
    return 1;
}
void sauvegarderProjet(void)
{
    FILE *f = fopen("projet_formes.txt", "w");
    if (!f)
    {
        perror("Sauvegarde impossible");
        return;
    }
    fprintf(f, "ATELIER_FORMES_V1\n");
    fprintf(f, "%d %d %d %d %d %d %d %d %d\n", nbCercles, nbEllipses, nbCarres, nbRectangles, nbLignes, nbMultilignes, nbPolygones, nbTrajectoires, nbGroupes);
    for (int i = 0; i < nbCercles; i++)
        fprintf(f, "%d %d %d\n", cercles[i].x, cercles[i].y, cercles[i].rayon);
    for (int i = 0; i < nbEllipses; i++)
        fprintf(f, "%d %d %d %d\n", ellipses[i].x, ellipses[i].y, ellipses[i].rayonX, ellipses[i].rayonY);
    for (int i = 0; i < nbCarres; i++)
        fprintf(f, "%d %d %d\n", carres[i].x, carres[i].y, carres[i].cote);
    for (int i = 0; i < nbRectangles; i++)
        fprintf(f, "%d %d %d %d\n", rectangles[i].x, rectangles[i].y, rectangles[i].largeur, rectangles[i].hauteur);
    for (int i = 0; i < nbLignes; i++)
        fprintf(f, "%d %d %d %d\n", lignes[i].x1, lignes[i].y1, lignes[i].x2, lignes[i].y2);
    for (int i = 0; i < nbMultilignes; i++)
        ecrirePoints(f, &multilignes[i]);
    for (int i = 0; i < nbPolygones; i++)
        ecrirePoints(f, &polygones[i]);
    for (int i = 0; i < nbTrajectoires; i++)
        ecrirePoints(f, &trajectoires[i]);
    for (int i = 0; i < nbGroupes; i++)
    {
        fprintf(f, "%d", groupes[i].nombreMembres);
        for (int j = 0; j < groupes[i].nombreMembres; j++)
            fprintf(f, " %d %d", groupes[i].membres[j].type, groupes[i].membres[j].numero);
        fprintf(f, "\n");
    }
    if (ferror(f) || fclose(f) != 0)
    {
        puts("Erreur pendant la sauvegarde.");
        return;
    }
    
    puts("Projet sauvegarde dans projet_formes.txt");
}
void importerProjet(void)
{
    FILE *f = fopen("projet_formes.txt", "r");
    if (!f)
    {
        puts("Fichier projet_formes.txt introuvable.");
        return;
    }
    Sauvegarde *a = calloc(1, sizeof(*a));
    if (!a)
    {
        fclose(f);
        puts("Memoire insuffisante.");
        return;
    }
    char entete[64];
    int ok = 1;
    if (fscanf(f, "%63s", entete) != 1 || strcmp(entete, "ATELIER_FORMES_V1") != 0)
        ok = 0;
    if (ok && fscanf(f, "%d %d %d %d %d %d %d %d %d", &a->nc, &a->ne, &a->nk, &a->nr, &a->nl, &a->nm, &a->np, &a->nt, &a->ng) != 9)
        ok = 0;
    if (ok && (a->nc < 0 || a->nc > MAX_FORMES || a->ne < 0 || a->ne > MAX_FORMES || a->nk < 0 || a->nk > MAX_FORMES || a->nr < 0 || a->nr > MAX_FORMES || a->nl < 0 || a->nl > MAX_FORMES || a->nm < 0 || a->nm > MAX_FORMES || a->np < 0 || a->np > MAX_FORMES || a->nt < 0 || a->nt > MAX_FORMES || a->ng < 0 || a->ng > MAX_GROUPES))
        ok = 0;
    for (int i = 0; ok && i < a->nc; i++)
        if (fscanf(f, "%d %d %d", &a->cercles[i].x, &a->cercles[i].y, &a->cercles[i].rayon) != 3 || a->cercles[i].rayon <= 0)
            ok = 0;
    for (int i = 0; ok && i < a->ne; i++)
        if (fscanf(f, "%d %d %d %d", &a->ellipses[i].x, &a->ellipses[i].y, &a->ellipses[i].rayonX, &a->ellipses[i].rayonY) != 4 || a->ellipses[i].rayonX <= 0 || a->ellipses[i].rayonY <= 0)
            ok = 0;
    for (int i = 0; ok && i < a->nk; i++)
        if (fscanf(f, "%d %d %d", &a->carres[i].x, &a->carres[i].y, &a->carres[i].cote) != 3 || a->carres[i].cote <= 0)
            ok = 0;
    for (int i = 0; ok && i < a->nr; i++)
        if (fscanf(f, "%d %d %d %d", &a->rectangles[i].x, &a->rectangles[i].y, &a->rectangles[i].largeur, &a->rectangles[i].hauteur) != 4 || a->rectangles[i].largeur <= 0 || a->rectangles[i].hauteur <= 0)
            ok = 0;
    for (int i = 0; ok && i < a->nl; i++)
        if (fscanf(f, "%d %d %d %d", &a->lignes[i].x1, &a->lignes[i].y1, &a->lignes[i].x2, &a->lignes[i].y2) != 4)
            ok = 0;
    for (int i = 0; ok && i < a->nm; i++)
        ok = lirePoints(f, &a->multilignes[i], 2);
    for (int i = 0; ok && i < a->np; i++)
        ok = lirePoints(f, &a->polygones[i], 3);
    for (int i = 0; ok && i < a->nt; i++)
        ok = lirePoints(f, &a->trajectoires[i], 2);
    const int nombres[] = {a->nc, a->ne, a->nk, a->nr, a->nl, a->nm, a->np, a->nt};
    for (int i = 0; ok && i < a->ng; i++)
    {
        Groupe *g = &a->groupes[i];
        if (fscanf(f, "%d", &g->nombreMembres) != 1 || g->nombreMembres < 1 || g->nombreMembres > MAX_MEMBRES)
        {
            ok = 0;
            break;
        }
        for (int j = 0; ok && j < g->nombreMembres; j++)
        {
            MembreGroupe *m = &g->membres[j];
            if (fscanf(f, "%d %d", &m->type, &m->numero) != 2 || m->type < 1 || m->type > 8)
            {
                ok = 0;
                break;
            }
            if (m->numero < 1 || m->numero > nombres[m->type - 1])
                ok = 0;
        }
    }
    fclose(f);
    if (!ok)
    {
        free(a);
        puts("Fichier invalide : les donnees actuelles ont ete conservees.");
        return;
    }
    memcpy(cercles, a->cercles, sizeof(cercles));
    nbCercles = a->nc;
    memcpy(ellipses, a->ellipses, sizeof(ellipses));
    nbEllipses = a->ne;
    memcpy(carres, a->carres, sizeof(carres));
    nbCarres = a->nk;
    memcpy(rectangles, a->rectangles, sizeof(rectangles));
    nbRectangles = a->nr;
    memcpy(lignes, a->lignes, sizeof(lignes));
    nbLignes = a->nl;
    memcpy(multilignes, a->multilignes, sizeof(multilignes));
    nbMultilignes = a->nm;
    memcpy(polygones, a->polygones, sizeof(polygones));
    nbPolygones = a->np;
    memcpy(trajectoires, a->trajectoires, sizeof(trajectoires));
    nbTrajectoires = a->nt;
    memcpy(groupes, a->groupes, sizeof(groupes));
    nbGroupes = a->ng;
    free(a);
    
    puts("Projet recharge depuis projet_formes.txt");
}

void ouvrirSVG(void)
{
#ifdef __APPLE__
    /* Sur Mac, ouvre le dessin SVG dans l'application par defaut. */
    system("open dessin.svg");
#else
    puts("Ouvrez dessin.svg dans votre navigateur pour voir les formes.");
#endif
}

void afficherMenu(void)
{
    puts("\n===== EDITEUR DE FORMES SVG =====");
    puts(" 1 Cercle          2 Ellipse");
    puts(" 3 Carre           4 Rectangle");
    puts(" 5 Ligne           6 Multiligne");
    puts(" 7 Polygone        8 Trajectoire");
    puts(" 9 Creer groupe   10 Afficher les formes");
    puts("11 Modifier       12 Supprimer une forme");
    puts("13 Voir groupe    14 Supprimer un groupe");
    puts("15 Exporter SVG et afficher le dessin");
    puts("16 Sauvegarder    17 Importer");
    puts(" 0 Quitter");
}

int main(void)
{
    int choix;
    do
    {
        afficherMenu();
        choix = lireEntier("Votre choix : ");
        switch (choix)
        {
        case 1: creerCercle(); break;
        case 2: creerEllipse(); break;
        case 3: creerCarre(); break;
        case 4: creerRectangle(); break;
        case 5: creerLigne(); break;
        case 6: creerPoints(multilignes, &nbMultilignes, "multiligne", 2); break;
        case 7: creerPoints(polygones, &nbPolygones, "polygone", 3); break;
        case 8: creerPoints(trajectoires, &nbTrajectoires, "trajectoire", 2); break;
        case 9: creerGroupe(); break;
        case 10: afficherFormes(); break;
        case 11: modifierForme(); break;
        case 12: supprimerForme(); break;
        case 13: afficherGroupe(); break;
        case 14: supprimerGroupe(); break;
        case 15: exporterSVG(); ouvrirSVG(); break;
        case 16: sauvegarderProjet(); break;
        case 17: importerProjet(); break;
        case 0: puts("Au revoir !"); break;
        default: puts("Choix invalide.");
        }
    } while (choix != 0);
    return 0;
}
