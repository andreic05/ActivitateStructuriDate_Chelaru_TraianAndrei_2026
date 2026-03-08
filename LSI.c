#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Smartphone {
	int id;
	char* brand;
	char* model;
	char* os;
	int ram;
	int stocare;
	float pret;
	int stoc;
};



typedef struct Smartphone Sph;
typedef struct NodLista NL;

struct NodLista {
	Sph info;
	NL* next;
};


Sph citireSmartphoneDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Sph sph;
	aux = strtok(buffer, sep);
	sph.id = atoi(aux);
	
	aux = strtok(NULL, sep);
	sph.brand = malloc(strlen(aux) + 1);
	strcpy_s(sph.brand, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	sph.model = malloc(strlen(aux) + 1);
	strcpy_s(sph.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	sph.os = malloc(strlen(aux) + 1);
	strcpy_s(sph.os, strlen(aux) + 1, aux);

	sph.ram = atoi(strtok(NULL, sep));
	sph.stocare = atoi(strtok(NULL, sep));
	sph.pret = atof(strtok(NULL, sep));
	sph.stoc = atoi(strtok(NULL, sep));

	return sph;
}

void adaugaSmartphoneInLista(NL** lista, Sph sph) {
	NL* nou = (NL*)malloc(sizeof(NL));
	nou->info = sph;
	nou->next = NULL;

	if (*lista == NULL) {
		*lista = nou;
	}
	else {
		NL* p = *lista;
		while (p->next) p = p->next;
		p->next = nou;
	}
}

void adaugaSmartphoneLaInceputInLista(NL** lista, Sph sph) {
	NL* nou = malloc(sizeof(NL));
	nou->info = sph;
	nou->next = *lista;
	*lista = nou;
}

NL* citireListaSmartphoneDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");

	NL* lista = NULL;

	while (!feof(f)) {
		Sph sph = citireSmartphoneDinFisier(f);
		adaugaSmartphoneInLista(&lista, sph);
	}
	fclose(f);

	return lista;
}

void dezalocareListaSmartphone(NL** lista) {
	while (*lista) {
		free((*lista)->info.brand);
		free((*lista)->info.model);
		free((*lista)->info.os);
		NL* p = *lista;
		*lista = (*lista)->next;
		free(p);
	}
	*lista = NULL;
}

void afisareSmartphone(Sph sph) {
	printf("Id: %d\n", sph.id);
	printf("Brand: %s\n", sph.brand);
	printf("Model: %s\n", sph.model);
	printf("OS: %s\n", sph.os);
	printf("RAM: %d", sph.ram);
	printf("Stocare: %d\n", sph.stocare);
	printf("Pret: %.2f\n", sph.pret);
	printf("Stoc: %d\n\n", sph.stoc);
}


void afisareListaSmartphone(NL* lista) {
	while (lista) {
		afisareSmartphone(lista->info);
		lista = lista->next;
	}
}

void stergeDinListaDupaId(NL** lista, int id) {
	NL* p = *lista;
	NL* prev = NULL;
	while (p) {
		if (p->info.id == id) {
			if (prev) {
				prev->next = p->next;
			}
			else {
				if (p->next) *lista = p->next;
				else {
					free(*lista);
					*lista = NULL;
				}
			}

			NL* aux = p;
			free(p->info.brand);
			free(p->info.model);
			free(p->info.os);
			p = p->next;
			free(aux);
		}
		else {
			prev = p;
			p = p->next;
		}
	}

	
}



int main() {
	NL* lista = citireListaSmartphoneDinFisier("smartphones.txt");

	stergeDinListaDupaId(&lista, 1);
	stergeDinListaDupaId(&lista, 4);
	stergeDinListaDupaId(&lista, 10);

	afisareListaSmartphone(lista);

	

	dezalocareListaSmartphone(&lista);

	return 0;
}