#include<iostream>
#include <fstream>
#include <stdio.h>
#include <algorithm>
using namespace std;


struct FCFS
{
    /*
      AT -> Arrival Time                    //Tablica struktur zawieraj¹ca wymienione zmienne potrzebne do obliczen w algorytmie
      BT -> Burst time
      FT -> Finish time
      WT -> Waiting Time
      TAT-> Turn around time
      */
    int AT,BT,FT,WT,TAT;

} tab[1000];

bool checkAT(struct FCFS a,struct FCFS b)
{
    return a.AT < b.AT;                     //Funkcja porownuje s¹siednie wartoœci AT i zwraca fa³sz albo prawda
}

int main()
{

    int sumaTAT=0;              //suma Turn around time
    int sumaWT=0;               //suma Waiting Time
    float averageTAT=0;         //srednia Turn around time
    float averageWT=0;          //srednia Waiting Time
    int count_=1;               //zlicza, który ci¹g zosta³ wpisany



    ifstream plik;                   // deklaracja pokazuj¹ca programowi,
    plik.open("dane.txt");          // sk¹d bêd¹ wczytywane dane



    while(!plik.eof())                  //wykonuje, a¿ nie na trafi na koniec pliku
    {
        for(int i=0; i<100; i++)        //pêtla wczytuj¹ca AT ktorymi s¹ pierwsze 100 liczb
        {
            //Arrival Time
            plik>>tab[i].AT;
        }
        for(int i=0; i<100; i++)        //pêtla wczytuj¹ca BT ktorymi s¹ drugie 100 liczb
        {
            //Burst Time
            plik>>tab[i].BT;
        }


        stable_sort(tab,tab+100,checkAT); //sortowanie ca³ej tablicy struktur wzglêdem AT

        for(int i=0; i<100; i++)                    //pêtla dla 100 procesów w jednym ci¹gu
        {
            if(i==0)                                //warunek pozwalaj¹cy na unikniêcie b³êdu w pierwszej linijce wyœwietlanej tabeli
            {
                tab[i].FT=tab[i].AT+tab[i].BT;         //Finish time = Arrival Time + Burst Time
            }
            else if(tab[i-1].FT<tab[i].AT)          //warunek sprawdzaj¹cy czy Finish time nie jest wiêkszy ni¿ Arrival Time
            {
                int X;
                X=tab[i].AT-tab[i-1].FT;                //X = Arrival Time + Finish time(ale z poprzedniej linijki)
                tab[i].FT=tab[i-1].FT+X+tab[i].BT;      //Finish time = Finish time(ale z poprzedniej linijki)+ X + Burst Time

            }
            else                                        //jeœli poprzednie warunki (if) nie zostan¹ u¿yte to wyliczmy FT w podstawowy sposób
            {
                tab[i].FT=tab[i-1].FT+tab[i].BT;        //Finish time = Finish time(ale z poprzedniej linijki) + Burst Time
            }

            tab[i].TAT=tab[i].FT-tab[i].AT;             //Turn around time = Finish time - Arrival Time
            tab[i].WT=tab[i].TAT-tab[i].BT;             //Waiting Time = Turn around time - Burst Time

            sumaTAT+=tab[i].TAT;       //sumuje Turn around time
            sumaWT+=tab[i].WT;         //sumuje Waiting Time
        }

        averageTAT=float(sumaTAT)/100;      //œrednia wyliczana Turn around time
        averageWT=float(sumaWT)/100;        //œrednia wyliczana Waiting Time


        //WYPISANIE DO PLIKU/NA EKRAN

        ofstream zapis;
        zapis.open("zapis_dane_test_FCFS.txt",ios_base::app);

        zapis<<endl;
        zapis<<endl;
        zapis<<"\t\t\t\t\t"<<count_<<"\t\t\t"<<endl;
        zapis<<"\nProcess\tArrival Time\tBurst time\tFinish time\tWaiting time\tTurn around time\n";
        for(int i=0; i<100; i++)
        {
            zapis<<i+1<<"\t"<<tab[i].AT<<"\t\t"<<tab[i].BT<<"\t\t"<<tab[i].FT<<"\t\t"<<tab[i].WT<<"\t\t"<<tab[i].TAT<<endl;
        }

        zapis<<endl;
        zapis<<"\nAverage turn around time= "<< averageTAT;
        zapis<<"\nAverage Waiting Time= "<<averageWT;


        //ZEROWANIE - zeruje podstawowe zmienne u¿ywane w jednym ci¹gu

        sumaTAT=0;
        sumaWT=0;
        for(int i=0; i<100; i++)
        {
            tab[i].AT=0;
            tab[i].BT=0;
            tab[i].FT=0;
            tab[i].TAT=0;
            tab[i].WT=0;
        }
        count_++; //zlicza ktory ciąg wykonuje program
    }

    return 0;
}
