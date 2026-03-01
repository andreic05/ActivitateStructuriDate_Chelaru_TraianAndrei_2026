#include<stdio.h>
#include<malloc.h>

typedef struct Depozit Depozit;

struct Depozit {
	int id;
	float suprafata;
	int volum;
	char* denumire;
	char clasa;
};
struct Depozit initializare(int id, float suprafata, int volum, char* denumire, char clasa) {
	Depozit s;
	s.id = id;
	s.suprafata = suprafata;
	s.volum = volum;
	s.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy(s.denumire, denumire);
	s.clasa = clasa;
	return s;
}

struct Depozit copiaza(Depozit d) {
	return initializare(d.id, d.suprafata, d.volum, d.denumire, d.clasa);
	
}

void afisare(struct Depozit s) {
	printf("%d. Depositul %s de clasa %c are o suprafata de %5.2f si un volum de %d \n", s.id, s.denumire, s.clasa, s.suprafata, s.volum);
}


void afisareVector(struct Depozit* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);

		//afisare(*vector);
		//vector++;

		//afisare(*(vector + i);
	}
}

struct Depozit* copiazaPrimeleNElemente(struct Depozit* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
	if (nrElementeCopiate < nrElemente) {
		struct Depozit* vectorNou = (Depozit*) malloc(sizeof(Depozit) * nrElementeCopiate);
		for (int i = 0; i < nrElementeCopiate; i++) {
			vectorNou[i] = copiaza(vector[i]);
		}

		return vectorNou;
	}
	else {
		return NULL;
	}
}

void dezalocare(struct Depozit** vector, int* nrElemente) {
	//dezalocam elementele din vector si vectorul
	for (int i = 0; i < *nrElemente; i++) {
		free((*vector)[i].denumire);
	}

	free(*vector);
	*nrElemente = 0;
	*vector = NULL;
}
//
void copiazaAnumiteElemente(struct Depozit* vector, char nrElemente, float prag, struct Depozit** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie

	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].suprafata < prag) (*dimensiune)++;
	}

	*vectorNou = (Depozit*)malloc(sizeof(Depozit) * (*dimensiune));
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].suprafata < prag) (*vectorNou)[(*dimensiune)++] = copiaza(vector[i]);
	}
}

struct Depozit getPrimulElementConditionat(struct Depozit* vector, int nrElemente, const char* numeCautat) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Depozit s;
	s.id = -1;
	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(numeCautat, vector[i].denumire) == 0) {
			s = copiaza(vector[i]);
		}
	}

	return s;
}
//	
//
//
int main() {
	struct Depozit depozit = initializare(1, 34.3, 70, "Emag", 'A');

	afisare(depozit);

	int nrDepozite = 4;
	Depozit* vector = (Depozit*)malloc(sizeof(Depozit) * nrDepozite);
	vector[0] = initializare(2, 56, 45, "DPD", 'B');
	vector[1] = initializare(3, 45, 89, "Amazon", 'A');
	vector[2] = initializare(4, 34, 67, "DSC", 'A');
	vector[3] = copiaza(depozit);

	afisareVector(vector, nrDepozite);

	printf("\n \n");
	int nrCopiate = 2;
	Depozit* scurt = copiazaPrimeleNElemente(vector, nrDepozite, nrCopiate);

	afisareVector(scurt, nrCopiate);

	dezalocare(&scurt, &nrCopiate);

	afisareVector(scurt, nrCopiate);

	printf("\n \n");
	Depozit* filtrat = NULL; 
	int dimensiune = 0;
	copiazaAnumiteElemente(vector, nrDepozite, 50, &filtrat, &dimensiune);
	afisareVector(filtrat, dimensiune);
	dezalocare(&filtrat, &dimensiune);

	printf("\n \n");
	Depozit depozitCautat = getPrimulElementConditionat(vector, nrDepozite, "Emag");
	afisare(depozitCautat);

	dezalocare(&vector, &nrDepozite);
	free(depozit.denumire);

	return 0;
}

// deplasare + dereferentiere inseamna indexare *(p+2) este p[2]