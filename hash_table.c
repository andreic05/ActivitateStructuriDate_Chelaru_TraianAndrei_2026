// #define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;
typedef struct HashTable HashTable;
typedef struct Nod Nod;

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod {
    Masina info;
    Nod* next;
};

//creare structura pentru tabela de dispersie
// aceasta este un vector de liste
struct HashTable {
	int dim;
    Nod** vector;
};

Masina initMasina(int id, int nrUsi, float pret, const char* model, const char* numeSofer, unsigned char serie) {
	Masina m;
	m.id = id;
	m.nrUsi = nrUsi;
	m.pret = pret;
	m.model =(char*) malloc(sizeof(char) * (strlen(model) + 1));
	strcpy(m.model, model);
	m.numeSofer = (char*) malloc(sizeof(char) * (strlen(numeSofer) + 1));
	strcpy(m.numeSofer, numeSofer);
	m.serie = serie;

	return m;
}


Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}




HashTable initHashTable(int dimensiune) {
	HashTable ht;
	//initializeaza vectorul de liste si seteaza fiecare lista ca fiind NULL;
    ht.dim = dimensiune;
    ht.vector = (Nod**) malloc(sizeof(Nod*) * dimensiune);
    for (int i=0;i<dimensiune;i++) {
        ht.vector[i] = NULL;
    }

	return ht;
}

int calcHash(int id, int dimensiune) {
	// este calculat hash-ul in functie de dimensiunea tabelei si un atribut al masinii
    return id % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina galerie) {
	//este folosit mecanismul CHAINING
	//este determinata pozitia si se realizeaza inserarea pe pozitia respectiva
    int index = calcHash(galerie.id, hash.dim);
    Nod* nou = (Nod*) malloc(sizeof(Nod));
    nou->info = galerie;
    nou->next = NULL;

    Nod* lista = hash.vector[index];
    if (lista == NULL) {
        hash.vector[index] = nou;
    } else {
        while (lista->next) {
            lista = lista->next;
        }

        lista->next = nou;
    }
}

HashTable citireMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	// aceste masini sunt inserate intr-o tabela de dispersie initializata aici
	//ATENTIE - la final inchidem fisierul/stream-ul
    HashTable hash = initHashTable(4);

    FILE* f = fopen(numeFisier, "r");
    if (f) {
        while (!feof(f)) {
            Masina masina = citireMasinaDinFisier(f);
            inserareMasinaInTabela(hash, masina);
        }

        fclose(f);
    }

    return hash;
}

void afisareTabelaDeMasini(HashTable ht) {
	//sunt afisate toate masinile cu evidentierea clusterelor realizate
    Nod** v = ht.vector;
    for (int i = 0; i < ht.dim; i++) {
        printf("Cluster %d:\n", i);
        if (v[i] == NULL) {
            printf("Empty!\n");
        } else {
            Nod* p = v[i];
            while (p) {
                afisareMasina(p->info);
                p = p->next;
            }
        }
    }
}

void dezalocareTabelaDeMasini(HashTable *ht) {
	//sunt dezalocate toate masinile din tabela de dispersie
	for (int i = 0; i < ht->dim; i++) {
		if (ht->vector[i]) {
			Nod* p = ht->vector[i];
			while (p) {
				free(p->info.model);
				free(p->info.numeSofer);
				Nod* aux = p;
				p = p->next;
				free(aux);
			}
			ht->vector[i] = NULL;
		}
	}

	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	//calculeaza pretul mediu al masinilor din fiecare cluster.
	//trebuie sa returnam un vector cu valorile medii per cluster.
	//lungimea vectorului este data de numarul de clustere care contin masini
	*nrClustere = 0;

	for (int i = 0; i < ht.dim; i++) {
		if (ht.vector[i]) (*nrClustere)++;
	}

	if (*nrClustere != 0) {
		float* pm =(float*) malloc(sizeof(float) * (*nrClustere));
		for (int i = 0; i < ht.dim; i++) {
			if (ht.vector[i]) {
				int sum, count;
				sum = count = 0;
				Nod* p = ht.vector[i];
				while (p) {
					sum += p->info.pret;
					count++;
					p = p->next;
				}
				pm[i] = sum / count;
			}
		}

		return pm;
	}

	return NULL;
}

Masina getMasinaDupaCheie(HashTable ht, int key) {
	Masina m;
	//cauta masina dupa valoarea atributului cheie folosit in calcularea hash-ului
	//trebuie sa modificam numele functiei

	int hash = calcHash(key, ht.dim);
	Nod* p = ht.vector[hash];
	if (p == NULL) return initMasina(-1, 0, 0, "", "", ' ');

	if (p->next == NULL) m = p->info;
	else {
		while (p) {
			if (p->info.id == key) {
				m = initMasina(p->info.id, p->info.nrUsi, p->info.pret, p->info.model, p->info.numeSofer, p->info.serie);
				break;
			}

			p = p->next;
		}

		if (p == NULL) m = initMasina(-1, 0, 0, "", "", ' ');
	}

	return m;
}

int main() {
    HashTable ht = citireMasiniDinFisier("masini.txt");

    afisareTabelaDeMasini(ht);

	int nrClustere;
	float* preturiMedii = calculeazaPreturiMediiPerClustere(ht, &nrClustere);
	for (int i = 0; i< nrClustere; i++) {
		printf("Cluster %d are media preturilor egatala cu %.2f\n", i + 1, preturiMedii[i]);
	}

	Masina m = getMasinaDupaCheie(ht, 7);
	afisareMasina(m);
	free(m.model);
	free(m.numeSofer);
	m = getMasinaDupaCheie(ht, 21);
	afisareMasina(m);
	free(m.model);
	free(m.numeSofer);


	dezalocareTabelaDeMasini(&ht);

	return 0;
}