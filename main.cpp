#include <allegro.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
using namespace std;
int estructura_eliminada_contador = 0;
int contador = 0;
int nivel = 1;
//---------- Estructuras
struct Balas{
    int x , y;
    int dx , dy;
};
struct ARMAS{
    float x, y;
    int n_disp;
    int max_disp;
    int ancho_b, alto_b;
    int ancho_p, alto_p;
    int vida;

    BITMAP* img_arma;
    BITMAP* img_bala;
    BITMAP* exp_enem_1;
    BITMAP* exp_enem_2;
    BITMAP* exp_enem_3;

    void inicia(char* ruta_arma, char* ruta_bala, int _ancho_b, int _alto_b,
                int _ancho_p, int _alto_p,int _x, int _y, int _vida);
    void pinta(BITMAP* buffer);
    void dispara_E(struct Balas disparos[], BITMAP* buffer);
};
void ARMAS::inicia(char* ruta_arma, char* ruta_bala, int _ancho_b, int _alto_b,
                   int _ancho_p, int _alto_p, int _x, int _y, int _vida){
    x = _x; y = _y;
    n_disp = 0; max_disp = 2;
    img_arma = load_bitmap(ruta_arma, NULL);
    img_bala = load_bitmap(ruta_bala, NULL);
    ancho_b = _ancho_b; alto_b = _alto_b;
    ancho_p = _ancho_p; alto_p = _alto_p;
    vida = _vida;
    exp_enem_1 = load_bitmap("./img/destruccion_1.bmp",NULL);
    exp_enem_2 = load_bitmap("./img/destruccion_2.bmp",NULL);
    exp_enem_3 = load_bitmap("./img/destruccion_3.bmp",NULL);
}
void ARMAS::pinta(BITMAP* buffer){
    masked_blit(img_arma,buffer,0,0,x,y,ancho_p,alto_p);
}

//---------- Disparos
bool colision(int x1, int y1, int w1 , int h1,
              int x2, int y2, int w2 , int h2){
    return ((x1 < x2 + w2)&&(x2 < x1 + w1)&&(y1<y2+h2)&&(y2<y1+h1));
}

void crear_bala(int& n_disparos, const int max_disparos ,struct Balas disparos[] ,
                const int X, const int Y , const int dx, const int dy)
{
              if( n_disparos < max_disparos-1){ // n -1 disparos permitidos
                   n_disparos++;
                   disparos[n_disparos].x  = X;
                   disparos[n_disparos].y  = Y;
                   disparos[n_disparos].dx = dx;
                   disparos[n_disparos].dy = dy;
               }
}

void pintar_bala(int& n_disparos, const int max_disparos,struct Balas disparos[],
                 BITMAP* buffer, BITMAP* bala, int ancho, int alto)
{
     if ( n_disparos > 0 && n_disparos < max_disparos){
            for ( int cont = 1; cont <= n_disparos; cont++){
                       disparos[cont].x += disparos[cont].dx;
                       disparos[cont].y += disparos[cont].dy;
                       masked_blit( bala, buffer, 0, 0, disparos[cont].x, disparos[cont].y, ancho, alto);
            }
     }
}

void eliminar(struct Balas disparos[], int& n_disparos, int cont){
    Balas Btemp;
    Btemp = disparos[cont];
    disparos[cont] = disparos[n_disparos];
    disparos[n_disparos] = Btemp;
    n_disparos--;
    if ( n_disparos < 0 ) n_disparos=0;
}

void elimina_bala(int& n_disparos, const int max_disparos,struct Balas disparos[],
                  const int ANCHO, const int ALTO)
{
      if ( n_disparos > 0 && n_disparos < max_disparos){
            for ( int cont = 1; cont <= n_disparos; cont++){
                      if ( disparos[cont].y > ALTO || disparos[cont].y < 0 ||
                           disparos[cont].x > ANCHO|| disparos[cont].x < 0  )
                       {
                           eliminar(disparos, n_disparos, cont);
                       }
            }
      }
}
//------- Eliminador de naves enemigas
bool elimina_bala_objeto(struct ARMAS& N, struct ARMAS& E, struct Balas B[]){
    if ( N.n_disp > 0 && N.n_disp < N.max_disp){
            for ( int cont = 1; cont <= N.n_disp; cont++){
                if(colision(E.x, E.y, E.ancho_p, E.alto_p,
                            B[cont].x , B[cont].y, N.ancho_b, N.alto_b)&& E.vida > 0){
                    eliminar(B,N.n_disp,cont);
                    E.vida--;
                    return true;
                }
            }
            return false;
    }
}
//---------- Medida de la Pantalla
#define ANCHO 1620
#define ALTO  1000
//----------- Dispara Enemigo
void ARMAS::dispara_E(struct Balas disparos[], BITMAP* buffer){
        crear_bala(n_disp, max_disp, disparos, x + 80, y + 40, 0, 1);
        pintar_bala(n_disp, max_disp, disparos, buffer, img_bala, ancho_b, alto_b);
        elimina_bala(n_disp, max_disp, disparos, ANCHO, ALTO);
}
//-------------- Eliminando Y Crear Estructuras
struct estructuras {
    int dan;
};
void iniciar_estructuras (struct estructuras edificios[]){
    for(int i = 0; i < 6; i++){
        edificios[i].dan = 0;
    };
};
void pintar_estructuras(struct estructuras Edi[],BITMAP* buffer,
                        BITMAP* estruc_1,BITMAP* estruc_2,BITMAP* estruc_3,
                        BITMAP* estruc_4,BITMAP* estruc_5,BITMAP* estruc_6, int contador){
    if(contador == 6){
        Edi[0].dan = 0;
        Edi[1].dan = 0;
        Edi[2].dan = 0;
        Edi[3].dan = 0;
        Edi[4].dan = 0;
        Edi[5].dan = 0;
    }
    if(Edi[0].dan != 1){
        masked_blit(estruc_1,buffer,0,0,148,805,183,57);
    }
    if(Edi[1].dan != 1){
        masked_blit(estruc_2,buffer,0,0,755,570,183,57);
    }
    if(Edi[2].dan != 1){
        masked_blit(estruc_3,buffer,0, 0, 962,750,183,57);
    }
    if(Edi[3].dan != 1){
        masked_blit(estruc_4,buffer,0,0,1355,638,183,57);
    }
    if(Edi[4].dan != 1){
        masked_blit(estruc_5,buffer,0,0,555,638,183,57);
    }
    if(Edi[5].dan != 1){
        masked_blit(estruc_6,buffer,0,0,370,695,183,57);
    }
}
//------- Eliminador de estructuras
void elimina_bala_estructuras(struct ARMAS &N, struct estructuras Edi[], struct Balas B[]){
    if ( N.n_disp > 0 && N.n_disp < N.max_disp){
        for ( int cont = 1; cont <= N.n_disp; cont++){
            if(colision(148,805,183,57,B[cont].x, B[cont].y,
                N.ancho_b, N.alto_b) && Edi[0].dan < 1){
                eliminar(B,N.n_disp,cont);
                Edi[0].dan++;
                estructura_eliminada_contador++;
            }
            if(colision(755,570,183,57,B[cont].x, B[cont].y,
                N.ancho_b, N.alto_b) && Edi[1].dan < 1){
                eliminar(B,N.n_disp,cont);
                Edi[1].dan++;
                estructura_eliminada_contador++;
            }
            if(colision(962,750,183,57,B[cont].x, B[cont].y,
                N.ancho_b, N.alto_b) && Edi[2].dan < 1){
                eliminar(B,N.n_disp,cont);
                Edi[2].dan++;
                estructura_eliminada_contador++;
            }
            if(colision(1355,638,183,57,B[cont].x, B[cont].y,
                N.ancho_b, N.alto_b) && Edi[3].dan < 1){
                eliminar(B,N.n_disp,cont);
                Edi[3].dan++;
                estructura_eliminada_contador++;
            }
            if(colision(555,638,183,57,B[cont].x, B[cont].y,
                N.ancho_b, N.alto_b) && Edi[4].dan < 1){
                eliminar(B,N.n_disp,cont);
                Edi[4].dan++;
                estructura_eliminada_contador++;
            }
            if(colision(370,695,183,57,B[cont].x, B[cont].y,
                N.ancho_b, N.alto_b) && Edi[5].dan < 1){
                eliminar(B,N.n_disp,cont);
                Edi[5].dan++;
                estructura_eliminada_contador++;
            }
        }
    }
}
//---------- Crear y Pintar todos los enemigos
void acomoda_enemigos(struct ARMAS E[]){
    E[0].inicia("./img/nave_1.bmp", "./img/Bala2.bmp", 6, 12, 172, 56,-6000,0, 1);
    E[1].inicia("./img/nave_4.bmp", "./img/Bala2.bmp", 6, 12, 172, 56,4600,210, 1);
    E[2].inicia("./img/nave_5.bmp", "./img/Bala2.bmp", 6, 12, 172, 56,-2600,280, 1);

    E[3].inicia("./img/nave_2.bmp", "./img/Bala2.bmp", 6, 12, 172, 56,3000,70, 1);
    E[4].inicia("./img/nave_3.bmp", "./img/Bala2.bmp", 6, 12, 172, 56,-2000,140, 1);
    E[5].inicia("./img/nave_6.bmp", "./img/Bala2.bmp", 6, 12, 172, 56,1900,350, 1);
}
void pintar_enemigo(struct ARMAS E[], BITMAP* buffer){
    if(nivel == 1){
        if(contador <= 4){
            if(E[1].vida > 0){
                E[1].pinta(buffer);
            }
            if(E[2].vida > 0){
                E[2].pinta(buffer);
            }
            if(E[3].vida > 0){
                E[3].pinta(buffer);
            }
            if(E[4].vida > 0){
                E[4].pinta(buffer);
            }
            if(E[5].vida > 0){
                E[5].pinta(buffer);
            }
        }
        else{
            if(E[0].vida > 0){
                E[0].pinta(buffer);
            }
        }
    }
    if(nivel == 2){
        if(contador <= 3){
            if(E[2].vida > 0){
                E[2].pinta(buffer);
            }
            if(E[3].vida > 0){
                E[3].pinta(buffer);
            }
            if(E[4].vida > 0){
                E[4].pinta(buffer);
            }
            if(E[5].vida > 0){
                E[5].pinta(buffer);
            }
        }
        else{
            if(E[0].vida > 0){
                E[0].pinta(buffer);
            }
            if(E[1].vida > 0){
                E[1].pinta(buffer);
            }
        }

    }
    if(nivel >= 3){
        if(contador <= 2){
            if(E[3].vida > 0){
                E[3].pinta(buffer);
            }
            if(E[4].vida > 0){
                E[4].pinta(buffer);
            }
            if(E[5].vida > 0){
                E[5].pinta(buffer);
            }
        }
        else{
            if(E[0].vida > 0){
                E[0].pinta(buffer);
            }
            if(E[1].vida > 0){
                E[1].pinta(buffer);
            }
            if(E[2].vida > 0){
                E[2].pinta(buffer);
            }
        }
    }
}

void explosion_1(struct ARMAS E, BITMAP* buffer){
    BITMAP* parche = create_bitmap(172,56);
    clear_to_color(parche,0x000000);
    blit(parche, buffer, 0, 0,E.x, E.y, 172,56);
    masked_blit(E.exp_enem_1, buffer, 0, 0, E.x - 10, E.y, 135, 58);
    masked_blit(E.exp_enem_2, buffer, 0, 0, E.x - 10, E.y, 135, 58);
    masked_blit(E.exp_enem_3, buffer, 0, 0, E.x - 10, E.y, 135, 58);
    blit(buffer,screen, 0, 0, 0, 0, ANCHO, ALTO);
    rest(20);
}
void explosion_2(struct ARMAS N, BITMAP* buffer){
    BITMAP* parche = create_bitmap(83, 48);
    clear_to_color(parche,0x000000);
    blit(parche, buffer, 0 , 0, N.x, N.y, 83, 48);
    masked_blit(N.exp_enem_1, buffer, 0, 0, N.x , N.y, 135, 58);
    masked_blit(N.exp_enem_2, buffer, 0, 0, N.x , N.y, 135, 58);
    masked_blit(N.exp_enem_3, buffer, 0, 0, N.x , N.y, 135, 58);
    blit(buffer,screen, 0, 0, 0, 0, ANCHO, ALTO);
    rest(200);
}
void crear_bala_enemigo(struct ARMAS E[], int &azar, int num_naves_enemigas){
    if(E[azar].n_disp == 0){
        azar = rand() % num_naves_enemigas;
        while(E[azar].vida == 0){azar = rand() % num_naves_enemigas;}
    }
}
void crear_bala_nave(struct ARMAS& N, struct Balas disparos[],BITMAP* buffer){
    //----------- Variables
    int dsw = 0, dsww = 0, contt = 0, cont = 0;
    //---------- Rutina de disparos
     if(key[KEY_SPACE] && !key[KEY_RIGHT] && !key[KEY_LEFT] && dsw ==0 && dsww < 2)
       {
           crear_bala(N.n_disp, N.max_disp, disparos, 757, 510, 0, -2);
           dsw++;
           dsww++;
       }
       //---------- disparos torreta izq
       else if(key[KEY_SPACE] && !key [KEY_RIGHT] && key [KEY_LEFT] && dsw ==0 && dsww < 2)
       {
           crear_bala(N.n_disp, N.max_disp, disparos, 100, 570, 3, -1);
           dsw++;
           dsww++;
       }
       //---------- Torreta derecha
       else if(key[KEY_SPACE] && key [KEY_RIGHT] && !key [KEY_LEFT] && dsw ==0 && dsww < 2)
       {
           crear_bala(N.n_disp, N.max_disp, disparos, 1520, 510, -3, -1);
           dsw++;
           dsww++;
       }
       //----------
       if (contt++ > 60){
            dsw = 0; contt = 0;
            if (cont++ > 6){
                dsww = 0;
                cont =0;
            }
        }
       //---------- Pintando y Eliminando la bala
        pintar_bala(N.n_disp, N.max_disp, disparos, buffer, N.img_bala, N.ancho_b, N.alto_b);
        elimina_bala(N.n_disp, N.max_disp, disparos, ANCHO, ALTO);
}
void mover_enemigos(struct ARMAS E[],
    float dir_1, float dir_2,float dir_3,
    float dir_4,float dir_5,float dir_6, float distancia){
    for(int i = 0; i< 7; i++){
        E[0].x += dir_1 / 10 ;
        E[2].x += dir_2 / 10;
        E[4].x += dir_3 / 10;

        E[1].x -= dir_4 / 10;
        E[3].x -= dir_5 / 10;
        E[5].x -= dir_6 / 10;

        if(E[0].x > 1620 ){
            E[0].x = -6000 - distancia;
        }
        if(E[2].x > 1620){
            E[2].x = -2000 - distancia;
        }
        if(E[4].x > 1620){
            E[4].x = -2600 - distancia;
        }

        if(E[1].x < 0){
            E[1].x = 3000 + distancia;
        }
        if(E[3].x < 0){
            E[3].x = 4600 + distancia;
        }
        if(E[5].x < 0){
            E[5].x = 1900 + distancia;
        }
    }
}

//---------------------------
int main(){
    srand(time(NULL));
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, ANCHO, ALTO, 0, 0);

	// Estructuras
	BITMAP* estruc_1 = load_bitmap("./img/estruc_1.bmp", NULL);
    BITMAP* estruc_2 = load_bitmap("./img/estruc_2.bmp", NULL);
    BITMAP* estruc_3 = load_bitmap("./img/estruc_3.bmp", NULL);
    BITMAP* estruc_4 = load_bitmap("./img/estruc_4.bmp", NULL);
    BITMAP* estruc_5 = load_bitmap("./img/estruc_5.bmp", NULL);
    BITMAP* estruc_6 = load_bitmap("./img/estruc_6.bmp", NULL);
    BITMAP *estructura = load_bitmap("./img/estructura_juego.bmp", NULL);
    BITMAP *menu_con = load_bitmap("./img/menu_controles.bmp",NULL);
    BITMAP *Press_start = load_bitmap("./img/Press_start.bmp",NULL);
    BITMAP *buffer = create_bitmap(ANCHO, ALTO);

    //-----------------
    int naves_enemigas_contador = 0;
    int velocidad = 3;
    int num_naves_enemigas = 1;

    int puntos = 0;

    int cont=0;
    int azar = rand()%3;
    int valor = 0;
    float dificultad = 0;
    float distancia = 100;


    ARMAS N;
    N.inicia("./img/arma_centro.bmp", "./img/Bala2.bmp", 6, 5, 83, 48, 725, 515, 1);

    ARMAS E[7];
    acomoda_enemigos(E);

    ARMAS Estruc[6];

    Balas disparos[N.max_disp];
    Balas disp_E[E[0].max_disp];

    estructuras Edi[7];
    iniciar_estructuras(Edi);


    //-------------------------
     bool salida = false;
            while (!salida){
            clear_to_color(buffer,0x000000);
            masked_blit(estructura,buffer,0,0,0,510,1619,495);

            pintar_estructuras(Edi,buffer,estruc_1,estruc_2,estruc_3,estruc_4,estruc_5,estruc_6, contador);
            pintar_enemigo(E, buffer);
            N.pinta(buffer);
            masked_blit(menu_con,buffer,0,0,0,0,ANCHO,ALTO);

            if (cont <=150){
            masked_blit(Press_start,buffer,0,0,440,440,ANCHO,ALTO);
            }
            if (cont++ == 300){
                cont =0;
            }

            blit(buffer,screen,0,0,0,0,ANCHO,ALTO);
            if (key[KEY_A] || key[KEY_ESC]){
                salida=true;
                }
            }
    while(!key[KEY_ESC]){
        clear_to_color(buffer,0x000000);
        masked_blit(estructura,buffer,0,0,0,510,1619,495);

        N.pinta(buffer);
        crear_bala_nave(N,disparos,buffer);
        pintar_enemigo(E, buffer);

        pintar_estructuras(Edi,buffer,estruc_1,estruc_2,estruc_3,estruc_4,estruc_5,estruc_6, contador);

        // Velocidad de las naves enemiogas
        float dir_1 = 1 + rand() % velocidad;
        float dir_2 = 1 + rand() % velocidad;
        float dir_3 = 1 + rand() % velocidad;
        float dir_4 = 1 + rand() % velocidad;
        float dir_5 = 1 + rand() % velocidad;
        float dir_6 = 1 + rand() % velocidad;
        mover_enemigos(E,dir_1,dir_2,dir_3,dir_4,dir_5,dir_6, distancia);

        // Eliminador de naves enemigas
        if(nivel == 1){
            if(contador <= 5){
                for(int i = 1; i <= 5; i++){
                    if(elimina_bala_objeto(N,E[i],disparos)){
                        explosion_1(E[i], buffer);
                        naves_enemigas_contador++;
                        contador++;
                        puntos += 100;
                    }
                }
                if(elimina_bala_objeto(N,E[0],disparos)){
                    explosion_1(E[0], buffer);
                    naves_enemigas_contador++;
                    contador++;
                    puntos += 100;
                }
            }
        }
        if(nivel == 2){
            if(contador <= 4){
                for(int i = 2; i <= 5; i++){
                    if(elimina_bala_objeto(N,E[i],disparos)){
                        explosion_1(E[i], buffer);
                        naves_enemigas_contador++;
                        contador++;
                        puntos += 200;
                    }
                }
            }
            for(int j = 0; j <= 1; j++){
                if(elimina_bala_objeto(N,E[j],disparos)){
                    explosion_1(E[j], buffer);
                    naves_enemigas_contador++;
                    contador++;
                    puntos += 200;
                }
            }
        }
        if(nivel >= 3){
            if(contador <= 3){
                for(int i = 3; i <= 5; i++){
                    if(elimina_bala_objeto(N,E[i],disparos)){
                        explosion_1(E[i], buffer);
                        naves_enemigas_contador++;
                        contador++;
                        switch(nivel){
                            case 3:
                                puntos += 300;
                            break;
                            case 4:
                                puntos += 400;
                            break;
                            case 5:
                                puntos += 500;
                            break;
                            case 6:
                                puntos += 600;
                            break;
                            case 7:
                                puntos += 700;
                            break;
                            case 8:
                                puntos += 800;
                            break;
                            default:
                                if(nivel > 8){
                                    puntos += 900;
                                }
                        }
                    }
                }
            }
            for(int j = 0; j <= 2; j++){
                if(elimina_bala_objeto(N,E[j],disparos)){
                    explosion_1(E[j], buffer);
                    naves_enemigas_contador++;
                    contador++;
                    switch(nivel){
                        case 3:
                            puntos += 300;
                        break;
                        case 4:
                            puntos += 400;
                        break;
                        case 5:
                            puntos += 500;
                        break;
                        case 6:
                            puntos += 600;
                        break;
                        case 7:
                            puntos += 700;
                        break;
                        case 8:
                            puntos += 800;
                        break;
                        default:
                            if(nivel > 8){
                                puntos += 900;
                            }
                    }
                }
            }
        }
        // Puntos por nivel


        // Restableciendo las vidas de las naves enemigas
        if(contador == 6){
            E[0].vida = 1;
            E[1].vida = 1;
            E[2].vida = 1;
            E[3].vida = 1;
            E[4].vida = 1;
            E[5].vida = 1;
            E[1].x = 3000;
            E[3].x = 4600;
            E[5].x = 1900;
            E[0].x = -6000;
            E[2].x = -2000;
            E[4].x = -2600;

            contador = 0;
            velocidad++;
            if(num_naves_enemigas < 3){
                num_naves_enemigas++;
            }
            distancia += 100;
            puntos += 1000;
            nivel++;

        }
        // Reconstruyendo las estructuras
        if(key[KEY_1] && puntos >= 5000){
            iniciar_estructuras(Edi);
            puntos -= 5000;
            estructura_eliminada_contador = 0;
        }
        // ------------ Texto
        textprintf(buffer, font, 1100, 20, palette_color[10],"Nivel: %d", nivel);
        textprintf(buffer, font, 1200, 20, palette_color[10],"Naves enemigas destruidas: %d", naves_enemigas_contador);
        textprintf(buffer, font, 1450, 20, palette_color[10],"Puntaje: %d", puntos);

        // Terminando el juego
        if(estructura_eliminada_contador == 6){break;}

        crear_bala_enemigo(E,azar, num_naves_enemigas);
        if(elimina_bala_objeto(E[azar], N, disp_E)){
            explosion_2(N,buffer);
        }
        elimina_bala_estructuras(E[azar],Edi,disp_E);
        E[azar].dispara_E(disp_E, buffer);


        blit(buffer,screen,0,0,0,0,ANCHO,ALTO);
    }
    destroy_bitmap(buffer);
	return 0;
}
END_OF_MAIN();
