/* SACELEANU Andrei-Iulian 311CB */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


#ifndef _ELEMENT_
#define _ELEMENT_


#define Vida(T) ((*T) == NULL)

typedef int (*TFCmp)(void*, void*);

typedef struct celula
{
	void *info;					
	struct celula *urm;		
} TCelula, *TElem, **TStiva, **TCoada;

#endif 


#ifndef _COADA_H_
#define _COADA_H_


TCoada InitQ();
void Enqueue(TCoada q, void *x);
void *Dequeue(TCoada q);
void *Peek(TCoada q);
void MutaQ(TCoada dest, TCoada source);
int NrEQ(TCoada q);
void ConcatQ(TCoada dest, TCoada source);
void AfisareQ(TCoada q, void (*afiEl)(void *));
void DistrQ(TCoada q);
void InverseazaQ(TCoada q);
void SortareQ(TCoada q, TFCmp cmp);

#endif 

