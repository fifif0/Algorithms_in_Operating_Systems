#include<iostream>
#include <fstream>
#include <stdio.h>
#include <algorithm>
using namespace std;

struct SJF
{
    /*
      AT -> Arrival Time                    //Tablica struktur zawieraj¹ca wymienieone zmienne potrzebne do obliczen w algorytmie
      BT -> Burst time
      FT -> Finish time
      WT -> Waiting Time
      TAT-> Turn around time
      */
    int AT,BT,FT,WT,TAT;

} tab[1000];


bool checkAT(struct SJF a,struct SJF b)              //Funkcja porownuje s¹siednie wartoœci AT i zwraca fa³sz albo prawda
{
    int x =a.AT;
    int y =b.AT;
    return x < y;
}



int main()
{

    int sumaTAT=0;              //suma Turn around time
    int sumaWT=0;               //suma Waiting Time
    float averageTAT=0;         //œrednia Turn around time
    float averageWT=0;          //œrednia Waiting Time
    int n=100;                  //liczba ci¹gów
    int count_=1;               //zlicza, który ci¹g zosta³ wpisany

    int realTIME=0;             //zmienna czasu rzeczywistego (AT) potrzebna aby unikn¹æ problemu z wyborem procesu który ju¿ dotar³
    int condition=0;            //zmienna do sprawdzenia czy wykonaliœmy wszystkie procesy z ci¹gu
    int complete[100];          //pomocnicza tablica maj¹ca oznaczaæ element ktore zosta³y ju¿ policzone

    for(int i=0; i<100; i++)    //zerujemy wszystkie elementy tablicy
    {
        complete[i]=0;
    }

    ifstream plik;                  // deklaracja pokazuj¹ca programowi,
    plik.open("dane.txt");          // sk¹d bêd¹ wczytywane dane


    while(!plik.eof())                  //wykonuje, a¿ nie na trafi na koniec pliku
    {
        for(int i=0; i<n; i++)        //pêtla wczytuj¹ca AT ktorymi s¹ pierwsze 100 liczb
        {
            //Arrival Time
            plik>>tab[i].AT;
        }
        for(int i=0; i<n; i++)        //pêtla wczytuj¹ca BT ktorymi s¹ drugie 100 liczb
        {
            //Burst Time
            plik>>tab[i].BT;
        }


        while(condition != n)           //pêtla dla 100 procesow jednego ci¹gu steruj¹ca algorytmem
        {
            int num = -1;               //zmienna pomocnicza do oznaczenia danych do obliczeñ
            int mini = 10000;           //zmienna sprawdzaj¹ca i zapamiêtuj¹ca BT

            for(int i=0; i<n; i++)      //pêtla dla 100 procesow jednego ci¹gu do poszukiwania odpowiednich cmiennych
            {
                if(tab[i].AT<=realTIME && complete[i]==0) //warunek sprawdzaj¹cy czy AT jest mniejsze ni¿ rzeczywisty czas && tablica pomocnicza ¿eby niewzi¹æ dwa razy tego samego procesu
                {
                    if(tab[i].BT<mini)  //warunek sprawdzaj¹cy czy BT od zmiennej mini
                    {
                        mini = tab[i].BT;       //przypisanie mini do wartoœæi BT o indeksie i
                        num = i;                //przypisanie num do obecnego indeksu zmiennej
                    }

                    else if(tab[i].BT==mini) //warunek sprawdzaj¹cy czy BT jest rowne mini
                    {
                        if(tab[i].AT<tab[num].AT) //warunek sprawdzaj¹cy czy bierz¹ce AT jest mniejsze od AT (o indeksie zapisanym w zmiennej num)
                        {
                            mini = tab[i].BT;       //przypisanie mini do wartoœæi BT o indeksie i
                            num = i;                //przypisanie num do obecnego indeksu zmiennej
                        }
                    }
                }
            }

            if(num!=-1)         //warunek sprawdzaj¹cy czy num (indeks zmiennej) jest ro¿ny od -1 czyli czy zostal zamieniony
            {
                //wyliczenia prowadzimy w oparciu o indeks zapisany w num
                tab[num].FT=realTIME+tab[num].BT;       //Finish time = rzeczywisty czas + Burst time
                tab[num].TAT=tab[num].FT-tab[num].AT;   //Turn around time = Finish time - Arrival Time
                tab[num].WT=tab[num].TAT-tab[num].BT;   //Waiting Time = Turn around time - Burst time

                sumaTAT+=tab[num].TAT;       //sumuje Turn around time
                sumaWT+=tab[num].WT;         //sumuje Waiting Time


                complete[num]=1; //zmiana wartoœci na jeden w indeksie ktorego urzywaliœmy wczeœniej
                condition++; //inkrementacja zmiennej
                realTIME=tab[num].FT; //nowy czas rzeczywisty równy obecenemu Finish time
            }
            else //jeœli num = -1
            {
                realTIME++;//to tylko inkrementujemy czas rzeczywisty
            }

        }


        averageTAT=float(sumaTAT)/n;        //œrednia wyliczana Turn around time
        averageWT=float(sumaWT)/n;          //œrednia wyliczana Waiting Time


        //sort(tab,tab+n,checkAT);


        //WYPISANIE DO PLIKU/NA EKRAN

        ofstream zapis;
        zapis.open("zapis_dane_test_SJF.txt",ios_base::app);

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
        realTIME=0;
        condition=0;
        for(int i=0; i<100; i++)
        {
            tab[i].AT=0;
            tab[i].BT=0;
            tab[i].FT=0;
            tab[i].TAT=0;
            tab[i].WT=0;
            complete[i]=0;
        }

        count_++; //zlicza ktory ciąg wykonuje program
    }
    return 0;
}
