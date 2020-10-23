/* SACELEANU Andrei-Iulian 311CB */
#include "TStiva.h"
#include "TCoada.h"

//initializare stiva
TStiva InitS() {
	TStiva new = (TStiva)malloc(sizeof(TElem));
	if (!new) return NULL;
	*new = NULL;

	return new;
}

//introducere element in stiva
void Push(TStiva s, void *x) {
	TElem new = (TElem)malloc(sizeof(TCelula));
	if (!new) return; 
	new->info = x;
	new->urm = NULL;

	if(Vida(s)) {
		*s = new;
		return;
	}
//adaugarea noului element la inceputul listei/varful stivei
	new->urm = *s;
	*s = new;
}

//extragere element din varful stivei
void *Pop(TStiva s) {
	if(Vida(s))
		return NULL;
	
	void *result = (*s)->info;
	TElem aux = *s;
	*s = (*s)->urm;
	free(aux);

	return result;
}

//furnizare varf,fara extragere
void *Top(TStiva s) {
	if (Vida(s))
		return NULL;
	
	return (*s)->info;
}

//muta element din stiva source in stiva dest
void MutaS(TStiva dest, TStiva source) {
	Push(dest, Pop(source));
}

//numar de elemente din stiva
int NrES(TStiva s) {
	TStiva aux = InitS();
	int count = 0;
//extragere secventiala si numarare,urmata de reconstructie cu ajutorul
//stivei auxiliare
	while (!Vida(s)) {
		MutaS(aux, s);
		count++;
	}

	while(!Vida(aux))
		MutaS(s, aux);

	return count;	
}

//rastoarna elementele din source in dest
void Rastoarna(TStiva dest,TStiva source){
	while(!Vida(source)){
		MutaS(dest,Pop(source));
	}
}

//cu ajutorul unei stive auxiliare,se rastoarna de doua ori elementele
//din source pentru ca in aux acestea sa isi pastreze ordinea relativa
void Suprapune(TStiva dest,TStiva source){
	TStiva aux=InitS();
	while(!Vida(source)){
		MutaS(aux,Pop(source));
	}
	while(!Vida(aux)){
		MutaS(dest,Pop(aux));
	}
	DistrS(aux);
}

//afisare stiva
//in functie de continut,se foloseste o anumita functie de afisare afiEl
void AfisareS(TStiva s, void (*afiEl)(void *)) {
	TStiva aux = InitS();

	printf("[");
	while (!Vida(s)) {
		void *elm = Pop(s);
		afiEl(elm);
		if (!Vida(s))
			printf(", ");
		Push(aux, elm);
	}
	printf("]\n");

	while(!Vida(aux))
		MutaS(s, aux);

	DistrS(aux);
}

//distrugere stiva
void DistrS(TStiva s) {
		while (!Vida(s))
			free(Pop(s));
	free(s);
	s = NULL;
}

//inversare stiva folosind o coada auxiliara
void InverseazaS(TStiva s) {
	TCoada aux=InitQ();
	while(!Vida(s)){
		void *elm=Top(s);
		Pop(s);
		Enqueue(aux,elm);
	}
	while(!Vida(aux)){
		void *elm=Peek(aux);
		Dequeue(aux);
		Push(s,elm);
	}
	DistrQ(aux);
}


//golire stiva
void Clear(TStiva s){
	while(!Vida(s))
		Pop(s);
}
