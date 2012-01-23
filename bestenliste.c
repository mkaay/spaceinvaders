#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bestenliste 
{
       char name[10];
       int zeit;
       };

Highscore(int time)
{
        // variablen
    char name[10];
    struct bestenliste *daten;
    daten=(struct bestenliste*)malloc(11*sizeof(struct bestenliste));
    FILE *datei;
    int i,j;

   
    system("cls");
    printf("\n bitte Name eingeben : ");
    scanf("%s",&name);
        
    datei = fopen("bestenliste.txt", "r");
    for(i=0;i<10;i++)
    {
        fread(&daten[i],sizeof(struct bestenliste),1,datei);
    }
    fclose(datei);
    
 
    for(i=0;i<10;i++)
    {
        if (time > daten[i].zeit) 
        {
        break;
        }
    }
  /*  
    for(j=9;j>=i;j--)
    {
        daten[j+1]=daten[j];
    }
   */
    strcpy (daten[i].name,name);
    daten[i].zeit=time;
    
   
    datei = fopen("bestenliste.txt", "w");	
    
   for(i=0;i<10;i++)
    {
        fwrite(&daten[i],sizeof(struct bestenliste),1,datei);
    }
    
    for(i=10; i>0; i--)
    {
        printf("\n %s : %d",daten[i].name,daten[i].zeit);
    }
    fclose(datei);

    }

int main()
{
   Highscore(1); 
    
    
  system("PAUSE");	
  return 0;
}






/*#include <stdio.h>
#include <stdlib.h>



void bestenliste_einlesen(){
    FILE *datei;
    datei = fopen("bestenliste.txt", "r");	
}


struct highscore{
    int_ergebnis;
    char name [10];
    int counter = 10;
    int ergebnis [6];
    };

typedef struct highscore h;

for (counter=0; counter < 10 ; counter ++){
    name[counter]=0;
    ergebnis[counter]=0;
}

printf("\n bitte Nickname eingeben");
scanf("%s",&h.name);


for (h.counter= 0 -> 10){
    fprint(out,"%s",h.name);
	}

fprintf(out,".");


while (isnumeric(getc...)==0){
    ergebnis// char
    }
      
int_ergebnis=atoi(ergebnis);
    
void quicksort{
    int data;
    struct linked_list *link;
    struct linked_list *prev;
    }
typedef struct linked_list node;
void create(node *first);
void print(node *first);
node *quick_sort(node *first);
int count(node *first);

main()
    {
    node *first;
    int x,n,i,j=0,m;
    clrscr();
    first=(node *)malloc(sizeof(node));
    first->prev=0;

    create(first);
    printf("the numbers you have entered are :\n");
    print(first);
    n=count(first);
    printf("\n the number of elements you have entered are: %d\n",n);
    for(i=0;idata);
    printf( " do you want to add more?(y=1/n=2)\n");
    scanf("%d",&x);
    if(x==2){
        first->link=0;
        }
        else
        {
        first->link= (node *)malloc(sizeof(node));
        first->link->prev=first;
        create(first->link);
        }
return;
}

typedef struct t_bestenliste
        { 
        int id_platz;
        char s_name[10];
        int int_punktzahl;
        t_bestenliste *next
        }  t_bestenliste;
      
      t_bestenliste bestenliste[10];
      
