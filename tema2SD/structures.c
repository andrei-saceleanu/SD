/* SACELEANU Andrei-Iulian 311CB */
#include "structures.h"
#include "utils.h"

//deschidere tab si mutare indicator de tab curent la finalul listei
//adica la noul tab deschis
void newtab(TLDI s,TLDI *current_tab){
//alocare structura
	Tab *new=(Tab *)malloc(sizeof(Tab));
	if(!new)
		return;
	new->back_stack=InitS();
	new->forward_stack=InitS();
	new->current_page=NULL;
//inserare la finalul listei,adica dupa s->pre
	int ok=InsDupa(s->pre,new);
	if(!ok){
		while(!Vida(new->forward_stack)){
			void *elm=Pop(new->forward_stack);
			free(((Webpage *)elm)->resources);
			free(elm);
		}
		while(!Vida(new->back_stack)){
			void *elm=Pop(new->back_stack);
			free(((Webpage *)elm)->resources);
			free(elm);
		}
		free(new);
	}
//actualizare indicator de tab curent
	*current_tab=s->pre;
}

//dealocare tab
void freeTab(void *tab){
	Tab *t=(Tab *)tab;
	//golire si dealocare stive de back si forward
	while(!Vida(t->forward_stack)){
		void *elm=Pop(t->forward_stack);
		free(((Webpage *)elm)->resources);
		free(elm);
	}
	free(t->forward_stack);
	while(!Vida(t->back_stack)){
		void *elm=Pop(t->back_stack);
		free(((Webpage *)elm)->resources);
		free(elm);
	}
	free(t->back_stack);
//daca pagina nu e goala,dealocare resurse si structura pagina
	if(t->current_page){
		free(t->current_page->resources);
	}
	free(t->current_page);
	free(t);
}

//stergere tab din lista
void deltab(TLDI s,TLDI *current_tab){
	TLDI delete=s->pre;
//daca tabul curent este ultimul si vrem sa stergem,mutam intai indicatorul
//cu o pozitie in urma
	if(*current_tab==delete){
		*current_tab=delete->pre;
	}
//actualizare legaturi din lista
	delete->pre->urm=s;
	s->pre=delete->pre;
//dealocare
	freeTab(delete->info);
	free(delete);
}

//mutare indicator de tab curent la tabul de index tab_index
void change_tab(TLDI s,int tab_index,TLDI *current_tab){
	TLDI p=s->urm;
	int index=0;
	while(index<tab_index){
		p=p->urm;
		index++;
	}
	*current_tab=p;
}

//parcurgere lista de taburi si afisarea paginii deschise in fiecare
//dintre acestea
void print_open_tabs(TLDI s,FILE *output){
	TLDI p=s->urm;
	int index=0;
	while(p!=s){
		Webpage *page=((Tab *)p->info)->current_page;
		if(!page){
			fprintf(output,"(%d: empty)\n",index);
		}else{
			fprintf(output,"(%d: %s)\n",index,page->url);
		}
		index++;
		p=p->urm;
	}
}

//navigare catre o noua pagina in tabul curent
//se creeaza pagina cu numele dat ca parametru si resursele aferente
void Goto(char url[21],TLDI current_tab){
	Webpage *new=(Webpage *)malloc(sizeof(Webpage));
	if(!new)
		return;
	new->resources=get_page_resources(url,&new->num_res);
	if(!new->resources){
		free(new);
		return;
	}
	strcpy(new->url,url);
	Webpage *page=((Tab *)current_tab->info)->current_page;
//daca exista deja o pagina deschisa in tab,aceasta intra in stiva de back
	if(page)
		Push(((Tab *)current_tab->info)->back_stack,page);
//actualizare pagina curenta si golire stiva de forward
	((Tab *)current_tab->info)->current_page=new;
	Clear(((Tab *)current_tab->info)->forward_stack);

}

//navigare catre pagina anterioara din tab,daca este posibil
void Back(TLDI current_tab,FILE *output){
	Tab *tab=(Tab *)current_tab->info;
	if(Vida(tab->back_stack)){
		fprintf(output,"can't go back, no pages in stack\n");
		return;
	}
//daca este posibil,pagina curenta intra in forward,actualizandu-se apoi
//cu varful din back
	Push(tab->forward_stack,tab->current_page);
	tab->current_page=(Webpage *)Pop(tab->back_stack);
}

//navigare catre pagina urmatoare din tab,daca exista
void Forward(TLDI current_tab,FILE *output){
	Tab *tab=(Tab *)current_tab->info;
	if(Vida(tab->forward_stack)){
		fprintf(output,"can't go forward, no pages in stack\n");
		return;
	}
//pagina curenta este salvata in back,actualizandu-se apoi la varful stivei
//de forward
	Push(tab->back_stack,tab->current_page);
	tab->current_page=(Webpage *)Pop(tab->forward_stack);
}

//afisare resurse descarcabile din pagina deschisa in tabul curent
void List_downloads(TLDI current_tab,FILE *output){
	Webpage *current_page=((Tab *)current_tab->info)->current_page;
	if(current_page){//daca pagina din tabul curent nu este goala
//se afiseaza resursele
	int i;
	for(i=0;i<current_page->num_res;i++){
		fprintf(output,"[%d - \"%s\" : %lu]\n",i,current_page->resources[i].name,
current_page->resources[i].dimension);
	}
	}
}


