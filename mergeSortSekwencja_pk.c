#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define TABLE_MAX_LENGTH 1000

typedef struct DataS{
    int *tab;		// Wskaznik na tablice 
    int lewyIndeks;	// Poczatkowy Indeks tablicy
    int prawyIndeks;	// Koncowy Indeks tablicy
}DataS;


/**  scalaj(int tab[], int lewy, int srodek, int prawy)
 * Funkcja dokonuje scalania podzielonej tablicy i sortowania jej
 * Pierwsza czesc jest tablica tab[l..srodek]
 * Druga czesc jest tablica tab[srodek+1..r]
 *
 * @param[in] - tab - wskaznik na tablice
 * @param[in] -	lewy - lewy indeks tablicy
 * @param[in] -	srodek - srodkowy indeks tablicy
 * @param[in] -	prawy - prawy indeks tablicy
 * @parame[out] - void
 ***/

void scalaj(int tab[], int lewy, int srodek, int prawy)
{
    //printf("zlacz\n");
    int i, j, k;
    int n1 = srodek - lewy + 1;
    int n2 = prawy - srodek;


    /* Kopiowanie danych do tablic tymczasowych TempLewy[], TempPrawy[] */
    int TempLewy[n1], TempPrawy[n2];
    for (i = 0; i < n1; i++)
        TempLewy[i] = tab[lewy + i];
    for (j = 0; j < n2; j++)
        TempPrawy[j] = tab[srodek + 1 + j];

    /* Polacz temp tab spowrotem w tab[l..r]*/
    i = 0; // poczatkowy indeks pierwszej czastki tablicy
    j = 0; // poczatkowy indeks drugiej czastki tablicy
    k = lewy; // poczatkowy indeks polaczonej tablicy
    while (i < n1 && j < n2) {
        if (TempLewy[i] <= TempPrawy[j]) {
            tab[k] = TempLewy[i];
            i++;
               // printf("1_TABLICA[%d] = %d\n", k, TempLewy[i]);
        }
        else {
            tab[k] = TempPrawy[j];
            j++;
               // printf("2_TABLICA[%d] = %d\n", k, TempPrawy[j]);
        }
        k++;
    }

    /* Kopiowanie pozostalych elementow TempLewy[], jesli cos zostalo */
        while (i < n1) {
        tab[k] = TempLewy[i];
                //printf("3_TABLICA[%d] = %d\n", k, TempLewy[i]);
        i++;
        k++;
    }

    /* Kopiowanie pozostalych elementow TempPrawy[], jesli cos zostalo */
    while (j < n2) {
        tab[k] = TempPrawy[j];
                //printf("4_TABLICA[%d] = %d\n", k, TempPrawy[j]);
        j++;
        k++;
    }
}


/**  sortujScalaj(void *arg)
 * Funkcja dokonuje rekurencyjnego podzialu tablicy na dwie czesci
  *
 * @param[in] - strktura DataS - zawiera 
 *		tab - wskaznik na tablice
 * 		lewyIndeks - lewy indeks tablicy
 * 		prawyIndeks - prawy indeks tablicy
 * @parame[out] - void
 *
 ***/
void* sortujScalaj(void *arg)
{
    DataS data =  *((DataS*)arg);
	
    int *tab = data.tab;
    int lewyIndeks = data.lewyIndeks;
    int prawyIndeks = data.prawyIndeks;

	//printf("Prt0 - lewy:[%d], prawy:[%d]\n", lewyIndeks, prawyIndeks);

    if (data.lewyIndeks < data.prawyIndeks) {
        /*** mid - srodek wyliczenie srodka 
	 * m = (lewyIndeks + prawyIndeks)/2 
	 * w celu zabezpieczenia przed przeciazeniem dodawane jest do lewyIndeks
	 ** */
        int mid = lewyIndeks + (prawyIndeks - lewyIndeks) / 2;

        DataS args_1;
	args_1.tab = tab;
	args_1.lewyIndeks = data.lewyIndeks;
       	args_1.prawyIndeks = mid;

        
	DataS args_2;
	args_2.tab = tab;
	args_2.lewyIndeks = mid + 1;
	args_2.prawyIndeks = prawyIndeks;

        // Posortuj pierwsza i druga polowke
	//printf("Prt1 - lewy:[%d],srodek:[%d], prawy:[%d]\n", lewyIndeks, mid, prawyIndeks);

	sortujScalaj(&args_1);

        //printf("Prt2 - lewy:[%d], srodek:[%d], prawy:[%d]\n", lewyIndeks, mid, prawyIndeks);

	sortujScalaj(&args_2);

        //printf("Ptr3 - lewy:[%d], srodek:[%d], prawy:[%d]\n", lewyIndeks, mid, prawyIndeks);

        scalaj(tab, lewyIndeks, mid, prawyIndeks);
    }
}

/**  drukujTab
 * drukowanie tablicy
 * @param[in] - tab - wskaznik na tablice
 * @param[in] - size - rozmiar tablicy
 * @parame[out] - void
 *
 ***/
void drukujTab(int tab[], int rozmiar)
{
    int i;
    for (i = 0; i < rozmiar; i++)
        printf("%d ", tab[i]);
    printf("\n");
}

void generujTab(int *tab, int rozmiar)
{
	int i;
	for (i = 0; i < rozmiar; i++)
	{
		tab[i] = 1 + rand() % rozmiar;
	}

}

int main()
{
    	int tab[TABLE_MAX_LENGTH] = {0};
	generujTab(tab, TABLE_MAX_LENGTH);
	int tab_size = sizeof(tab) / sizeof(tab[0]);

	DataS dane;
	dane.tab = tab;
	dane.lewyIndeks = 0;
	dane.prawyIndeks = tab_size - 1;

	printf("ROZMIAR TABLICY WEJSCIOWEJ = %d\n", tab_size);

	printf("TABLICA WEJSCIOWA: \n");
	drukujTab(tab, tab_size);

	sortujScalaj(&dane);
   
	printf("\nTABLICA POSORTOWANA: \n");
	drukujTab(tab, tab_size);
	return 0;
}
