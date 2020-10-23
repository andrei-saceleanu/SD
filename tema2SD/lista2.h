/* SACELEANU Andrei-Iulian 311CB */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


#ifndef _LISTA_DUBLU_INLANTUITA_
#define _LISTA_DUBLU_INLANTUITA_

typedef struct celula2
{
  void *info;            
  struct celula2 *pre, *urm; 
} TCelula2, *TLDI;

typedef int (*TFCmp)(void*, void*);

TLDI InitLDI();
int InsDupa(TLDI p, void* x);
void* ExtrLDI(TLDI p);
void ResetLDI(TLDI s);
void DistrLDI(TLDI * as);
void AfisareLDI(TLDI s, void (*afiEl)(void *));  
#endif

