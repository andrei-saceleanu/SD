/* SACELEANU Andrei-Iulian 311CB */
#include "lista2.h"
#include "structures.h"

//inserare element dupa element cu adresa a
int InsDupa(TLDI a, void* x) 
{
	TLDI aux = (TLDI)malloc(sizeof(TCelula2));
	if(!aux) return 0;
	aux->info = x;
	aux->pre = a;
	aux->urm = a->urm;
	a->urm->pre = aux;
	a->urm = aux;
    return 1;
}

//resetare lista dublu inlantuita
void ResetLDI(TLDI s)
{
	TLDI p = s->urm, aux;
	while(p != s)
	{
		aux = p;
		p = p->urm;
		freeTab(aux->info);
		free(aux);
	}
	s->urm = s->pre = s;
}

//distrugere structura de lista
void DistrLDI(TLDI* as)
{
	ResetLDI(*as);
	free(*as);
	*as = NULL;
}

//initializare lista
TLDI InitLDI()
{
	TLDI aux = (TLDI)malloc(sizeof(TCelula2));
    if (!aux) return NULL;

	aux->pre = aux->urm = aux;
    aux->info = NULL;

	return aux;    
}

//afisare lista
void AfisareLDI(TLDI s, void (*afiEl)(void *))
{
	TLDI p;
	printf("Lista: ");
	if(s->urm == s)
	{
		printf("vida\n");
		return;
	}
	for(p = s->urm; p != s; p = p->urm)
		afiEl(p->info);
	printf("\n");
}
