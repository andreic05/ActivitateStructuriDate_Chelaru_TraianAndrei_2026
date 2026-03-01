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
}

void afisareSmartphone(Sph sph) {
	printf("Id: %d\n", sph.id);
	printf("Model: %s\n\n", sph.model);
}

void afisareListaSmartphone(NL* lista) {
	while (lista) {
		afisareSmartphone(lista->info);
		lista = lista->next;
	}
}



int main() {
	NL* lista = citireListaSmartphoneDinFisier("smartphones.txt");

	afisareListaSmartphone(lista);

	dezalocareListaSmartphone(&lista);

	return 0;
}