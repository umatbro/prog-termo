#include <iostream>
#include <stdio.h> //printf
#include <stdlib.h> //exit_success

using namespace std;

class wyjscie0_s
{
public:
    float h;
    wyjscie0_s(float wys)
    {
        h = wys;
    }
};


class wejscie0_s
{
public:
    float q_d; //dop³yw do zbiornika
    float q_o;  // odp³yw ze zbiornika
    wejscie0_s(float doplyw, float odplyw)
    {
        q_d = doplyw;
        q_o = odplyw;
    }
};

wyjscie0_s obiekt(wejscie0_s we)
{
    //parametry obiektu
    float s=3; //przekroj zbiornika
    //pomocnicze zmienne lokalne
    static wyjscie0_s wy(0);
    wy.h = wy.h + (we.q_d + we.q_o)/s; // tu zamiast modelu mo¿e byæ wartoœæ odczytana z realnego obiektu

    if (wy.h < 0) wy.h = 0; //poziom cieczy nie moze byæ ujemny
    return wy;
}

float regulator(wyjscie0_s w_zad, wyjscie0_s wy_o)
{
    float k=3;
    float k_i = 4;
    float k_d = 0;

    //zmienne pomocnicze
    float p,i,d,r;
    float u; //uchyb regulacji
    static float u_p = 0; //uchyb regulacji w poprzednim wywo³aniu
    static float su = 0; //suma minionych uchybów regulacji
    u = w_zad.h - wy_o.h; // aktualny uchyb regulacji

// wyznaczenie skladnika proporcjonalnego
    p = k * u;

// wyznaczenie sk³adnika ca³kowego
su = su + u; //najpierw trzeba wyliczyæ sumê wszystkich uchybów;
i = k_i * su;

// wyznaczenie sk³adnika D
d = k_d * (u-u_p);
u_p = u; //zapamiêtaj chwilow¹ wartoœæ uchybu dla przysz³ych obliczeñ

r = p + i + d; //sygna³ wyjœciowy regulatora
if (r > 0) r = 0; //czyli wyp³yw nie mo¿e byæ dodatni

    return r;
}

void drukuj(unsigned int t, wejscie0_s we, wyjscie0_s wy, wyjscie0_s zad)
{
    //cout<<"t = "<<t<<"\tq_d = "<<we.q_d<<"\tq_o = "<<we.q_o<<"\t\th = "<<wy.h<<"\t(zad: "<<zad.h<<")"<<"\n";
    printf("t=%3d, q_d=%9f, q_o=%9f h=%9f (zadane:%8f)\n",t,we.q_d,we.q_o,wy.h,zad.h);
}

int main()
{
    unsigned int t = 0; //czas
    unsigned int powtorzen = 20; //ile powtorzen w jednym kroku
    wejscie0_s we_o(0,0);  //wartosci wejœciowe obiektu
    wyjscie0_s wy_o(0);     //wartoœci wyjœciowe obiektu
    wyjscie0_s w_zad(2); //wartoœæ zadana dla regulatora

    we_o.q_d = 1; //wymuszenie na wejœciu do obiektu
    bool flaga = false;
    char wybor;
    do
    {
        for (int i = 0; i<powtorzen; i++)
        {
            t++;
            wy_o = obiekt(we_o); //pobierz wartoœci wyjœciowe obiektu
            drukuj(t,we_o,wy_o,w_zad);
            we_o.q_o = regulator(w_zad,wy_o);
        }
        cout<<"Kolejne 20s? (T/N): ";
        cin>>wybor;
        if (wybor == 'T' || wybor == 't' || wybor == '1' || wybor == 'y') flaga = true;
        else flaga = false;
    }while(flaga);
    return EXIT_SUCCESS;
}
