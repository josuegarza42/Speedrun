
#include <iostream>

#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "laberinto.h"
#include <time.h>
using namespace std;
int tocabotonIN(int x,int y,BOTON menu[2]);
void MenuInicio(char *n, char *n2);
int Juego(char *n, char *n2,char *n3,int *puno,int *pdos);

int main()
{
    int boolganador=0;
    int p1=1000,p2=1000;
    initwindow(680,480);
    char n[6],n2[6],n3[6];
    while(true)
    {
        MenuInicio(n,n2);
        boolganador=Juego(n,n2,n3,&p1,&p2);
        if(boolganador >0)
        {
            ganador(n3,p2);
        }
        else if (boolganador<0)
        {
            ganador(n3,p1);

        }

    }
    getch();
    closegraph();
}
