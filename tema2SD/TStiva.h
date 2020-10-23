/* SACELEANU Andrei-Iulian 311CB */
#include <stdlib.h>
#include <stdio.h>
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

#ifndef _STIVA_H_
#define _STIVA_H_

TStiva InitS();
void Push(TStiva s, void *x);
void *Pop(TStiva s);
void *Top(TStiva s);
void MutaS(TStiva dest, TStiva source);
int NrES(TStiva s);
void AfisareS(TStiva s, void (*afiEl)(void *));
void DistrS(TStiva s);
void Clear(TStiva s);
void InverseazaS(TStiva s);
void SortareS(TStiva s, TFCmp cmp);

#endif 
