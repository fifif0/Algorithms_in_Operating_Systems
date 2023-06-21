#include<iostream>
#include <fstream>
#include <stdio.h>
#include <algorithm>
using namespace std;


struct FIFO_LRU                                               //Tablica struktur zawieraj¹ca wymienione zmienne potrzebne do obliczen w algorytmie
{
    /*PR -> values of Page Referencing
      MorH -> MISS or HIT
      */
    int PR,MorH;

} tablica[1000];

//FIFO

void algorytmFIFO()
{
    int count_=1;                //zlicza, który ci¹g zosta³ wpisany
    int frame[20];               //tablica w ktorej element jest ramka w algorytmie( w zależności od wybranej liczby ramek )
    int PF=0; //Page Frame       //liczba ramek wczytywana z pliku
    int NCH=0; //not count hit   //do zliczania nie trafionych stron
    int CH=0; //count hit        //do zliczania trafionych stron
    int pf=0; //Number of Misses //licznik błędów strony
    int i=0;                     //zmienna do sterowania petla while
    int n=100;                   //liczba numerow stron w jednym ciagu
    int temp;                    //zmienna pomocnicza

    fstream plik;                       // deklaracja pokazujaca programowi,
    plik.open("dane_STRONY.txt");       // sk¹d bêd¹ wczytywane dane

    ofstream zapis;
    zapis.open("zapis_dane_test_FIFO.txt",ios_base::app);


    while(!plik.eof())           //wykonuje, a¿ nie na trafi na koniec pliku
    {

        //Page frames
        plik>>PF;               //wczytuje PF które jest pierwszą liczba w wczytywanym pliku

        for(int i=0; i<n; i++)          //pêtla wczytuj¹ca PR ktorymi s¹ drugie 100 liczb
        {
            //values of Page Referencing
            plik>>tablica[i].PR;

        }

        for(int i=0; i<PF; i++)             //wypelniamy całą tablice -1
        {
            frame[i]=-1;
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

        while(i<n)                     //pêtla dla 100 procesow jednego ci¹gu steruj¹ca algorytmem
        {
            if(frame[i]==-1 && i<PF) 	//warunek sprawdza czy ramka jest pusta (pusta jeśli wartość równa -1)tylko dla pierwszych 3 stron
            {
                frame[i]=tablica[i].PR;

                for(int j=0; j<PF; j++)
                {
                    if(frame[j]==tablica[i+1].PR) 		//sprawdzamy czy w ramce jest numer strony ktorej szukamy
                    {
                        tablica[i+1].MorH=1;  //zapisanie do tablicy trafień w odpowiednim indeksie
                    }
                    else
                    {
                        NCH++;              //zlicanie nie trafionych
                    }
                }
                if(tablica[i].MorH==1)
                {
                    frame[i]=-1;
                }
            }
            else
            {

                NCH=0;
                for(int j=0; j<PF; j++)
                {
                    if(frame[j]==tablica[i].PR) 		//sprawdzamy czy w ramce jest numer strony ktorej szukamy
                    {
                        tablica[i].MorH=1;  //zapisanie do tablicy trafień w odpowiednim indeksie
                    }
                    else
                    {
                        NCH++;                  //zlicanie nie trafionych

                    }
                }
                if(NCH==PF) 					//warunek sprawdzający czy nie trafione strony są równe licbie ramek
                {
                    for(int j=0; j<PF-1; j++) 		//przesunięcie stron w ramkach
                    {
                        temp=frame[j];
                        frame[j]=frame[j+1];
                        frame[j+1]=temp;
                    }

                    frame[PF-1]=tablica[i].PR;                 //wstawiamy wartość nowej strony

                }
            }

            //przekierowanie na ekran/do pliku
            zapis<<i+1<<"\t"<<tablica[i].PR<<"\t";
            for(int i=0; i<PF; i++)
            {
                if(frame[i]==-1)
                    zapis<<"\t\t"<<0<<"\t";
                else
                    zapis<<"\t\t"<<frame[i]<<"\t";
            }
            if(tablica[i].MorH==1)
            {
                zapis<<"\t"<<"Hit";
                CH++;           //zliczanie trafień
            }
            else if(tablica[i].MorH==0)
                pf++;

            zapis<<endl;
            i++;
        }


        zapis<<"\nNumber of Misses= "<< pf;
        zapis<<"\nMiss ratio= "<< float(pf)/n;
        zapis<<"\nNumber of Hits="<<CH;
        zapis<<"\nHit ratio= "<<float(CH)/n;



//ZEROWANIE


        PF=0;//Page frames
        NCH=0; //not count hit
        CH=0; //count hit
        pf=0; //Page fault
        temp=0;
        i=0;
        for(int i=0; i<PF; i++)
        {
            frame[i]=0;
        }

        for(int i=0; i<100; i++)
        {
            tablica[i].PR=0;
            tablica[i].MorH=0;
        }

        count_++;
    }
}

//LRU

void algorytmLRU()
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
            plik>>tablica[i].PR;
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
                frame[i]=tablica[i].PR;

                for(int j=0; j<PF; j++)
                {
                    if(frame[j]==tablica[i+1].PR) 		//sprawdzamy czy w ramce jest numer strony ktorej szukamy
                    {
                        tablica[i+1].MorH=1;  //zapisanie do tablicy trafień w odpowiednim indeksie
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

                    if(frame[j]==tablica[i].PR) 		//sprawdzamy czy w ramce jest numer strony ktorej szukamy
                    {
                        tablica[i].MorH=1;      //zapisanie do tablicy trafień w odpowiednim indeksie
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

                    frame[mini]=tablica[i].PR;                               //zastępujemy wartość w ramce nową
                    lastFrame[mini]=i+1;                                 //zapisujemy indeks ramki ktora została wpisana
                                                                    //zliczanie blędów strony
                }
            }

            //przekierowanie na ekran/do pliku
            zapis<<i+1<<"\t"<<tablica[i].PR<<"\t";
            for(int i=0; i<PF; i++)
            {
                if(frame[i]==-1)
                    zapis<<"\t\t"<<0<<"\t";
                else
                    zapis<<"\t\t"<<frame[i]<<"\t";
            }
            if(tablica[i].MorH==1)
            {
                zapis<<"\t"<<"Hit";
                CH++;           //zliczanie trafień
            }
            else if(tablica[i].MorH==0)
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
            tablica[i].PR=0;
            tablica[i].MorH=0;
        }

count_++;

    }
}



int main()
{
    int zmienna;
    cout<<"Wybierz odpowiedni algorytm podajac odpowiedni numer"<<endl;
    cout<<"Dostepne algorytmy: "<<endl;
    cout<<"1.Algorytm zastepowania stron - FIFO"<<endl;
    cout<<"2.Algorytm zastepowania stron - LRU"<<endl;

    cin>>zmienna;




    switch(zmienna)
    {
    case 1:
        cout<<"1.Algorytm zastepowania stron - FIFO"<<endl;
        algorytmFIFO();
        break;

    case 2:
        cout<<"1.Algorytm zastepowania stron - LRU"<<endl;
        algorytmLRU();
        break;


    default:
        break;
    }



    return 0;
}
