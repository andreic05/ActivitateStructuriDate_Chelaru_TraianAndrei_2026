#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Film Film;
typedef struct Nod Nod;
typedef struct Coada Coada;

struct Film {
    char* denumire;
    char* regizor;
    float durata;
    int rating;
};

struct Nod {
    Film info;
    Nod* next;
    Nod* prev;
};

struct Coada {
    Nod* cap;
    Nod* coada;
};

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

void afisareFilm(Film film) {
    printf("\nFilmul %s, regizat de %s, are o durata de %.2f minute si un rating de %d.", film.denumire, film.regizor, film.durata, film.rating);
}

void put(Coada* coada, Film film) {
    Nod* nou = (Nod*) malloc(sizeof(Nod));
    nou->info = film;
    nou->next = NULL;
    nou->prev = NULL;
    
    if (coada->coada == NULL) {
        coada->cap = coada->coada = nou;
    } 
    else {
        nou->prev = coada->coada;
        coada->coada->next = nou;
        coada->coada = nou;
    }
}

Film get(Coada* coada) {
    if (coada->cap == NULL) return initFilm("", "", 0, 0);
    
    Nod* p = coada->cap;
    if (coada->cap == coada->coada) {
        coada->cap = NULL;
        coada->coada = NULL;
    } else {
        coada->cap->next->prev = NULL;
        coada->cap = coada->cap->next;
    }

    Film f = initFilm(p->info.denumire, p->info.regizor, p->info.durata, p->info.rating);
    free(p->info.denumire);
    free(p->info.regizor);
    free(p);

    return f;
}

void initCoada(Coada* coada) {
    put(coada, initFilm("Inception", "Christopher Nolan", 148.35, 9));
    put(coada, initFilm("Interstellar", "Christopher Nolan", 169.42, 9));
    put(coada, initFilm("The Matrix", "Lana Wachowski", 136.75, 9));
    put(coada, initFilm("Titanic", "James Cameron", 195.50, 8));
    put(coada, initFilm("Avatar", "James Cameron", 162.20, 8));
    put(coada, initFilm("The Godfather", "Francis Ford Coppola", 175.90, 10));
    put(coada, initFilm("Pulp Fiction", "Quentin Tarantino", 154.65, 9));
    put(coada, initFilm("The Dark Knight", "Christopher Nolan", 152.10, 10));
}

int main() {
    Coada coada;
    coada.cap = NULL;
    coada.coada = NULL;

    initCoada(&coada);

    Film film;
    while (coada.cap) {
        film = get(&coada);
        afisareFilm(film);
        free(film.denumire);
        free(film.regizor);
    }



    return 0;
}