#include<iostream>
#include <fstream>
#include <stdio.h>
#include <algorithm>
using namespace std;

struct LRU
{
    /*PR -> values of Page Referencing
      MorH -> MISS or HIT                   //Tablica struktur zawieraj¹ca wymienione zmienne potrzebne do obliczen w algorytmie
    */
    int PR,MorH;

} tab[1000];


int main()
{
    int frame[20];               //tablica w ktorej element jest ramka w algorytmie( w zależności od wybranej liczby ramek )
    int lastFrame[20];           //tablica pomocnicza dla ramek która była ostatnio używana
    int PF=0; //Page Frame       //liczba ramek wczytywana z pliku
    int NCH=0; //not count hit   //do zliczania nie trafionych stron
    int CH=0; //count hit        //do zliczania trafionych stron
    int pf=0; //Number of Misses //licznik błędów strony
    int i=0;                     //zmienna do sterowania petla while
    int n=100;                   //liczba numerow stron w jednym ciagu
    int count_=1;                //zlicza, który ci¹g zosta³ wpisany

    fstream plik;                       // deklaracja pokazujaca programowi,
    plik.open("dane_STRONY.txt");       // sk¹d bêd¹ wczytywane dane
    ofstream zapis;
    zapis.open("zapis_dane_test_LRU.txt",ios_base::app);

    while(!plik.eof())                  //wykonuje, a¿ nie na trafi na koniec pliku
    {
        //Page frames
        plik>>PF;                       //wczytuje PF które jest pierwszą liczba w wczytywanym pliku

        for(int i=0; i<n; i++)          //pêtla wczytuj¹ca PR ktorymi s¹ drugie 100 liczb
        {
            //values of Page Referencing
            plik>>tab[i].PR;
        }



        for(int i=0; i<PF; i++)         //wypelniamy całą tablice ramek -1 i zerujemy
        {
            frame[i]=-1;
            lastFrame[i]=0;

        }


        //wypisywanie na ekran/do pliku

        zapis<<endl;
        zapis<<"\t\t\t\t\t"<<count_<<"\t\t\t"<<endl;
        zapis<<"LP.\tPage Trace\t";
        for(int i=0; i<PF; i++)
        {
            zapis<<"\tPage Frame"<<i+1<<"\t";
        }
        zapis<<endl;

        //ALGORYTM

        while(i<n)          //pêtla dla 100 procesow jednego ci¹gu steruj¹ca algorytmem
        {
            if(frame[i]==-1 && i<PF) 	//warunek sprawdza czy ramka jest pusta (pusta jeśli wartość równa -1)tylko dla pierwszych 3 stron
            {
                frame[i]=tab[i].PR;

                for(int j=0; j<PF; j++)
                {
                    if(frame[j]==tab[i+1].PR) 		//sprawdzamy czy w ramce jest numer strony ktorej szukamy
                    {
                        tab[i+1].MorH=1;  //zapisanie do tablicy trafień w odpowiednim indeksie
                    }
                    else
                    {
                        NCH++;              //zlicanie nie trafionych
                    }
                }
            }
            else
            {

                NCH=0;
                for(int j=0; j<PF; j++)
                {

                    if(frame[j]==tab[i].PR) 		//sprawdzamy czy w ramce jest numer strony ktorej szukamy
                    {
                        tab[i].MorH=1;      //zapisanie do tablicy trafień w odpowiednim indeksie
                        lastFrame[j]=i+1;   //zapisanie do tablicy pomocniczej ramek indeks strony

                    }
                    else
                    {
                        NCH++;                  //zlicanie nie trafionych
                    }
                }
                if(NCH==PF) 					//warunek sprawdzający czy nie trafione strony są równe licbie ramek
                {

                    int mini=0;                     //zmienna pomocnicza do wyliczenia ostanio używanej strony
                    for(int j=0; j<PF-1; j++) 		//pętla przechodząca przez ramki-1
                    {
                        if(lastFrame[mini]>lastFrame[j+1]) //warunek do szukania ostatnio używanej strony
                            mini=j+1;
                    }

                    frame[mini]=tab[i].PR;                               //zastępujemy wartość w ramce nową
                    lastFrame[mini]=i+1;                                 //zapisujemy indeks ramki ktora została wpisana
                                                                    //zliczanie blędów strony
                }
            }

            //przekierowanie na ekran/do pliku
            zapis<<i+1<<"\t"<<tab[i].PR<<"\t";
            for(int i=0; i<PF; i++)
            {
                if(frame[i]==-1)
                    zapis<<"\t\t"<<0<<"\t";
                else
                    zapis<<"\t\t"<<frame[i]<<"\t";
            }
            if(tab[i].MorH==1)
            {
                zapis<<"\t"<<"Hit";
                CH++;           //zliczanie trafień
            }
            else if(tab[i].MorH==0)
                pf++;

            zapis<<endl;

            i++;
        }
        zapis<<endl;
        zapis<<"\nNumber of Misses= "<< pf;
        zapis<<"\nMiss ratio= "<< float(pf)/n;
        zapis<<"\nNumber of Hits="<<CH;
        zapis<<"\nHit ratio= "<<float(CH)/n;





        //ZEROWANIE

        PF=0;//Page frames
        NCH=0; //not count hit
        CH=0; //count hit
        pf=0; //Page fault
        i=0;
        for(int i=0; i<1000; i++)
        {
            tab[i].PR=0;
            tab[i].MorH=0;
        }

        count_++; //zlicza ktory ciąg wykonuje program

    }

    return 0;
}
