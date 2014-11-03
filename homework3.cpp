#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 999999


typedef struct nod
{
	int cont_nod;
	float cost_arc;
	struct nod *next;
	int *cale_nod;
	int cont;
} nod;
//   Structura specifica grafului, unde campul n reprezinta numarul de noduri, iar v este un vector de liste
typedef struct 
{
	int n;
	nod **v;
}Graf;
//  initG reprezinta functia de initializare a grafului
void initG( Graf & g, int nr)
{	
	g.n = nr;
	g.v = (nod**)calloc(nr+1,sizeof(nod*));
}
//  addM reprezinta functia de adaugare de noduri la listele de adiacenta
void addM( Graf & g, int curent, float length, int next_one )
{
        nod *temp = (nod*)malloc(sizeof(nod));
        temp->cost_arc = length;
        temp->cont_nod = next_one;
        temp->cale_nod = (int*)calloc(g.n,sizeof(int));
        temp->cont = 0;
        temp->next = g.v[curent];
        g.v[curent] = temp;
}
//  Functia modM modifica costul dintre doua noduri si actualizeaza vectorul de noduri intermediare
void modM(float length, int k, nod * & temp)
{
			temp->cost_arc = length;
			temp->cale_nod[temp->cont] = k;
			temp->cont = temp->cont + 1;	
}
//  Functia cauta se ocupa cu gasirea unor noduri atunci cand se primesc ca parametrii sursa si destinatia. Rezultatul
//  Rezultatul este memorat in variabila de tip nod "temp" si de asemenea returneaza 1 in cazul in care cautarea s-a rea-
//  lizat ccu succes sau zero altfel. 
int cauta(Graf & g, int x0, int i, nod * & temp)
{
	if (x0 >= g.n )
		return 0;
	for (temp = g.v[x0]; temp != NULL; temp = temp->next)
		if (temp->cont_nod == i)
				return 1;
	return 0;
}
// Functia drum_minim e principala functia care se ocupa de aflarea drmului de cost minim dintre oricare doua noduri.
void drum_Minim(Graf & g)
{
	int i, j, k, flag;
	float arc_ik, arc_kj, arc_ij;
	nod *temp, *aux;
	for (k=0;k<g.n;k++)
			for (i=0;i<g.n;i++)
				for (j=0;j<g.n;j++)
				{
                    // Pentru cele 3 bucle for, avem urmatoarele combinatii "ik", "jk", "ij", unde i,j,k reprezinta indicii nodurilor.
                    // Se disting 3 situatii. Daca indicii coincid atunci valoarea costului e 0, iar in caz contrar se apeleaza functia de cautare
                    // Daca aceasta functie returneaza 1, inseamna ca exista o cale si un cost aferent intre cei doi indici(cele doua noduri), iar daca nu
                    // costul va fi initializat cu o valoare MAX.
					if (i==k)
						arc_ik = 0;
					else
					{
						cauta(g,i,k,temp);
						if (temp == NULL)
							arc_ik = MAX;
						else
							arc_ik = temp->cost_arc;
					}
					if (j==k)
						arc_kj = 0;
					else
					{
						cauta(g,k,j,temp);
						if (temp == NULL)
							arc_kj = MAX;
						else
							arc_kj = temp->cost_arc;
					}
					flag = 0;
					if (i!=j)
					{
						cauta(g,i,j,temp);
						if (temp == NULL)
							arc_ij = MAX;
						else
						{
							arc_ij = temp->cost_arc;
							flag=1;
						}
					}
                    // conditia de existenta a unui drum de cost minim
					if (arc_ik && arc_kj && i!=j && (arc_ij > arc_ik + arc_kj))
					{
						arc_ij = arc_ik + arc_kj;
                        //   Daca se gaseste un drum intre oricare doua noduri cu un cost mai mic, atunci acest cost se actualizeaza.
                        // De asemenea exista si situatia cand intre doua noduri nu exista o legatura directa(un arc direct), ci o cale
                        //	cu un anumit cost, caz in care se va adauga un nou nod la lista de adicenta. Astfel pentru fiecare lista,
                        // nodurile nu vor mai reprezenta succesori directi ai primului element. 					
						if (flag == 0)
							addM(g, i, arc_ij, j);
						cauta(g,i,j,aux);
						modM(arc_ij,k,aux);
					}

				}

}
// Functia cale1 parcurge recursiv graful pentru a determina nodurile intermediare dintre doua noduri
void cale1(Graf & g, int i, int j, nod * & aux)
{
	int m,k;
	for (m = 0; m < aux->cont; m++)
	{
		k = aux->cale_nod[m];
		if (k != -1)
		{
			if (cauta(g,i,k,aux) == 1)
				cale1(g,i,k,aux);

			printf("%d ", k);

			if (cauta(g,k,j,aux) == 1)
				cale1(g,k,j,aux);
				
		}
	}
}
// Functia cale2 este asemanatoare cu cale1 doar ca de data aceasta afiseaza si niste insstructiuni de urmat.
void cale2(Graf & g, int i, int j, nod * & aux)
{
	int m,k;
	for (m = 0; m < aux->cont; m++)
	{
		k = aux->cale_nod[m];
		if (k != -1)
		{
			if (cauta(g,i,k,aux) == 1)
				cale2(g,i,k,aux);

			printf("virati catre %d\n", k);
			printf("mergeti inainte %.3f km\n", aux->cost_arc);

			if (cauta(g,k,j,aux) == 1)
				cale2(g,k,j,aux);
				
		}
	}
}
// Din functia cerinta se apeleaza functia drum_minim precum si cele doua functii cale1 si cale2. Pe langa acestea 
// functia cerinta mai contine niste conditii care trateaza cazurile de "adresa invalida" sau "traseu imposibil"
void cerinta(Graf & g, int oA1, int deA1, float distA1, int oA2, int deA2, float distA2, int task)
{
	nod *aux;
	int x0, x1,i;
	float d1=0,d2=0;
	
	if (oA1 == oA2 || deA1 == deA2)
	{
		printf("adresa invalida\n");
		return;
	}
	if (cauta(g,oA1,deA1,aux) == 0)
	{
		printf("adresa invalida\n");
		return;
	}

	d1 = aux->cost_arc - distA1;
	if (d1 < 0)
	{
		printf("adresa invalida\n");
		return;
	}
	if ( task == 2 )
		if (d1 > 0)
			printf("mergeti inainte %.3f km\n", d1);
	x0 = deA1;	

	if (cauta(g,oA2,deA2,aux) == 0)
	{
		printf("adresa invalida\n");
		return;
	}
	d2 = aux->cost_arc - distA2;
	if (d2 < 0)
	{
		printf("adresa invalida\n");
		return;
	}
	if (distA2 > 0)
		x1 = deA2;
	else
		x1 = oA2;

	drum_Minim(g);

	if (cauta(g,x0,x1,aux) == 0)
	{
		printf("traseu imposibil\n");
		return;
	}

    if ( task == 1 )
    {
		printf("%.3f ",aux->cost_arc + d1 - d2);
		if (d1 > 0)
			printf("%d ",x0);
		cale1(g,x0,x1,aux);
		printf("\n");
    }

	if ( task == 2 )
	{
		cale2(g,x0,x1,aux);
		if (distA2 != 0)
		{
			printf("virati catre %d\n" ,deA2);
			printf("mergeti inainte %.3f km\n", distA2);
			printf("ati ajuns la destinatie\n");
		}
		else
		{
			printf("virati catre %d\n", x1);
			printf("mergeti inainte %.3f km\n", aux->cost_arc);
			printf("ati ajuns la destinatie\n");
		}
	}
}

// In sfarsit, functia main primeste nr de argumente precum si vectorul argumentelor
// Citeste numarul de noduri din fiserul de intrare si initializeaza graful. Apoi adauga elemente in graf si continua cu tratarea fiecarei
// cerinte in parte. 
int main(int argc, char *argv[])
{
	int c1=1, c2=2;
	int n,i,j,succ,length,follower,startA1,endA1;
	int startA2,endA2;
	float distanceA1, distanceA2;
	Graf g;
	FILE *fisier;
	fisier=fopen(argv[2],"r");
	fscanf(fisier,"%d",&n);
	initG(g,n);
    
	for (i=0;i<n;i++)
	{
		fscanf(fisier,"%d",&succ);
		for (j=0;j<succ;j++)
		{
			fscanf(fisier,"%d", &length);
			fscanf(fisier,"%d", &follower);
			addM (g,i,length,follower);
		}
	}
	
	if (strcmp(argv[1],"1")==0)
	{
		scanf("%d%d%f",&startA1,&endA1,&distanceA1);
		scanf("%d%d%f",&startA2,&endA2,&distanceA2);
		cerinta(g,startA1,endA1,distanceA1,startA2,endA2,distanceA2,c1);
	}
	if (strcmp(argv[1],"2")==0)
	{
		scanf("%d%d%f",&startA1,&endA1,&distanceA1);
		scanf("%d%d%f",&startA2,&endA2,&distanceA2);
		cerinta(g,startA1,endA1,distanceA1,startA2,endA2,distanceA2,c2);
	}

	fclose(fisier);	
return 0;
}
