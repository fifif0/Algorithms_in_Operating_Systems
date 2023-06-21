#include<iostream>
#include <fstream>
#include <stdio.h>
#include <algorithm>
using namespace std;

struct FCFS_SJF                                               //Tablica struktur zawieraj¹ca wymienione zmienne potrzebne do obliczen w algorytmie
{
    /*
      AT -> Arrival Time
      BT -> Burst time
      FT -> Finish time
      WT -> Waiting Time
      TAT-> Turn around time
      */
    int AT,BT,FT,WT,TAT;

} tab[1000];

struct FIFO_LRU                                               //Tablica struktur zawieraj¹ca wymienione zmienne potrzebne do obliczen w algorytmie
{
    /*PR -> values of Page Referencing
      MorH -> MISS or HIT
      */
    int PR,MorH;

} tablica[1000];

bool checkAT(struct FCFS_SJF a,struct FCFS_SJF b)             //Funkcja porownuje s¹siednie wartoœci AT i zwraca fa³sz albo prawda
{
    return a.AT < b.AT;
}

bool checkBT(struct FCFS_SJF a,struct FCFS_SJF b)             //Funkcja porownuje s¹siednie wartoœci BT i zwraca fa³sz albo prawda
{
    int x =a.BT;
    int y =b.BT;
    return x < y;
}

bool checkFT(struct FCFS_SJF a,struct FCFS_SJF b)             //Funkcja porownuje s¹siednie wartoœci FT i zwraca fa³sz albo prawda
{
    int x =a.FT;
    int y =b.FT;
    return x < y;
}



//FCFS

void algorytmFCFS()
{

    int sumaTAT=0;              //suma Turn around time
    int sumaWT=0;               //suma Waiting Time
    float averageTAT=0;         //srednia Turn around time
    float averageWT=0;          //srednia Waiting Time
    int count_=1;               //zlicza, który ci¹g zosta³ wpisany



    fstream plik;                   // deklaracja pokazuj¹ca programowi,
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
        count_++;
    }

}

//SJF

void algorytmSJF()
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


        sort(tab,tab+n,checkAT);


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

        count_++;
    }
}

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
    cout<<"1.Algorytm przydzialu czasu procesora - FCFS"<<endl;
    cout<<"2.Algorytm przydzialu czasu procesora - SJF"<<endl;
    cout<<"3.Algorytm zastepowania stron - FIFO"<<endl;
    cout<<"4.Algorytm zastepowania stron - LRU"<<endl;


    cin>>zmienna;



    switch(zmienna)
    {
    case 1:
        cout<<"1.Algorytm przydzialu czasu procesora - FCFS"<<endl;
        algorytmFCFS();
        break;

    case 2:
        cout<<"2.Algorytm przydzialu czasu procesora - SJF"<<endl;
        algorytmSJF();
        break;

    case 3:
        cout<<"1.Algorytm zastepowania stron - FIFO"<<endl;
        algorytmFIFO();
        break;

    case 4:
        cout<<"1.Algorytm zastepowania stron - LRU"<<endl;
        algorytmLRU();
        break;



    default:
        break;
    }



    return 0;
}
