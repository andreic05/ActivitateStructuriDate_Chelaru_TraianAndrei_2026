#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Film Film;
typedef struct Nod Nod;

struct Film {
    char* denumire;
    char* regizor;
    float durata;
    int rating;
};

struct Nod {
    Film info;
    Nod* next;
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

void push(Nod** baza, Film film) {
    Nod* nou = (Nod*) malloc(sizeof(Nod));
    nou->info = film;
    nou->next = NULL;

    if (*baza == NULL) *baza = nou;
    else {
        Nod* p = *baza;
        while (p->next) p = p->next;
        p->next = nou;
    }
}

Film pop(Nod** baza) {
    if (*baza == NULL) return initFilm("","",0,0);
    else {
        Nod* prev = *baza;
        Nod* p = (*baza)->next;

        if (p) {
            while (p->next) {
                prev = p;
                p = p->next;
            }
            prev->next = NULL;
        } else {
            p = prev;
            *baza = NULL;
        }

        Film film = initFilm(p->info.denumire, p->info.regizor, p->info.durata, p->info.rating);
        free(p->info.denumire);
        free(p->info.regizor);
        free(p);
            
        return film;
    }
}

void initStiva(Nod** stiva) {
    push(stiva, initFilm("Inception", "Christopher Nolan", 148.35, 9));
    push(stiva, initFilm("Interstellar", "Christopher Nolan", 169.42, 9));
    push(stiva, initFilm("The Matrix", "Lana Wachowski", 136.75, 9));
    push(stiva, initFilm("Titanic", "James Cameron", 195.50, 8));
    push(stiva, initFilm("Avatar", "James Cameron", 162.20, 8));
    push(stiva, initFilm("The Godfather", "Francis Ford Coppola", 175.90, 10));
    push(stiva, initFilm("Pulp Fiction", "Quentin Tarantino", 154.65, 9));
    push(stiva, initFilm("The Dark Knight", "Christopher Nolan", 152.10, 10));
}

int main() {
    Nod* stiva = (Nod*) malloc(sizeof(Nod));
    stiva = NULL;

    initStiva(&stiva);

    Film film;
    while (stiva) {
        film = pop(&stiva);
        afisareFilm(film);
        free(film.denumire);
        free(film.regizor);
    }



    return 0;
}