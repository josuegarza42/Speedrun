#ifndef LABERINTO_H_INCLUDED
#define LABERINTO_H_INCLUDED

#define TAM 30
#include <time.h>

typedef struct  //ESTRUCTURA QUE DEFINE LA COORDENADA INCIAL, FINAL Y EL COLOR DE LOS BOTÓNES
{
    float x1,y1,x2,y2;
    int color;
    char titulo[10];
} BOTON;

typedef struct nodoimg  //ESTRUCTURA DONDE SE GUARDAN LOS SPRITES DEL JUEGO
{
    void *img;
    int tam;
} IMAGEN;

typedef struct nodo  //ESTRUCUTRA DE LOS NODOS QUE FORMAN LA MALLA
{
    struct nodo *up;
    struct nodo *down;
    struct nodo *left;
    struct nodo *right;
    int x;
    int y;
    int id;
} NODO_MALLA;

typedef struct  //CONTROL DE INICIO Y FINAL DE MALLA COMO DE LOS BLOQUES QUE FORMAN EL ESCENARIO
{
    NODO_MALLA *cab1;
    NODO_MALLA *cab2;
    IMAGEN bloques[10];
} TCabeza;

typedef struct  //GUARDA LA INFORMACIÓN DE LOS PERSONAJES
{
    int x,y;
    NODO_MALLA *pos,*ini;
    IMAGEN sprite[4];
    int dire;
    int in;
    int gana;
    char nombre[4];
} PLAYER;


typedef struct  //GUARDA LA INFROMACIÓN DE LOS ENEMIGOS
{
    int x,y;
    NODO_MALLA *pos;
    IMAGEN sprite[4];
    int dire;
    int in;
} MOB;

typedef struct  //GUARDA LOS TIEMPOS PARA LOS CAMBIOS DE ESCENARIOS
{
    int x;
    int y;
    int Ttotal;
} TTiempo;
//////////////////////
void Titulos(int pantalla)  //GENERAR LOS TITULOS DE LAS DIFERENTES PANTALLAS
{
    do
    {
        switch(pantalla)
        {
        case 1:
            settextstyle(8,0,6);
            outtextxy(getmaxx()/5,80,"SPEEDRUN");
            settextstyle(8,0,2);
            outtextxy(getmaxx()/2,120,"Josue garza & Jorge Moctezuma");
            break;
        case 2:
            settextstyle(10,0,4);
            outtextxy(340,100,"GAME OVER");
            settextstyle(10,0,3);
            outtextxy(170,150,"WINNER: ");

        }
    }
    while(pantalla<!3);
}
////////////////////////////////////////
int tocabotonIN(int x,int y,BOTON menu[3]) //INTERACCIÓN MENÚ PRINCIPAL
{
    if(x>menu[0].x1 && x<menu[0].x2 && y>menu[0].y1 && y<menu[0].y2)
    {
        return 0;
    }
    if(x>menu[1].x1 && x<menu[1].x2 && y>menu[1].y1 && y<menu[1].y2)
    {
        exit (-1);
    }
    if(x>menu[2].x1 && x<menu[2].x2 && y>menu[2].y1 && y<menu[2].y2)
    {
        return 1;
    }
    if((x>535 || x<616 )&& (y>444 || y<470))
    {
        return 2;
    }

}
void creabotoninicio(BOTON menu[3])  //CREACIÓN DE LOS BOTONES DEL PROGRAMA
{
    float x1,x2,y1,y2,lon1,lon2;
    x1=(getmaxx()/6)*2.1;
    x2=(getmaxx()/6)*3.5;
    y1=(getmaxy()/6)*2.1;
    y2=(getmaxy()/6)*2.6;
    for(int i=0; i<3; i++)
    {
        menu[i].x1=x1;
        menu[i].x2=x2;
        menu[i].y1=(y1+(100*i));
        menu[i].y2=(y2+(100*i));
        menu[i].color=10;
    }
    strcpy(menu[0].titulo,"INICIO");
    strcpy(menu[1].titulo,"SALIR");
    strcpy(menu[2].titulo,"AYUDA");
    setfillstyle(5,3);
    for(int j=0; j<3; j++)
    {
        bar(menu[j].x1,menu[j].y1,menu[j].x2,menu[j].y2);
        line(menu[j].x1,menu[j].y1,menu[j].x1,menu[j].y2);
        line((menu[j].x1)-1,(menu[j].y1)-1,(menu[j].x1)-1,(menu[j].y2)-1);
        line(menu[j].x1,menu[j].y1,menu[j].x2,menu[j].y1);
        line((menu[j].x1)-1,(menu[j].y1)-1,menu[j].x2,(menu[j].y1)-1);
        settextstyle(6,0,3);
        lon1=(menu[j].x1)+((menu[j].x2)-(menu[j].x1)-textheight(menu[j].titulo))/5;
        lon2=(menu[j].y1)+((menu[j].y2)-(menu[j].y1)-textheight(menu[j].titulo))/5;
        outtextxy(lon1,lon2,menu[j].titulo);
    }
}

NODO_MALLA *creaNodo()  //CREA UN NODO DE LA MALLA
{
    NODO_MALLA *nuevo;
    nuevo=(NODO_MALLA *)malloc(sizeof(NODO_MALLA));
    nuevo->down=nuevo->left=nuevo->right=nuevo->up=NULL;
    nuevo->x=nuevo->y=0;
    nuevo->id=0;
    return (nuevo);
}
void creaMalla(TCabeza *cab)  //CREA LA MALLA COMPLETA
{
    int i,j;
    NODO_MALLA *nuevo,*aux;
    for(i=0; i<TAM; i++)
    {
        cab->cab1=aux=nuevo=creaNodo();
        for(j=0; j<TAM; j++)
        {
            nuevo=creaNodo();
            aux->right=nuevo;
            nuevo->left=aux;
            cab->cab1->right->left=cab->cab1;
            if(cab->cab2)
            {
                cab->cab2->down=aux;
                aux->up=cab->cab2;
            }
            if(i==0)
                aux->up=nuevo->up=NULL;
            if(i==TAM-1)
                aux->down=nuevo->down=NULL;
            if(j==0)
                aux->left=nuevo->left=NULL;
            if(j==TAM-1)
                aux->right=nuevo->right=NULL;
            aux->x=j*16;
            aux->y=i*16;
            aux=nuevo;
            if(cab->cab2)cab->cab2=cab->cab2->right;
        }
        cab->cab2=cab->cab1;
    }
    for(; cab->cab1->up; cab->cab1=cab->cab1->up);
    for(i=0,cab->cab2=cab->cab1; cab->cab2->up && i<TAM; cab->cab2=cab->cab2->up,i++);
    for(i=0,cab->cab2; cab->cab2->down && i<TAM-1; cab->cab2=cab->cab2->down,i++);
    for(i=0,cab->cab2; cab->cab2->right && i<TAM-1; cab->cab2=cab->cab2->right,i++);
}

PLAYER *creaJug(NODO_MALLA *cab,char *nom)  //CREA UN JUGADOR
{
    PLAYER *nuevo;
    nuevo=(PLAYER *)malloc(sizeof(PLAYER));
    nuevo->dire=nuevo->x=nuevo->y=0;
    nuevo->ini=NULL;
    nuevo->pos=cab;
    nuevo->in=nuevo->gana=0;
    strcpy(nuevo->nombre,nom);
    return(nuevo);
}

MOB *creaMob()  //CREA UN ENEMIGO
{
    MOB *nuevo;
    nuevo=(MOB *)malloc(sizeof(MOB));
    nuevo->dire=nuevo->x=nuevo->y=0;
    nuevo->pos=NULL;
    nuevo->in=0;
    return(nuevo);
}
void dibuja(PLAYER *j1,PLAYER *j2,TCabeza C,MOB *e1, MOB *e2, MOB *e3)  //DIBUJA MAPA Y JUGADORES
{
    NODO_MALLA *aux1,*aux2;
    int i,j;
    putimage(j1->pos->x,j1->pos->y,j1->sprite[j1->dire].img,1);
    putimage(j2->pos->x,j2->pos->y,j2->sprite[j2->dire].img,1);

    putimage(e1->pos->x,e1->pos->y,e1->sprite[e1->dire].img,1);
    putimage(e2->pos->x,e2->pos->y,e2->sprite[e2->dire].img,1);
    putimage(e3->pos->x,e3->pos->y,e3->sprite[e3->dire].img,1);

    for(i=0,aux1=C.cab1; i<TAM && aux1; i++,aux1=aux1->down)
    {
        for(j=0,aux2=aux1; j<TAM && aux2; j++,aux2=aux2->right)
        {
            if(aux2->id!=0)
                putimage(aux2->x,aux2->y,C.bloques[aux2->id].img,1);
        }
    }
}
///////////////////////////////////////////////
void guardaSprite(IMAGEN *spr,char nom[30])  // EXTRAE LOS SPRITES DEL ARCHIVO PARA CARGARLOS AL JUEGO
{
    FILE *a;
    int i,j,e;
    a=fopen(nom,"r");

    fscanf(a,"%d",&e);
    fscanf(a,"%d",&e);

    for(i=0; i<16; i++)
    {
        for(j=0; j<16; j++)
        {
            fscanf(a,"%d",&e);
            putpixel(i,j,e);
        }
    }
    fclose(a);
    spr->tam=imagesize(0,0,15,15);
    spr->img=malloc(spr->tam);
    getimage(0,0,15,15,spr->img);
}

void cargaSprite(IMAGEN sprej1[4],IMAGEN sprej2[4],IMAGEN sprem[10],IMAGEN spen1 [4],IMAGEN spen2[4],IMAGEN spen3[4])  //CARGA LOS SPRITES AL JUEGO
{
    guardaSprite(&sprej1[0],"p1arriba.txt");
    guardaSprite(&sprej1[1],"p1abajo.txt");
    guardaSprite(&sprej1[2],"p1dere.txt");
    guardaSprite(&sprej1[3],"p1izq.txt");

    guardaSprite(&sprej2[0],"p2arriba.txt");
    guardaSprite(&sprej2[1],"p2abajo.txt");
    guardaSprite(&sprej2[2],"p2dere.txt");
    guardaSprite(&sprej2[3],"p2izq.txt");

    guardaSprite(&sprem[1],"pared.txt");
    guardaSprite(&sprem[2],"pared2.txt");
    guardaSprite(&sprem[3],"pared3.txt");
    guardaSprite(&sprem[4],"meta.txt");

    guardaSprite(&spen1[0],"dragonizq.txt");
    guardaSprite(&spen1[1],"dragondere.txt");
    guardaSprite(&spen1[2],"dragonabajo.txt");
    guardaSprite(&spen1[3],"dragonarriba.txt");


    guardaSprite(&spen2[0],"dragonizq.txt");
    guardaSprite(&spen2[1],"dragondere.txt");
    guardaSprite(&spen2[2],"dragonabajo.txt");
    guardaSprite(&spen2[3],"dragonarriba.txt");

    guardaSprite(&spen3[0],"dragonizq.txt");
    guardaSprite(&spen3[1],"dragondere.txt");
    guardaSprite(&spen3[2],"dragonabajo.txt");
    guardaSprite(&spen3[3],"dragonarriba.txt");

}

void puntuaciones(char linea[10])             //Esta funcion se encarga de mostrar las puntuaciones
{
    //Obteniendolas del archivo donde fueron guardadas.

    FILE *vP;
    vP=fopen("puntuaciones.txt", "r");
    if(vP==NULL)
    {
        printf("No se han registrado puntuaciones.\n");
    }
    else
    {
        printf("PUNTUACIONES\n\n");
        fgets(linea, 10, (FILE*) vP);
        printf("Numero guardado: %s\n",linea);

        fclose(vP);
    }
}


void GuardarNumero(int numero)
{
    int num=numero;
    FILE *arch;
    arch=fopen("puntuaciones.txt", "w");
    fprintf(arch,"%d\n",num);
    fclose(arch);
}


void ganador(char *nom,int pg)
{
    char linea[10];
    int tecla=0;

    GuardarNumero(pg);
    puntuaciones(linea);

    while(tecla!=13)
    {
        if(kbhit())
        {
            tecla=getch();
            Titulos(2);
            settextstyle(10,0,3);
            outtextxy(340,150,nom);
            outtextxy(170,250,"Puntuacion:");
            outtextxy(340,250,linea);

        }
    }
}

void muevejugadores(PLAYER **p1,PLAYER **p2,int *puntos1,int *puntos2)  //LEE LAS TECLAS PARA QUE LOS JUGADORES SE PUEDAN MOVER POR LA MALLA
{
    int tecla=0;
    if(kbhit())
    {
        tecla=getch();
        if(tecla==224)tecla=getch();
        switch(tecla)
        {
        case 77:
            (*p1)->dire=2;
            if((*p1)->pos->right &&((*p1)->pos->right->id==0 ||(*p1)->pos->right->id==4))
            {
                (*p1)->pos=(*p1)->pos->right;
                *puntos1-=5;
                printf("p1:%d\n",*puntos1);
            }
            if((*p1)->pos->id==4) (*p1)->gana=1;
            break;
        case 75:
            (*p1)->dire=3;
            if((*p1)->pos->left &&((*p1)->pos->left->id==0 ||(*p1)->pos->left->id==4))
            {
                (*p1)->pos=(*p1)->pos->left;
                *puntos1-=5;
                printf("p1:%d\n",*puntos1);
            }

            if((*p1)->pos->id==4) (*p1)->gana=1;
            break;
        case 72:
            (*p1)->dire=0;
            if((*p1)->pos->up &&((*p1)->pos->up->id==0 ||(*p1)->pos->up->id==4))
            {
                (*p1)->pos=(*p1)->pos->up;
                *puntos1-=5;
                printf("p1:%d\n",*puntos1);
            }

            if((*p1)->pos->id==4) (*p1)->gana=1;
            break;
        case 80:
            (*p1)->dire=1;
            if((*p1)->pos->down &&((*p1)->pos->down->id==0 ||(*p1)->pos->down->id==4))
            {
                (*p1)->pos=(*p1)->pos->down;
                *puntos1-=5;
                printf("p1:%d\n",*puntos1);

            }

            if((*p1)->pos->id==4) (*p1)->gana=1;
            break;
        case 119:
            (*p2)->dire=0;
            if((*p2)->pos->up &&((*p2)->pos->up->id==0 ||(*p2)->pos->up->id==4))
            {
                (*p2)->pos=(*p2)->pos->up;
                *puntos2-=5;
                printf("p2:%d\n",*puntos2);
            }

            if((*p2)->pos->id==4) (*p2)->gana=1;
            break;
        case 115:
            (*p2)->dire=1;
            if((*p2)->pos->down && ((*p2)->pos->down->id==0 ||(*p2)->pos->down->id==4))
            {
                (*p2)->pos=(*p2)->pos->down;
                *puntos2-=5;
                printf("p2:%d\n",*puntos2);
            }

            if((*p1)->pos->id==4) (*p2)->gana=1;
            break;
        case 97:
            (*p2)->dire=3;
            if((*p2)->pos->left && ((*p2)->pos->left->id==0 ||(*p2)->pos->left->id==4))
            {
                (*p2)->pos=(*p2)->pos->left;
                *puntos2-=5;
                printf("p2:%d\n",*puntos2);
            }

            if((*p1)->pos->id==4) (*p2)->gana=1;
            break;
        case 100:
            (*p2)->dire=2;
            if((*p2)->pos->right && ((*p2)->pos->right->id==0 ||(*p2)->pos->right->id==4))
            {
                (*p2)->pos=(*p2)->pos->right;
                *puntos2-=5;
                printf("p2:%d\n",*puntos2);
            }

            if((*p1)->pos->id==4) (*p2)->gana=1;
            break;
        }
    }
}
void cargaMapa(NODO_MALLA *cab, PLAYER **p1, PLAYER **p2, MOB **e1,MOB **e2,MOB **e3)  // SE ENCARGA DE CARGAR ALEATORIAMENTE LOS MAPAS
{
    char mapas[5][20]= {"mapa0.txt","mapa1.txt","mapa2.txt","mapa3.txt","mapa4.txt"};
    int i,j;
    NODO_MALLA *aux1,*aux2;
    FILE *a;
    srand(time(0));
    a=fopen(mapas[rand()%5],"r");
    for(i=0,aux1=cab; i<TAM && aux1; i++,aux1=aux1->down)
    {
        for(j=0,aux2=aux1; j<TAM && aux2; j++,aux2=aux2->right)
        {
            fscanf(a,"%d",&aux2->id);
            if(aux2->id==5 && (*p1)->in==0)
            {
                (*p1)->pos=aux2;
                (*p1)->ini=aux2;
                (*p1)->in=1;
                aux2->id=0;
            }
            if(aux2->id==6 &&(*p2)->in==0)
            {
                (*p2)->pos=aux2;
                (*p2)->ini=aux2;
                (*p2)->in=1;
                aux1->id=0;
            }
            if(aux2->id==6 || aux2->id==5 &&((*p1)->in==1 && (*p2)->in==1))
                aux2->id=0;

            if(aux2->id==7 &&(*e1)->in==0)
            {
                (*e1)->pos=aux2;
                (*e1)->in=1;
                aux2->id=0;
            }
            if(aux2->id==8 &&(*e2)->in==0)
            {
                (*e2)->pos=aux2;
                (*e2)->in=1;
                aux2->id=0;
            }
            if(aux2->id==9 &&(*e3)->in==0)
            {
                (*e3)->pos=aux2;
                (*e3)->in=1;
                aux2->id=0;
            }
            if(aux2->id==7 || aux2->id==8 || aux2->id==9 &&((*e1)->in==1 && (*e2)->in==1 && (*e3)->in==1))
                aux2->id=0;
        }
    }
    fclose(a);
}

int Movimientoenemigo(MOB **p1, MOB **p2, MOB **p3)
{
    int dir;
    srand(time(0));
    dir=rand()%4;
    switch(dir)
    {
    case 0:
        (*p1)->dire=(*p2)->dire=(*p3)->dire=1;
        if((*p1)->pos->right &&((*p1)->pos->right->id==0 ||(*p1)->pos->right->id==4))
            (*p1)->pos=(*p1)->pos->right;
        if((*p2)->pos->right &&((*p2)->pos->right->id==0 ||(*p2)->pos->right->id==4))
            (*p2)->pos=(*p2)->pos->right;
        if((*p3)->pos->right &&((*p3)->pos->right->id==0 ||(*p3)->pos->right->id==4))
            (*p3)->pos=(*p3)->pos->right;
        break;
    case 1:
        (*p1)->dire=(*p2)->dire=(*p3)->dire=0;
        if((*p1)->pos->left &&((*p1)->pos->left->id==0 ||(*p1)->pos->left->id==4))
            (*p1)->pos=(*p1)->pos->left;
        if((*p2)->pos->left &&((*p2)->pos->left->id==0 ||(*p2)->pos->left->id==4))
            (*p2)->pos=(*p2)->pos->left;
        if((*p3)->pos->left &&((*p3)->pos->left->id==0 ||(*p3)->pos->left->id==4))
            (*p3)->pos=(*p3)->pos->left;
        break;
    case 2:
        (*p1)->dire=(*p2)->dire=(*p3)->dire=2;
        if((*p1)->pos->up &&((*p1)->pos->up->id==0 ||(*p1)->pos->up->id==4))
            (*p1)->pos=(*p1)->pos->up;
        if((*p2)->pos->up &&((*p2)->pos->up->id==0 ||(*p2)->pos->up->id==4))
            (*p2)->pos=(*p2)->pos->up;
        if((*p3)->pos->up &&((*p3)->pos->up->id==0 ||(*p3)->pos->up->id==4))
            (*p3)->pos=(*p3)->pos->up;
        break;
    case 3:
        (*p1)->dire=(*p2)->dire=(*p3)->dire=3;
        if((*p1)->pos->down &&((*p1)->pos->down->id==0 ||(*p1)->pos->down->id==4))
            (*p1)->pos=(*p1)->pos->down;
        if((*p2)->pos->down &&((*p2)->pos->down->id==0 ||(*p2)->pos->down->id==4))
            (*p2)->pos=(*p2)->pos->down;
        if((*p3)->pos->down &&((*p3)->pos->down->id==0 ||(*p3)->pos->down->id==4))
            (*p3)->pos=(*p3)->pos->down;
        break;
    }
}
void movimientoinicio(PLAYER **p1, PLAYER **p2,MOB **e1,MOB **e2,MOB **e3 )
{

    if((*p1)->pos==((*e1)->pos))
        (*p1)->pos=(*p1)->ini;
    if((*p1)->pos==((*e2)->pos))
        (*p1)->pos=(*p1)->ini;
    if((*p1)->pos==((*e3)->pos))
        (*p1)->pos=(*p1)->ini;

    if((*p2)->pos==((*e1)->pos))
        (*p2)->pos=(*p2)->ini;
    if((*p2)->pos==((*e2)->pos))
        (*p2)->pos=(*p2)->ini;
    if((*p2)->pos==((*e3)->pos))
        (*p2)->pos=(*p2)->ini;

}
int Juego(char *n, char *n2, char*n3, int *puno,int *pdos)  //LE DA INICIO A TODO EL JUEGO
{
    int bandera=0;
    TCabeza Cab;
    PLAYER *p1,*p2;
    MOB *e1,*e2,*e3;
    Cab.cab1=Cab.cab2=NULL;
    TTiempo TGlobal;

    TGlobal.x=TGlobal.y=TGlobal.Ttotal=0;

    int v=0,tecla=0,res=0;
    setactivepage(1);
    creaMalla(&Cab);
    p1=creaJug(Cab.cab1,n);
    p2=creaJug(Cab.cab2,n2);
    e1=creaMob();
    e2=creaMob();
    e3=creaMob();

    cargaSprite(p1->sprite,p2->sprite,Cab.bloques,e1->sprite,e2->sprite,e3->sprite);
    cargaMapa(Cab.cab1,&p1,&p2,&e1,&e2,&e3);
    while(p1->gana==0 && p2->gana==0)
    {
        TGlobal.x=time(0);
        setactivepage(v);
        cleardevice();

        muevejugadores(&p1,&p2,puno,pdos);
        Movimientoenemigo(&e1,&e2,&e3);
        movimientoinicio(&p1,&p2,&e1,&e2,&e3);
        dibuja(p1,p2,Cab,e1,e2,e3);

        outtextxy(510,20,"P1:");
        readimagefile("jugador1.jpg",520,50,600,140);
        outtextxy(540,160,p1->nombre);
        //
        outtextxy(510,190,"P2:");
        readimagefile("jugador2.jpg",520,220,610,310);
        outtextxy(540,330,p2->nombre);

        if(TGlobal.Ttotal==7)
        {
            cargaMapa(Cab.cab1,&p1,&p2,&e1,&e2,&e3);
            TGlobal.Ttotal=0;
        }

        delay(15);
        setvisualpage(v);

        v=!v;
        TGlobal.y=time(0);
        TGlobal.Ttotal+=TGlobal.y-TGlobal.x;
    }
    if(p1->gana==1)
    {
        strcpy(n3,p1->nombre);
        bandera=-1;
    }

    if(p2->gana==1)
    {
        strcpy(n3,p2->nombre);
        bandera=1;

    }

    return bandera;
}

void capturatexto(int x,int y, char *cadena)
{
    //CAPTURA EL TEXTO DE LA PANTALLA
    {
        char tecla, var;
        int contador=0;
        cadena[0]='\0';

        do
        {
            do
            {
                setcolor(15);
                outtextxy(x+textwidth(cadena),y,"_");
                delay(50);
                setcolor(0);
                outtextxy(x+textwidth(cadena),y,"_");
                delay(50);
                setcolor(15);
            }
            while(!kbhit());

            tecla = getch();

            if(tecla==0)
                tecla=getch();
            if(tecla==8 && contador > 0)
            {
                setcolor(BLACK);
                outtextxy(x,y,cadena);
                cadena[--contador]='\0';
                setcolor( WHITE );
                outtextxy(x,y,cadena);
            }
            else
            {
                if(tecla!=13)
                {
                    cadena[contador++]=tecla;
                    cadena[contador]='\0';
                    outtextxy(x,y,cadena);
                }
            }
        }
        while(tecla!=13);
    }
}

void introducirtamano(char *n,char *n2)  //PIDE EL NOMBRE DE LOS JUGADORES
{
    cleardevice();
    settextstyle(GOTHIC_FONT,HORIZ_DIR,1);
    outtextxy(10,100,"Nombre jugador 1: ");
    capturatexto(10+textwidth("Nombre jugador 1: "),100,n);
    cleardevice();
    outtextxy(10,100,"Nombre jugador 2: ");
    capturatexto(10+textwidth("Nombre jugador 2: "),100,n2);
    cleardevice();
}

void MenuInicio(char *n, char *n2)  //MENÚ PRINCIPAL DEL jueguenge
{
    cleardevice();
    int x,y,teclaesc=0,opc=-1;
    BOTON menu[3];
    creabotoninicio(menu);
    Titulos(1);
    while(opc!=0)
    {
        opc=-1;
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN,x,y);
            opc=tocabotonIN(x,y,menu);
            printf("%d ",x);
            printf("%d ",y);
        }
        switch(opc)
        {
        case 1:
            readimagefile("Ayuda.jpg",0,0,680,480);
            rectangle(535,444,616,470);
            outtextxy(550,449,"BACK");
            opc=tocabotonIN(x,y,menu);
            break;
        case 2:
            cleardevice();
            creabotoninicio(menu);
            Titulos(1);
            break;
        }
    }
    introducirtamano(n,n2);
}

#endif // LABERINTO_H_INCLUDED
