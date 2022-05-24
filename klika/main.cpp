#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int okno;
int **macierz;
vector<int> klika;
vector<int> stopien;
vector< vector<int> > sasiedzi;

class Sekwencja
{
public:
    int numer_sekwencji;
    int dlugosc_sekwencji;
    string sekw;
};

class Nukleotydy:Sekwencja
{
public:
    string pojedyncze[150];
    int numer[150];
    string ocena1;
    int ocena2[150];
};

class Punkt:Nukleotydy
{
public:
    string *litery;
    int *wmacierzy;
    int ostatni;
    int ilosc;
    Punkt();
};
Punkt::Punkt()
{
    litery=new string[150];
    for(int i=0;i<150;i++)
        litery[i].clear();
    wmacierzy=new int[150];
    for(int i=0;i<150;i++)
        wmacierzy[i]=-1;
}

int dlugosc_okna(int *okno)
{
    do
    {
        cout<<"Podaj dlugosc okna (4-7): ";
        cin>>*okno;
    }while(*okno<4 || *okno>7);
    return *okno;
}

int main()
{
    int wierzcholki,prog,k,l,u,pierwsza,druga,ile,numer,st,ile_sasiadow,nr,rozmiar,ile_sekwencji,maksymalna,ktory,m,z;
    vector<int> tab;
    vector<int> ile1;
    vector<int> ile2;
    vector<int> sekk;
    vector<int> ilosc_sek;
    vector<int> dodaj;
    vector<int> jakie1;
    vector<int> dodatkowa;

    Sekwencja s[7];
    Nukleotydy n[7];
    Punkt p[7];

    fstream wczytaj;
    wczytaj.open("sekwencja.fasta",ios::in);
    if(wczytaj.good()==false)
    {
        cout<<"Nie udalo sie otworzyc pliku!";
        exit(0);
    }
    int i,j,dlugosc;
    i=j=0;
    string sekw;
    while(getline(wczytaj,sekw))
    {
        if(i%2!=0)
        {
            j++;
            dlugosc=sekw.length();
            s[j-1].numer_sekwencji=j;
            s[j-1].sekw=sekw;
            s[j-1].dlugosc_sekwencji=dlugosc;
        }
        i++;
    }
    wczytaj.close();

    for(i=0;i<7;i++)
    {
        for(j=0;j<s[i].dlugosc_sekwencji;j++)
        {
            n[i].pojedyncze[j]=s[i].sekw[s[i].dlugosc_sekwencji-j-1];
            n[i].numer[j]=j;
        }
    }

    fstream wczytaj_ocene;
    wczytaj_ocene.open("ocena.qual",ios::in);
    if(wczytaj_ocene.good()==false)
    {
        cout<<"Nie udalo sie otworzyc pliku!";
        exit(0);
    }
    i=j=0;
    while(getline(wczytaj_ocene,sekw))
    {
        if(i%2!=0)
        {
            j++;
            n[j-1].ocena1=sekw;
        }
        i++;
    }
    wczytaj_ocene.close();

    for(i=0;i<7;i++)
    {
        istringstream iss(n[i].ocena1);
        string slowo;
        int liczba;
        j=0;
        while(iss>>slowo)
        {
            liczba=atoi(slowo.c_str());
            n[i].ocena2[j]=liczba;
            j++;
        }
    }

    okno=0;
    dlugosc_okna(&okno);

    wierzcholki=0;
    for(i=0;i<7;i++)
    {
        for(int k=0;k<s[i].dlugosc_sekwencji;k++)
        {
            if(okno+k<=s[i].dlugosc_sekwencji)
            {
                for(j=k;j<okno+k;j++)
                {
                    p[i].litery[k]+=n[i].pojedyncze[j];
                }
                p[i].wmacierzy[k]=wierzcholki;
                wierzcholki++;
            }
        }
    }

    cout<<"Podaj prog wiarygodnosci: ";
    cin>>prog;

    int minimum=100;
    for(i=0;i<s[0].dlugosc_sekwencji;i++)
    {
        if(n[0].ocena2[i]<minimum)
            minimum=n[0].ocena2[i];
    }

    int del=0;
    if(prog>=minimum)
        del=1;

    macierz=new int*[wierzcholki];
    for(i=0;i<wierzcholki;i++)
        macierz[i]=new int[wierzcholki];
    for(i=0;i<wierzcholki;i++)
    {
        for(j=0;j<wierzcholki;j++)
            macierz[i][j]=0;
    }

    string temp,pod1,pod2;
    pod1.clear();
    pod2.clear();

    vector<string> delecje;
    vector<int> poz;
    int ogr=s[0].dlugosc_sekwencji;
    for(i=0;i<ogr;i++)
    {
        temp.clear();
        if(okno+i<=s[0].dlugosc_sekwencji)
        {
            for(j=i;j<okno+i;j++)
            {
                if(n[0].ocena2[j]>=prog)
                    temp+=n[0].pojedyncze[j];
                else
                    continue;
            }
            delecje.push_back(temp);
            poz.push_back(i);
        }
    }

    int licz=0;
    ile=0;
    numer=-1;
    for(i=0;i<7;i++)
    {
        for(j=0;j<7;j++)
        {
            if(i!=j)
            {
                for(k=0;k<(s[i].dlugosc_sekwencji-okno+1);k++)
                {
                    for(l=0;l<(s[j].dlugosc_sekwencji-okno+1);l++)
                    {
                        if(p[i].litery[k]==p[j].litery[l])
                        {
                            pierwsza=p[i].wmacierzy[k];
                            druga=p[j].wmacierzy[l];
                            macierz[pierwsza][druga]=1;
                            macierz[druga][pierwsza]=1;
                        }
                        if(del)
                        {
                            for(int y=0;y<delecje.size();y++)
                            {
                                if(okno==4 && delecje[y].length()>=3)
                                {
                                    pod1=delecje[y];
                                    pod2=p[j].litery[l];
                                    licz=0;
                                    for(int z=0;z+1<okno;z++)
                                    {
                                        if(pod1[z]==pod2[z])
                                            licz++;
                                    }
                                    if(licz>=3)
                                    {
                                        pierwsza=p[0].wmacierzy[poz[y]];
                                        druga=p[j].wmacierzy[l];
                                        macierz[pierwsza][druga]=1;
                                        macierz[druga][pierwsza]=1;
                                    }
                                }
                                if(okno==5 && delecje[y].length()>=4)
                                {
                                    pod1=delecje[y];
                                    pod2=p[j].litery[l];
                                    licz=0;
                                    for(int z=0;z+1<okno;z++)
                                    {
                                        if(pod1[z]==pod2[z])
                                            licz++;
                                    }
                                    if(licz>=4)
                                    {
                                        pierwsza=p[0].wmacierzy[poz[y]];
                                        druga=p[j].wmacierzy[l];
                                        macierz[pierwsza][druga]=1;
                                        macierz[druga][pierwsza]=1;
                                    }
                                }
                                if(okno==6 && delecje[y].length()>=4)
                                {
                                    pod1=delecje[y];
                                    pod2=p[j].litery[l];
                                    licz=0;
                                    for(int z=0;z+1<okno;z++)
                                    {
                                        if(pod1[z]==pod2[z])
                                            licz++;
                                    }
                                    if(licz>=4)
                                    {
                                        pierwsza=p[0].wmacierzy[poz[y]];
                                        druga=p[j].wmacierzy[l];
                                        macierz[pierwsza][druga]=1;
                                        macierz[druga][pierwsza]=1;
                                    }
                                }
                                if(okno==7 && delecje[y].length()>=5)
                                {
                                    pod1=delecje[y];
                                    pod2=p[j].litery[l];
                                    licz=0;
                                    for(int z=0;z+1<okno;z++)
                                    {
                                        if(pod1[z]==pod2[z])
                                            licz++;
                                    }
                                    if(licz>=5)
                                    {
                                        pierwsza=p[0].wmacierzy[poz[y]];
                                        druga=p[j].wmacierzy[l];
                                        macierz[pierwsza][druga]=1;
                                        macierz[druga][pierwsza]=1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for(i=0;i<wierzcholki;i++)
        tab.push_back(-1);

    for(i=0;i<wierzcholki;i++)
        sasiedzi.push_back(tab);

    for(i=0;i<7;i++)
    {
        for(k=0;k<(s[i].dlugosc_sekwencji-okno+1);k++)
        {
            p[i].ostatni=p[i].wmacierzy[k];
            p[i].ilosc=k;
        }
    }

    for(i=0;i<7;i++)
    {
        ile1.push_back(p[i].ostatni-p[i].ilosc);
        ile2.push_back(p[i].ostatni);
    }

    nr=-1;
    for(i=0;i<7;i++)
    {
        for(j=0;j<(s[i].dlugosc_sekwencji-okno+1);j++)
        {
            ile_sasiadow=0;
            st=p[i].wmacierzy[j];
            for(k=0;k<wierzcholki;k++)
            {
                if(macierz[st][k]==1)
                    ile_sasiadow++;
            }
            stopien.push_back(ile_sasiadow);
        }
    }

    for(u=0;u<wierzcholki;u++)
    {
        for(i=0;i<wierzcholki;i++)
        {
            if(macierz[u][i])
                sasiedzi[u][i]=i;
        }

        vector<int> dod;
        for(i=0;i<sasiedzi.size();i++)
        {
            if(sasiedzi[u][i]>-1)
                dod.push_back(sasiedzi[u][i]);
        }

        vector<int> jakie;
        for(i=0;i<dod.size();i++)
        {
            for(j=0;j<ile1.size();j++)
            {
                if(dod[i]>=ile1[j] && dod[i]<=ile2[j])
                    jakie.push_back(j);
            }
        }

        rozmiar=0;
        for(i=0;i<jakie.size();i++)
        {
            if(jakie[i]!=jakie[i+1])
                rozmiar++;
        }
        ilosc_sek.push_back(rozmiar);
    }

    maksymalna=0;
    for(i=0;i<ilosc_sek.size();i++)
    {
        if(ilosc_sek[i]>maksymalna)
        {
            maksymalna=ilosc_sek[i];
            ktory=i;
        }
    }

    for(i=0;i<sasiedzi.size();i++)
    {
        if(sasiedzi[ktory][i]>-1)
            dodaj.push_back(sasiedzi[ktory][i]);
    }

    for(i=0;i<dodaj.size();i++)
    {
        for(j=0;j<ile1.size();j++)
        {
            if(dodaj[i]>=ile1[j] && dodaj[i]<=ile2[j])
                jakie1.push_back(j);
        }
    }

    for(i=0;i<jakie1.size();i++)
    {
        if(jakie1[i]!=jakie1[i+1])
            dodatkowa.push_back(i);
    }
    for(i=0;i<dodatkowa.size();i++)
        klika.push_back(dodaj[dodatkowa[i]]);

    for(i=0;i<dodatkowa.size();i++)
    {
        for(j=0;j<7;j++)
        {
            for(k=0;k<(s[i].dlugosc_sekwencji-okno+1);k++)
            {
                if(klika[i]==p[j].wmacierzy[k])
                {
                    cout<<"numer sekwencji: "<<j+1<<endl;
                    cout<<"znaleziony fragment: "<<p[j].litery[k]<<endl;
                    cout<<"od pozycji: "<<n[j].numer[k]<<endl<<endl;
                }
            }
        }
    }

    return 0;
}
