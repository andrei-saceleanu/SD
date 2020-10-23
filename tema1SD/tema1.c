/* SACELEANU Andrei-Iulian -311CB*/
#include "structures.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

//lungimea maxima pentru valoare de la delete sau search
#define MAX_VALUE_LEN 255 
//numarul maxim de coloane dintr-un tabel
#define MAX_COLUMNS 400
//precizia de comparatie si afisare a variabilelor de tip float
#define EPSILON 0.0000001

//alocare celula de continut pentru linii cu numere intregi
t_intCell *alocCelulaInt(int value){
	t_intCell *cell=(t_intCell *)malloc(sizeof(t_intCell));
	if(!cell)
		return NULL;
	cell->next=NULL;
	cell->value=value;
	return cell;
}

//alocare linie cu numere intregi
t_intLine *alocInt(char line_content[][MAX_VALUE_LEN],int count){
	t_intLine *line=(t_intLine *)malloc(sizeof(t_intLine));
	if(!line)
		return NULL;
	line->next=NULL;
	int i;
	line->cells=NULL;
	t_intCell *ultim=NULL;
	for(i=0;i<count;i++){
		t_intCell *aux=alocCelulaInt(atoi(line_content[i]));
		if(!aux)
			return NULL;
		if(!line->cells){//celula nou alocata aux va fi adaugata mereu la final
			line->cells=aux;
			ultim=aux;
		}else{
			ultim->next=aux;
			ultim=aux;
		}
	}
	return line;
}

//alocare celula de continut pentru linii cu numere reale
t_floatCell *alocCelulaFloat(float value){
	t_floatCell *cell=(t_floatCell *)malloc(sizeof(t_floatCell));
	if(!cell)
		return NULL;
	cell->next=NULL;
	cell->value=value;
	return cell;
}

//alocare linie cu numere reale
t_floatLine *alocFloat(char line_content[][MAX_VALUE_LEN],int count){
	t_floatLine *line=(t_floatLine *)malloc(sizeof(t_floatLine));
	if(!line)
		return NULL;
	line->next=NULL;
	int i;
	line->cells=NULL;
	t_floatCell *ultim=NULL;
	for(i=0;i<count;i++){
		t_floatCell *aux=alocCelulaFloat(atof(line_content[i]));
		if(!aux)
			return NULL;
		if(!line->cells){
			line->cells=aux;
			ultim=aux;
		}else{
			ultim->next=aux;
			ultim=aux;
		}
	}
	return line;
}

//alocare celula de continut pentru linii cu siruri de caractere
t_stringCell *alocCelulaString(char *value){
	t_stringCell *cell=(t_stringCell *)malloc(sizeof(t_stringCell));
	if(!cell)
		return NULL;
	cell->next=NULL;
	cell->value=strdup(value);
	if(!cell->value){
		free(cell);
		return NULL;
	}
	return cell;
}

//alocare linie cu siruri de caractere
t_stringLine *alocString(char line_content[][MAX_VALUE_LEN],int count){
	t_stringLine *line=(t_stringLine *)malloc(sizeof(t_stringLine));
	if(!line)
		return NULL;
	line->next=NULL;
	int i;
	line->cells=NULL;
	t_stringCell *ultim=NULL;
	for(i=0;i<count;i++){
		t_stringCell *aux=alocCelulaString(line_content[i]);
		if(!aux)
			return NULL;
		if(!line->cells){
			line->cells=aux;
			ultim=aux;
		}else{
			ultim->next=aux;
			ultim=aux;
		}
	}
	return line;
}

//dealocare linie cu numere intregi,** pentru vizibilitate in afara functiei
void freeInt(t_intLine **line){
		t_intCell *aux=(*line)->cells;
		t_intCell *elim=aux;
		while(aux){
			elim=aux;
			aux=aux->next;
			free(elim);
		}
		free(*line);
}

//dealocare linie cu numere reale
void freeFloat(t_floatLine **line){
		t_floatCell *aux=(*line)->cells;
		t_floatCell *elim=aux;
		while(aux){
			elim=aux;
			aux=aux->next;
			free(elim);
		}
		free(*line);
}

//dealocare linie cu siruri de caractere
void freeString(t_stringLine **line){
		t_stringCell *aux=(*line)->cells;
		t_stringCell *elim=aux;
		while(aux){
			elim=aux;
			aux=aux->next;
			free(elim->value);//se dezaloca si continutul obtinut cu strdup
			free(elim);
		}
		free(*line);
}

//afisare linie cu numere intregi
void printLineInt(t_intCell *head){
	while(head){
		printf("%-*d",MAX_COLUMN_NAME_LEN,head->value);
		printf(" ");
		if(head->next==NULL)
			break;
		head=head->next;
	}
	printf("\n");
}

//afisare linie cu numere reale
void printLineFloat(t_floatCell *head){
	while(head){
		printf("%-*.6f",MAX_COLUMN_NAME_LEN,head->value);
		printf(" ");
		if(head->next==NULL)
			break;
		head=head->next;
	}
	printf("\n");
}


//afisare linie cu siruri de caractere
void printLineString(t_stringCell *head){
	while(head){
		printf("%-*s",MAX_COLUMN_NAME_LEN,head->value);
		printf(" ");
		if(head->next==NULL)
			break;
		
		head=head->next;
	}
	printf("\n");
}

//verifica respectarea relatiei intre intregi pentru functiile delete si search
int relationInt(int db_value,int given_value,char relation[3]){
	if(strcmp(relation,"<")==0)
		return (db_value<given_value);
	else if(strcmp(relation,"<=")==0)
		return (db_value<=given_value);
	else if(strcmp(relation,"==")==0)
		return (db_value==given_value);
	else if(strcmp(relation,"!=")==0)
		return (db_value!=given_value);
	else if(strcmp(relation,">=")==0)
		return (db_value>=given_value);
	else
		return (db_value>given_value);
}

//verifica respectarea relatiei intre siruri pentru functiile delete si search
int relationString(char* db_value,char* given_value,char relation[3]){
	if(strcmp(relation,"<")==0)
		return strcmp(db_value,given_value)<0;
	else if(strcmp(relation,"<=")==0)
		return strcmp(db_value,given_value)<=0;
	else if(strcmp(relation,"==")==0)
		return strcmp(db_value,given_value)==0;
	else if(strcmp(relation,"!=")==0)
		return strcmp(db_value,given_value)!=0;
	else if(strcmp(relation,">=")==0)
		return strcmp(db_value,given_value)>=0;
	else
		return strcmp(db_value,given_value)>0;
}

//verifica respectarea relatiei intre floats pentru functiile delete si search
int relationFloat(float db_value,float given_value,char relation[3]){
	if(strcmp(relation,"<")==0)
		return (db_value<given_value);
	else if(strcmp(relation,"<=")==0)
		return (db_value<=given_value);
	else if(strcmp(relation,"==")==0)
		return fabs(db_value-given_value)<EPSILON;//evitare round-off error
	else if(strcmp(relation,"!=")==0)
		return fabs(db_value-given_value)>=EPSILON;
	else if(strcmp(relation,">=")==0)
		return (db_value>=given_value);
	else
		return (db_value>given_value);
}

//alocare coloana tabel
t_column *aloc_column(char *name){
	t_column *col=(t_column *)malloc(sizeof(t_column));
	if(!col)
		return NULL;
	strcpy(col->name,name);
	col->next=NULL;
	return col;
}


//initializare baza de date
t_db *InitDB(char *nume){
	t_db *db=(t_db *)malloc(sizeof(t_db));
	if(!db)
		return NULL;
	strcpy(db->name,nume);
	db->tables=NULL;
	return db;
}


//dealocare tabel
void dealloc_Table(t_table **p){
//in functie de tipul stocat in tabelul de dealocat,se obtine inceputul aferent
//lineint,linefloat sau linestring
		t_cellType current_type=(*p)->type;
		t_intLine *lineint=NULL;
		t_floatLine *linefloat=NULL;
		t_stringLine *linestring=NULL;
		switch(current_type){
			case INT:
				lineint=(t_intLine *)(*p)->lines;
				break;
			case FLOAT:
				linefloat=(t_floatLine *)(*p)->lines;
				break;
			case STRING:
				linestring=(t_stringLine *)(*p)->lines;
				break;
			default:
				break;
		}	
//in functie de tipul ales,maxim o bucla se va executa si va dealoca fiecare
//linie
		for(;lineint!=NULL;){
			t_intLine *aux2=lineint;
			lineint=lineint->next;
			freeInt(&aux2);
		}
		for(;linefloat!=NULL;){
			t_floatLine *aux2=linefloat;
			linefloat=linefloat->next;
			freeFloat(&aux2);
		}
		for(;linestring!=NULL;){
			t_stringLine *aux2=linestring;
			linestring=linestring->next;
			freeString(&aux2);
		}

//dealocare coloane
		t_column *aux3=(*p)->columns;
		
		while(aux3){
			t_column *m=aux3;
			aux3=aux3->next;
			free(m);
		}
//dealocare structura de t_tabel
		free(*p);

}

//dealocare baza de date,se dealoca secvential fiecare tabel,apoi intreaga
//structura
void DistrugeDB(t_db **db){
	
	t_table *p=NULL;
	p=(*db)->tables;
	for(;p!=NULL;){
		t_table *current=p;
		p=p->next;
		dealloc_Table(&current);
	}
	free(*db);
}

//verifica existenta unui tabel cu numele tabel_name in baza de date db
int has_Table(t_db *db,char *table_name){
	t_table *p=db->tables;
	while(p){
		if(strcmp(p->name,table_name)==0)
			return 1;
		p=p->next;
	}
	return 0;
}

//alocare tabel si introducere in baza de date
void Create_Table(t_db *db,char *table_name,t_cellType type,
char columns[][MAX_COLUMN_NAME_LEN],int count_cols){
	t_table *new=(t_table *)calloc(sizeof(t_table),1);
	if(!new)
		return;
	new->type=type;
	strcpy(new->name,table_name);
	new->lines=NULL;
	new->columns=NULL;
	t_column *last_column=NULL;
	int i;
//alocare si constructie coloane
	for(i=0;i<count_cols;i++){
		t_column *aux=aloc_column(columns[i]);
		if(!aux){
			free(new);
			return;
		}
		if(!new->columns){
			new->columns=aux;
			last_column=aux;
		}else{
			last_column->next=aux;
			last_column=aux;
		}
	}
//gasire ultim tabel din baza de date si adaugare dupa acesta
	t_table *last_table=NULL;
	if(!db->tables){
		db->tables=new;
	}else{
		last_table=db->tables;
		while(last_table->next){
			last_table=last_table->next;
		}
		last_table->next=new;
	}		
}



//stergere si dealocare tabel
void Delete_Table(t_db *db,char *table_name){
	t_table *p=db->tables;
	t_table *prev=NULL;
//gasire tabel cu anterior
	while(p){
		if(strcmp(p->name,table_name)==0)
			break;
		prev=p;
		p=p->next;
	}
//daca anterior(prev)==NULL,inseamna ca se sterge primul tabel
//inceputul listei de tabele puncteaza la urmatorul tabel
	if(!prev){
		db->tables=p->next;
	}else{
		prev->next=p->next;
	}
//dealocare tabel scos din baza de date
	dealloc_Table(&p);
}


//afisare antet de tabel(titlu,coloane si separatorii cu ----)
void print_header(t_db *db,char *table_name){
	t_table *p=db->tables;
	while(p){
		if(strcmp(p->name,table_name)==0)
			break;
		p=p->next;
	}
	printf("TABLE: %s\n",table_name);
	t_column *current_column=p->columns;
	int count=0;
	while(current_column){
		printf("%-*s",MAX_COLUMN_NAME_LEN,current_column->name);
		count++;
		printf(" ");
		if(current_column->next==NULL)
			break;
		
		current_column=current_column->next;
	}
	printf("\n");
	char separator[MAX_COLUMN_NAME_LEN+1]="init";
	memset(separator,'-',MAX_COLUMN_NAME_LEN);
	int i;
	for(i=1;i<=count;i++){
		printf("%-*s",MAX_COLUMN_NAME_LEN,separator);
			printf(" ");
	}
	printf("\n");


}

//afisare tabel
void Print_table(t_db *db,char *table_name){
	t_table *p=db->tables;
	while(p){
		if(strcmp(p->name,table_name)==0)
			break;
		p=p->next;
	}
	printf("TABLE: %s\n",table_name);
	t_column *current_column=p->columns;
	int count=0;
	while(current_column){
		printf("%-*s",MAX_COLUMN_NAME_LEN,current_column->name);
		count++;
		printf(" ");
		if(current_column->next==NULL)
			break;
		
		current_column=current_column->next;
	}
	printf("\n");
	char separator[MAX_COLUMN_NAME_LEN+1]="init";
	memset(separator,'-',MAX_COLUMN_NAME_LEN);
	int i;
	for(i=1;i<=count;i++){
		printf("%-*s",MAX_COLUMN_NAME_LEN,separator);
			printf(" ");
	}
	printf("\n");
//in functie de tip,se obtine inceputul corespunzator line(int/float/string)
	t_cellType current_type=p->type;
	t_intLine *lineint=NULL;
	t_floatLine *linefloat=NULL;
	t_stringLine *linestring=NULL;
	switch(current_type){
		case INT:
			lineint=(t_intLine *)p->lines;
			break;
		case FLOAT:
			linefloat=(t_floatLine *)p->lines;
			break;
		case STRING:
			linestring=(t_stringLine *)p->lines;
			break;
		default:
			break;
	}
//maxim o bucla se executa si se afiseaza liniile din tabel
	for(;lineint!=NULL;){
			t_intCell *aux=lineint->cells;
			printLineInt(aux);
			lineint=lineint->next;
			
	}
	for(;linefloat!=NULL;){
			t_floatCell *aux=linefloat->cells;
			printLineFloat(aux);
			linefloat=linefloat->next;
		
	}
	for(;linestring!=NULL;){
			t_stringCell *aux=linestring->cells;
			printLineString(aux);
			linestring=linestring->next;
			
	}
	printf("\n");
}
	
//afisare baza de date,se afiseaza secvential fiecare tabel
void Print_db(t_db *db){
	printf("DATABASE: %s\n",db->name);
	printf("\n");
	t_table *table=db->tables;
	while(table){
		Print_table(db,table->name);
		table=table->next;
	}

}


//verifica existenta coloanei cu numele column in tabelul table_name din db
int table_has_column(t_db *db,char *table_name,char column[MAX_COLUMN_NAME_LEN]){
	t_table *p=db->tables;
	while(p){
		if(strcmp(p->name,table_name)==0)
			break;
		p=p->next;
	}
	t_column *current_column=p->columns;
	while(current_column){
		if(strcmp(column,current_column->name)==0){
			return 1;
		}
		current_column=current_column->next;
	}
	return 0;
}

//identificare linii care indeplinesc relatia relation si afisarea acestora
void search_lines(t_table *p,int column_index,char relation[3],char value[MAX_VALUE_LEN]){
	t_cellType currentType=p->type;
	t_intLine *lineint=NULL;
	t_floatLine *linefloat=NULL;
	t_stringLine *linestring=NULL;
	switch(currentType){
		case INT:
			lineint=(t_intLine *)p->lines;
			break;
		case FLOAT:
			linefloat=(t_floatLine *)p->lines;
			break;
		case STRING:
			linestring=(t_stringLine *)p->lines;
			break;
		default:
			break;
	}
	int i;
//maxim o bucla se executa,explicatia de la int e similara la celelalte 2
	for(;lineint!=NULL;){
			i=0;
//se cauta elementul din linia curenta corespunzator coloanei dupa care se
//face cautarea
			t_intCell *aux=lineint->cells;
			t_intCell *start=aux;
			while(i<column_index){
				aux=aux->next;
				i++;
			}
//se stocheaza valoarea din acea celula
			int valueint=atoi(value);
//daca relatia dintre intregi este valida,se afiseaza linia intreaga
			if(relationInt(aux->value,valueint,relation)==1)
				printLineInt(start);
				
			lineint=lineint->next;
				
	}
	for(;linefloat!=NULL;){
			i=0;
			t_floatCell *aux=linefloat->cells;
			t_floatCell *start=aux;
			while(i<column_index){
				aux=aux->next;
				i++;
			}
			float valuefloat=atof(value);
			if(relationFloat(aux->value,valuefloat,relation)==1)
				printLineFloat(start);
				
			linefloat=linefloat->next;
				
	}
	for(;linestring!=NULL;){
			i=0;
			t_stringCell *aux=linestring->cells;
			t_stringCell *start=aux;
			while(i<column_index){
				aux=aux->next;
				i++;
			}
			char *valuestring=value;
			if(relationString(aux->value,valuestring,relation)==1)
				printLineString(start);
				
			linestring=linestring->next;
				
	}
	printf("\n");

}


void Search(t_db *db,char table_name[MAX_TABLE_NAME_LEN],char column[MAX_COLUMN_NAME_LEN],char relation[3],char value[MAX_VALUE_LEN],int found,int ok){
	int column_index=0;
	t_table *p=db->tables;
	while(p){
		if(strcmp(p->name,table_name)==0)
			break;
		p=p->next;
	}
//se obtine indexul coloanei dupa care se realizarea comparatia informatiei
//pentru a fi folosit in functia search_lines
	t_column *current_column=p->columns;
	while(current_column){
		if(strcmp(column,current_column->name)==0){
			break;
		}
		current_column=current_column->next;
		column_index++;
	}
	print_header(db,table_name);
	search_lines(p,column_index,relation,value);
}


//adaugare linie la tabel
void Add_line(t_db *db,char *table_name,char line_content[][MAX_VALUE_LEN],int count){
	t_table *p=db->tables;
	while(p){
		if(strcmp(p->name,table_name)==0)
			break;
		p=p->next;
	}
	t_cellType currentType=p->type;
	t_intLine *lineint=NULL;
	t_floatLine *linefloat=NULL;
	t_stringLine *linestring=NULL;
	switch(currentType){
		case INT:
			lineint=(t_intLine *)p->lines;
			break;
		case FLOAT:
			linefloat=(t_floatLine *)p->lines;
			break;
		case STRING:
			linestring=(t_stringLine *)p->lines;
			break;
		default:
			break;
	}
	t_intLine *newInt=NULL;
	t_floatLine *newFloat=NULL;
	t_stringLine *newString=NULL;
	//se aloca o linie de introdus,de tip corespunzator
	if(currentType==INT){
		newInt=alocInt(line_content,count);
	}else if(currentType==FLOAT){
		newFloat=alocFloat(line_content,count);
	}else
		newString=alocString(line_content,count);	

	if(lineint){
//se identifica ultima linie si se adauga dupa aceasta
		while(lineint->next){
			lineint=lineint->next;
		}
		lineint->next=newInt;
	}else if(currentType==INT){//daca tabelul e gol,dar totusi tipul e int
//inceputul listei de linii puncteaza catre noua linie
		p->lines=newInt;
	}

//similar la float si string ca la int
	if(linefloat){
		while(linefloat->next){
			linefloat=linefloat->next;
		}
		linefloat->next=newFloat;
	}else if(currentType==FLOAT){
		p->lines=newFloat;
	}

	if(linestring){
		while(linestring->next){
			linestring=linestring->next;
		}
		linestring->next=newString;
	}else if(currentType==STRING){
		p->lines=newString;
	}
	
}

//stergere linii tabel
//similar cu delete,insa tabelul si coloanele sale raman in structura bazei de
//date
void Clear(t_db *db,char *table_name){
		t_table *p=db->tables;
		while(p){
			if(strcmp(p->name,table_name)==0)
				break;
			p=p->next;
		}
		t_cellType current_type=p->type;
		t_intLine *lineint=NULL;
		t_floatLine *linefloat=NULL;
		t_stringLine *linestring=NULL;
		switch(current_type){
			case INT:
				lineint=(t_intLine *)p->lines;
				break;
			case FLOAT:
				linefloat=(t_floatLine *)p->lines;
				break;
			case STRING:
				linestring=(t_stringLine *)p->lines;
				break;
			default:
				break;
		}	
		for(;lineint!=NULL;){
			t_intLine *aux2=lineint;
			lineint=lineint->next;
			freeInt(&aux2);
		}
		for(;linefloat!=NULL;){
			t_floatLine *aux2=linefloat;
			linefloat=linefloat->next;
			freeFloat(&aux2);
		}
		for(;linestring!=NULL;){
			t_stringLine *aux2=linestring;
			linestring=linestring->next;
			freeString(&aux2);
		}
		p->lines=NULL;

}

//stergere linii conform conditiei relation
void delete_lines(t_table *p,int column_index,char relation[3],char value[MAX_VALUE_LEN]){
	t_cellType currentType=p->type;
	t_intLine *lineint=NULL;
	t_floatLine *linefloat=NULL;
	t_stringLine *linestring=NULL;
	switch(currentType){
		case INT:
			lineint=(t_intLine *)p->lines;
			break;
		case FLOAT:
			linefloat=(t_floatLine *)p->lines;
			break;
		case STRING:
			linestring=(t_stringLine *)p->lines;
			break;
		default:
			break;
	}
	int i;
	t_intLine *prev=NULL;
	for(;lineint!=NULL;){
			i=0;
//se identifica elementul din linie corespunzator coloanei dupa care se 
//realizeaza comparatia
			t_intCell *aux=lineint->cells;
			while(i<column_index){
				aux=aux->next;
				i++;
			}
			t_intLine *elim=NULL;
			int valueint=atoi(value);
//daca se indeplineste conditia
//se utilizeaza algoritmul de stergere a tuturor aparitiilor unor elemente
//in acest caz,linii,care verifica relatia
			if(relationInt(aux->value,valueint,relation)==1){
				elim=lineint;
				lineint=lineint->next;
				if(prev){
					prev->next=lineint;
				}else{
					p->lines=lineint;
				}
				freeInt(&elim);
			}else{
				prev=lineint;	
				lineint=lineint->next;
			}	
	}
	t_floatLine *prev2=NULL;
	for(;linefloat!=NULL;){
			i=0;
			t_floatCell *aux=linefloat->cells;
			while(i<column_index){
				aux=aux->next;
				i++;
			}
			t_floatLine *elim=NULL;
			float valuefloat=atof(value);
			if(relationFloat(aux->value,valuefloat,relation)==1){
				elim=linefloat;
				linefloat=linefloat->next;
				if(prev2){
					prev2->next=linefloat;
				}else{
					p->lines=linefloat;
				}
				freeFloat(&elim);
			}else{
				prev2=linefloat;	
				linefloat=linefloat->next;
			}	
	}
	t_stringLine *prev3=NULL;
	for(;linestring!=NULL;){
			i=0;
			t_stringCell *aux=linestring->cells;
			while(i<column_index){
				aux=aux->next;
				i++;
			}
			t_stringLine *elim=NULL;
			char *valuestring=value;
			if(relationString(aux->value,valuestring,relation)==1){
				elim=linestring;
				linestring=linestring->next;
				if(prev3){
					prev3->next=linestring;
				}else{
					p->lines=linestring;
				}
				freeString(&elim);
			}else{
				prev3=linestring;	
				linestring=linestring->next;
			}	
	}

}

//functia principala de stergere linii conform relatiei furnizate
void Delete_From_Table(t_db *db,char table_name[MAX_TABLE_NAME_LEN],
char column[MAX_COLUMN_NAME_LEN],char relation[3],char value[MAX_VALUE_LEN]){
	int column_index=0;
	t_table *p=db->tables;
	while(p){
		if(strcmp(p->name,table_name)==0)
			break;
		p=p->next;
	}
	t_column *current_column=p->columns;
	while(current_column){
		if(strcmp(column,current_column->name)==0){
			break;
		}
		current_column=current_column->next;
		column_index++;
	}
	delete_lines(p,column_index,relation,value);
}


int main(){
	char command[MAX_CMD_LEN];
	t_db *db=NULL;
	while(1){//se citeste comanda si se proceseaza
//in cazul delete_db,se dealoca toata informatia ramasa pana in acel moment
//iar programul isi termina executia
		fgets(command,MAX_CMD_LEN,stdin);
		if(command[strlen(command)-1]=='\n'){
			command[strlen(command)-1]='\0';
		}
		if(strstr(command,"INIT_DB")-command==0){
			char *db_name=strchr(command,' ')+1;
			db=InitDB(db_name);
		}else if(strstr(command,"DELETE_DB")-command==0){
			DistrugeDB(&db);
			break;
		}else if(strstr(command,"CREATE")-command==0){
//se parseaza parametri comenzii cu ajutorul functiei strtok
			char copy[MAX_CMD_LEN];
			strcpy(copy,command);
			char *p=strtok(copy," ");
			p=strtok(NULL," ");
			char table_name[MAX_TABLE_NAME_LEN];
//in caz de neconcordante cu baza de date curenta,se afiseaza erori corespunzatoare
			strcpy(table_name,p);
			int found=has_Table(db,table_name);
			if(found){
				printf("Table \"%s\" already exists.\n",table_name);
				continue;
			}
			p=strtok(NULL," ");
			if(strcmp(p,"INT")!=0&&strcmp(p,"FLOAT")!=0&&strcmp(p,"STRING")!=0){
				printf("Unknown data type: \"%s\".\n",p);
				continue;
			}
			t_cellType type;
			if(strcmp(p,"INT")==0)
				type=0;
			else if(strcmp(p,"FLOAT")==0)
				type=1;
			else
				type=2;
			p=strtok(NULL," ");
			char columns[MAX_COLUMNS][MAX_COLUMN_NAME_LEN];
			int count_cols=0;
//se creeaza un vector de nume de coloane care va fi folosit la constructia coloanelor
			while(p){
				strcpy(columns[count_cols++],p);
				p=strtok(NULL," ");
			}
			Create_Table(db,table_name,type,columns,count_cols);
		}else if(strstr(command,"DELETE")-command==0){
			if(strchr(command,' ')==strrchr(command,' ')){//se face diferenta dintre
//delete la un tabel si delete de linii
				char *table_name=strchr(command,' ')+1;
				int found=has_Table(db,table_name);
				if(!found){
					printf("Table \"%s\" not found in database.\n",table_name);
					continue;
				}
				Delete_Table(db,table_name);
			}else{
				char copy[MAX_CMD_LEN];
				strcpy(copy,command);
				char *p=strtok(copy," ");
				p=strtok(NULL," ");
				char table_name[MAX_TABLE_NAME_LEN];
				strcpy(table_name,p);
				int found=has_Table(db,table_name);
				if(!found){
					printf("Table \"%s\" not found in database.\n",table_name);
					continue;
				}
				p=strtok(NULL," ");
				char column[MAX_COLUMN_NAME_LEN];
				strcpy(column,p);
				int ok=table_has_column(db,table_name,column);
				if(!ok){
					printf("Table \"%s\" does not contain column \"%s\".\n",table_name,column);
					continue;
				}
				char relation[3];
				p=strtok(NULL," ");
				strcpy(relation,p);
				char value[MAX_VALUE_LEN];
				p=strtok(NULL," ");
				strcpy(value,p);
				Delete_From_Table(db,table_name,column,relation,value);
			}
		}else if(strstr(command,"PRINT_DB")-command==0){
			Print_db(db);
		}else if(strstr(command,"PRINT")-command==0){
			char *table_name=strchr(command,' ')+1;
			int found=has_Table(db,table_name);
			if(!found){
				printf("Table \"%s\" not found in database.\n",table_name);
				continue;
			}
			Print_table(db,table_name);
		}else if(strstr(command,"SEARCH")-command==0){
//parsarea parametrilor ca la create sau delete
			char copy[MAX_CMD_LEN];
			strcpy(copy,command);
			char *p=strtok(copy," ");
			p=strtok(NULL," ");
			char table_name[MAX_TABLE_NAME_LEN];
			strcpy(table_name,p);
			int found=has_Table(db,table_name);
			if(!found){
				printf("Table \"%s\" not found in database.\n",table_name);
				continue;
			}
			p=strtok(NULL," ");
			char column[MAX_COLUMN_NAME_LEN];
			strcpy(column,p);
			int ok=table_has_column(db,table_name,column);
			if(!ok){
				printf("Table \"%s\" does not contain column \"%s\".\n",table_name,column);
				continue;
			}
			char relation[3];
			p=strtok(NULL," ");
			strcpy(relation,p);
			char value[MAX_VALUE_LEN];
			p=strtok(NULL," ");
			strcpy(value,p);
			Search(db,table_name,column,relation,value,found,ok);
		}else if(strstr(command,"ADD")-command==0){
			char copy[MAX_CMD_LEN];
			strcpy(copy,command);
			char *p=strtok(copy," ");
			p=strtok(NULL," ");
			char table_name[MAX_TABLE_NAME_LEN];
			strcpy(table_name,p);
			int found=has_Table(db,table_name);
			if(!found){
				printf("Table \"%s\" not found in database.\n",table_name);
				continue;
			}
			char line_content[MAX_COLUMNS][MAX_VALUE_LEN];
			p=strtok(NULL," ");
			int count=0;
			while(p){
				strcpy(line_content[count++],p);
				p=strtok(NULL," ");
			}
			Add_line(db,table_name,line_content,count);
		}else if(strstr(command,"CLEAR")-command==0){
			char *table_name=strchr(command,' ')+1;
			int found=has_Table(db,table_name);
			if(!found){
				printf("Table \"%s\" not found in database.\n",table_name);
				continue;
			}
			Clear(db,table_name);
		}else{//comanda introdusa gresit sau inexistenta
			printf("Unknown command: \"%s\".\n",command);
		}
		
	}
	return 0;
}

