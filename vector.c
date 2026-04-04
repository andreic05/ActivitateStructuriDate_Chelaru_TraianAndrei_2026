#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Film {
    char* denumire;
    char* regizor;
    float durata;
    int rating;
};

typedef struct Film Film;

Film initFilm(const char* den, const char* reg, float dur, int r) {
    Film f;
    f.denumire = (char*) malloc(sizeof(char) * (strlen(den) + 1));
    strcpy(f.denumire, den);
    f.regizor = (char*) malloc(sizeof(char) * (strlen(reg) + 1));
    strcpy(f.regizor, reg);
    f.durata = dur;
    f.rating = r;

    return f;
}

Film* initVector(int* dim) {
    *dim = 8;
    Film* vector = (Film*) malloc(sizeof(Film) * (*dim));
    vector[0] = initFilm("Inception", "Christopher Nolan", 148.35, 9);
    vector[1] = initFilm("Interstellar", "Christopher Nolan", 169.42, 9);
    vector[2] = initFilm("The Matrix", "Lana Wachowski", 136.75, 9);
    vector[3] = initFilm("Titanic", "James Cameron", 195.50, 8);
    vector[4] = initFilm("Avatar", "James Cameron", 162.20, 8);
    vector[5] = initFilm("The Godfather", "Francis Ford Coppola", 175.90, 10);
    vector[6] = initFilm("Pulp Fiction", "Quentin Tarantino", 154.65, 9);
    vector[7] = initFilm("The Dark Knight", "Christopher Nolan", 152.10, 10);

    return vector;
}

void afisareFilm(Film film) {
    printf("\nFilmul %s, regizat de %s, are o durata de %.2f minute si un rating de %d.", film.denumire, film.regizor, film.durata, film.rating);
}

void afisareVector(Film* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
        afisareFilm(*vector);
        vector++;
    }
}

Film* copiazaPrimeleNElemente(Film* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
    if (nrElementeCopiate > nrElemente || nrElementeCopiate <= 0) return NULL;
	Film *vectorNou= (Film*) malloc(sizeof(Film) * nrElementeCopiate);
    for (int i = 0; i < nrElementeCopiate; i++) {
        vectorNou[i] = initFilm(vector->denumire, vector->regizor, vector->durata, vector->rating);
        vector++;
    }
    
	return vectorNou;
}

void dezalocare(Film** vector, int* nrElemente) {
	//dezalocam elementele din vector si vectorul
    for (int i = 0; i < *nrElemente; i++) {
        free((*vector)[i].denumire);
        free((*vector)[i].regizor);
    }

    free(*vector);
    *vector = NULL;
    *nrElemente = 0;
}

void copiazaAnumiteElemente(Film* vector, int nrElemente, int prag, Film** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie

    //contidie: copiaza filmele cu rating mai mare sau egal cu prag
    *vectorNou = (Film*) malloc(sizeof(Film) * nrElemente);
    *dimensiune = 0;
    for (int i=0; i<nrElemente; i++) {
        if (vector[i].rating >= prag) (*vectorNou)[(*dimensiune)++] = vector[i];
    }

    Film* aux = *vectorNou;

    *vectorNou = copiazaPrimeleNElemente(aux, *dimensiune, *dimensiune);
    free(aux);
}

Film getPrimulElementConditionat(Film* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.

    // conditie: denumire
	for (int i=0; i<nrElemente; i++) {
        if (strcmp(vector->denumire, conditie) == 0) return initFilm(vector->denumire, vector->regizor, vector->durata, vector->rating);
        vector++;
    }
	

	return initFilm("", "", 0, 0);
}
	


int main() {
    int dim = 0;
    Film* vector = initVector(&dim);
    afisareVector(vector, dim);
    
    int dimCopie = 5;
    Film* copie = copiazaPrimeleNElemente(vector, dim, dimCopie);
    printf("\n\nAfisare copie: ");
    if (copie != NULL) afisareVector(copie, dimCopie);

    dezalocare(&copie, &dimCopie);
    copie = NULL;
    dimCopie = 0;

    copiazaAnumiteElemente(vector, dim, 9, &copie, &dimCopie);
    printf("\n\nAfisare filme cu rating mai mare sau egal cu 9: ");
    if (copie != NULL) afisareVector(copie, dimCopie);

    printf("\n=======================\n");
    printf("Afisare film cu denumirea Interstellar:");
    Film f = getPrimulElementConditionat(vector, dim, "Interstellar");
    afisareFilm(f);
    free(f.denumire);
    free(f.regizor);
    
    dezalocare(&copie, &dimCopie);
    dezalocare(&vector, &dim);
    
	return 0;
}