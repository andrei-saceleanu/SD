/* SACELEANU Andrei-Iulian 311CB */
#include "structures.h"
#include "utils.h"
#include "TCoada.h"

//lungimea maxima a unei linii/comenzi
#define MAX_CMD_LEN 256



//adauga la istoricul global pagina la care se ajunge prin comanda goto        
void Construct_history(TCoada global_history,char url_name[21]){
	char *url=strdup(url_name);
	if(!url)
		return;
	Enqueue(global_history,(void *)url);
}

//afisarea istoricului global al browserului
void Show_history(TCoada global_history,FILE *output){
	TCoada aux=InitQ();

//se extrage cate un element din coada,se afiseaza,apoi este introdus in
//coada auxiliara pentru reconstructia global_history
	while(!Vida(global_history)){
		void *current=Dequeue(global_history);
		fprintf(output,"%s\n",(char *)current);
		Enqueue(aux,current);
	}
	while(!Vida(aux)){
		Enqueue(global_history,Peek(aux));
		Dequeue(aux);
	}
	DistrQ(aux);
}

//se sterg un numar nr de elemente din istoricul global
void Delete_entries(TCoada global_history,int nr){
	if(nr==0){//daca este furnizat 0,se sterg toate elementele din coada
		while (!Vida(global_history))
			free(Dequeue(global_history));
	}else{//in caz contrar,se sterg elemente cat timp indexul curent
//nu depaseste nr dat ca parametru
		int index=0;
		while(index<nr){
			free(Dequeue(global_history));
			index++;
		}
	}
}

//afisarea cozii de prioritati a descarcarilor impreuna cu lista de 
//descarcari finalizate down_finished(stocata ca o coada)
void Print_downloads(TCoada downloads,TCoada down_finished,FILE *output){
	TCoada aux=InitQ();
//se extrage cate un element,se afiseaza corespunzator si se introduce in aux
//pentru reconstructia downloads
	while(!Vida(downloads)){
		void *elm=Dequeue(downloads);
		fprintf(output,"[\"%s\" : %lu/%lu]\n",((Resource *)elm)->name,
((Resource *)elm)->dimension-((Resource *)elm)->currently_downloaded,((Resource *)elm)->dimension);
		Enqueue(aux,elm);
	}
	while(!Vida(aux)){
		Enqueue(downloads,Peek(aux));
		Dequeue(aux);
	}
//se aplica acelasi procedeu pentru down_finished
	while(!Vida(down_finished)){
		void *elm=Dequeue(down_finished);
		fprintf(output,"[\"%s\" : completed]\n",((Resource *)elm)->name);
		Enqueue(aux,elm);
	}
	while(!Vida(aux)){
		Enqueue(down_finished,Peek(aux));
		Dequeue(aux);
	}
	DistrQ(aux);
}

//se creeaza o copie a resursei original pentru a fi inserata 
//ulterior in downloads
Resource *copy_resource(Resource *original){
	Resource *new=(Resource *)malloc(sizeof(Resource));
	if(!new)
		return NULL;
	strcpy(new->name,original->name);
	new->dimension=original->dimension;
	new->currently_downloaded=0;
	return new;
}

//functie de comparare a resurselor pentru sortarea cozii de prioritati
//atunci cand se introduce o noua resursa
int cmpResource(void *a,void *b){
	int dload1=((Resource *)a)->currently_downloaded;
	int dload2=((Resource *)b)->currently_downloaded;
	int size1=((Resource *)a)->dimension;
	int size2=((Resource *)b)->dimension;
	int rem1=size1-dload1;
	int rem2=size2-dload2;
	if(rem1!=rem2){//sortare crescatoare dupa remaining size
		return rem1<rem2;
	}else{//daca aceasta dimensiune este egala,sorteaza dupa prioritate/
//cea mai recenta resursa descarcata la inceput
		return ((Resource *)a)->priority>((Resource *)b)->priority;
	}
}

//se adauga la coada de descarcari resursa de indice index a paginii web
//din tabul curent,daca pagina exista si resursa nu este vida
void Download_resource(TLDI current_tab,int index,TCoada downloads,int priority){
	if(((Tab *)current_tab->info)->current_page){
	if(((Tab *)current_tab->info)->current_page->resources[index].dimension!=0){
	Resource *original=&((Tab *)current_tab->info)->current_page->resources[index];
	Resource *copy=copy_resource(original);
	copy->priority=priority;
	if(!copy)
		return;
	Enqueue(downloads,(void *)copy);//se adauga la coada
	SortareQ(downloads,cmpResource);//se reface proprietatea ca
//elementele sa fie sortate crescator dupa dimensiunea din resursa care mai
//trebuie descarcata
	}
	}
}

//actualizarea descarcarilor in urma trecerii simulate a timpului
//cu o secunda in situatia comenzii goto,sau cu seconds oarecare in cazul
//comenzii wait

void Update_downloads(TCoada downloads,TCoada down_finished,int seconds,
int bandwidth){
	unsigned long total_bytes=seconds*bandwidth;//capacitate de descarcare
//curenta
	while(total_bytes>0){//cat timp mai e informatie "pe cablu"
		if(Vida(downloads))
			break;

//daca informatia disponibila este mai mica decat necesarul de finalizare
//al primului element din coada,aceasta se consuma in totalitate iar 
//dimensiunea descarcata a primului element creste

		if(total_bytes<((Resource *)Peek(downloads))->dimension-
((Resource *)Peek(downloads))->currently_downloaded){
			((Resource *)Peek(downloads))->currently_downloaded+=total_bytes;
			total_bytes=0;
		}else{

//in caz contrar,cand total_bytes acopera intreg necesarul primului element,
//se scade dimensiunea ramasa de descarcat a acestuia din total_bytes
//iar resursa trece din coada de prioritati in lista descarcarilor finalizate

			total_bytes-=((Resource *)Peek(downloads))->dimension-
((Resource *)Peek(downloads))->currently_downloaded;
			((Resource *)Peek(downloads))->currently_downloaded=((Resource *)Peek(downloads))->dimension;
			void *elm=Dequeue(downloads);
			Enqueue(down_finished,elm);
		}
		
	}
}


int main(int argc,char **argv){
//initializare lista de taburi s,istoric global,coada de prioritati si lista
//de descarcari finalizate
	TLDI s=InitLDI();
	TCoada global_history=InitQ();
	TCoada downloads=InitQ();
	TCoada down_finished=InitQ();
	TLDI current_tab=s;
//conform cerintei,browserul are initial un tab deschis gol
	newtab(s,&current_tab);

//initializare valoare implicita latime de banda
	int bandwidth=1024;
	int priority=0;//contor prioritate descarcari
//deschidere fisiere si verificare
	FILE *input=fopen(argv[1],"rt");
	if(!input){
		printf("Eroare deschidere input");
	}
	FILE *output=fopen(argv[2],"wt");
	if(!output){
		fclose(input);
		printf("Eroare deschidere output");
	}
//se citeste cate o comanda din fisierul de intrare
//si se proceseaza corespunzator,apelandu-se functia asociata
	char command[MAX_CMD_LEN];
	while(fgets(command,MAX_CMD_LEN,input)!=NULL){
		if(command[strlen(command)-1]=='\n'){
			command[strlen(command)-1]='\0';
		}
		if(strstr(command,"set_band")-command==0){
			char *p=strchr(command,' ')+1;
			bandwidth=atoi(p);
		}else if(strstr(command,"newtab")-command==0){
			newtab(s,&current_tab);
		}else if(strstr(command,"deltab")-command==0){
			deltab(s,&current_tab);
		}else if(strstr(command,"change_tab")-command==0){
			char *p=strchr(command,' ')+1;
			int tab_index=atoi(p);
			change_tab(s,tab_index,&current_tab);
		}else if(strstr(command,"print_open_tabs")-command==0){
			print_open_tabs(s,output);
		}else if(strstr(command,"goto")-command==0){
			char *url=strchr(command,' ')+1;//identificare parametru
			char url_name[21]="init";
			strcpy(url_name,url);
			Construct_history(global_history,url_name);
			Update_downloads(downloads,down_finished,1,bandwidth);
			Goto(url_name,current_tab);
		}else if(strstr(command,"back")-command==0){
			Back(current_tab,output);
		}else if(strstr(command,"forward")-command==0){
			Forward(current_tab,output);
		}else if(strstr(command,"history")-command==0){
			Show_history(global_history,output);
		}else if(strstr(command,"del_history")-command==0){
			char *nr_entries=strchr(command,' ')+1;
			int nr=atoi(nr_entries);//conversia parametrului la tipul dorit
			Delete_entries(global_history,nr);
		}else if(strstr(command,"list_dl")-command==0){
			List_downloads(current_tab,output);
		}else if(strstr(command,"downloads")-command==0){
			Print_downloads(downloads,down_finished,output);
		}else if(strstr(command,"download")-command==0){
			char *ind=strchr(command,' ')+1;
			int index=atoi(ind);
			Download_resource(current_tab,index,downloads,priority);
			priority++;
		}else if(strstr(command,"wait")-command==0){
			char *time=strchr(command,' ')+1;
			int seconds=atoi(time);
			Update_downloads(downloads,down_finished,seconds,bandwidth);
		}
	}
//dealocare structuri de reprezentare a browserului si inchiderea fisierelor
	DistrQ(global_history);
	DistrQ(downloads);
	DistrQ(down_finished);
	DistrLDI(&s);
	fclose(input);
	fclose(output);
	return 0;
}
