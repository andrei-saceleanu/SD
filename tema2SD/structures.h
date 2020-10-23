/* SACELEANU Andrei-Iulian 311CB */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "TStiva.h"
#include "lista2.h"
#include "utils.h"

#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

//definire structuri de tab si pagina web
typedef struct{
	char url[21];
	int num_res;
	Resource *resources;
}Webpage;

typedef struct{
	Webpage *current_page;
	TStiva back_stack,forward_stack;
}Tab;

//functii utilizate
void newtab(TLDI s,TLDI *current_tab);
void deltab(TLDI s,TLDI *current_tab);
void change_tab(TLDI s,int tab_index,TLDI *current_tab);
void print_open_tabs(TLDI s,FILE *output);
void Goto(char url[21],TLDI current_tab);
void Back(TLDI current_tab,FILE *output);
void Forward(TLDI current_tab,FILE *output);
void freeTab(void *tab);
void List_downloads(TLDI current_tab,FILE *output);
#endif
