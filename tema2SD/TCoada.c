/* SACELEANU Andrei-Iulian 311CB */
#include "TCoada.h"
#include "TStiva.h"

//initializare coada
TCoada InitQ() {
	TCoada new = (TCoada)malloc(sizeof(TElem));
	if (!new) return NULL;
	*new = NULL;

	return new;
}

//introducere element in coada
void Enqueue(TCoada q, void *x) {
	TElem new = (TElem)malloc(sizeof(TCelula));
	if (!new) return;
	new->info = x;
	new->urm = NULL;

	if(Vida(q)) {
		*q = new;
		return;
	}

	TElem aux = *q;
//identificare capat lista pentru a se insera in coada
	while(aux->urm != NULL)
		aux = aux->urm;

	aux->urm = new;
}

//extragere element din coada
void *Dequeue(TCoada q) {
	if(Vida(q))
		return NULL;
	
	void *result = (*q)->info;
	TElem aux = *q;
	*q = (*q)->urm;
	free(aux);

	return result;
}

//furnizare prim element,fara extragere
void *Peek(TCoada q) {
	if (Vida(q))
		return NULL;
	
	return (*q)->info;
}


//mutare element din source in dest
void MutaQ(TCoada dest, TCoada source) {
	Enqueue(dest, Dequeue(source));
}

//numar elemente din coada
int NrEQ(TCoada q) {
	TCoada aux = InitQ();
	int count = 0;

	while (!Vida(q)) {
		MutaQ(aux, q);
		count++;
	}

	while(!Vida(aux))
		MutaQ(q, aux);
	
	DistrQ(aux);

	return count;	
}

//mutarea tuturor elementelor din source in dest
void ConcatQ(TCoada dest, TCoada source) {
	while(!Vida(source))
		MutaQ(dest, source);
}

//afisare coada
//in functie de continut,se foloseste o anumita functie de afisare afiEl
void AfisareQ(TCoada q, void (*afiEl)(void *)) {
	TCoada aux = InitQ();

	printf("[");
	while (!Vida(q)) {
		void *elm = Dequeue(q);
		afiEl(elm);
		if (!Vida(q))
			printf(", ");
		Enqueue(aux, elm);
	}
	printf("]\n");

	while(!Vida(aux)) {
		Enqueue(q, Dequeue(aux));
	}

	DistrQ(aux);
}

//distrugere coada
void DistrQ(TCoada q) {
	while (!Vida(q))
		free(Dequeue(q));

	free(q);
	q = NULL;
}

//inversare coada cu ajutorul unei stive auxiliare
void InverseazaQ(TCoada q) {
	TStiva aux=InitS();
	while(!Vida(q)){
		void *elm=Dequeue(q);
		Push(aux,elm);
	}
	while(!Vida(aux)){
		void *elm=Top(aux);
		Pop(aux);
		Enqueue(q,elm);
	}
	DistrS(aux);
}

//de size ori,se rotesc circular elementele din coada q
//se aduce de "size" ori inceputul la sfarsit
void FrontToLast(TCoada q,int size){
	if(size<=0)
		return;
	Enqueue(q,Peek(q));
	Dequeue(q);
	FrontToLast(q,size-1);
}

//inserare ordonata a elementului temp in coada q
void pushInQueue(TCoada q,void *temp,int qsize,TFCmp cmp){
	if(Vida(q)||qsize==0){
		Enqueue(q,temp);
		return;
	}
	else if(cmp(temp,Peek(q))>0){
//daca se respecta relatia de comparare,se insereaza elementul la final
//iar prin apelul functiei FrontToLast acesta se aduce la inceputul cozii
		Enqueue(q,temp);
		FrontToLast(q,qsize);
	}else{
//primul element ajunge la sfarsit si se incearca inserarea ordonata
//in restul de qsize-1 elemente din coada 
		Enqueue(q,Peek(q));
		Dequeue(q);
		pushInQueue(q,temp,qsize-1,cmp);
	}

}

//sortare coada in acord cu functia de comparare cmp
void SortareQ(TCoada q,TFCmp cmp){
	if(Vida(q))
		return;
//se extrage un element,se sorteaza restul cozii iar apoi se insereaza
//in pozitia corespunzatoare elementul extras anterior
	void *temp=Peek(q);
	Dequeue(q);
	SortareQ(q,cmp);	
	pushInQueue(q,temp,NrEQ(q),cmp);
}


