#include <allegro.h>
#include <stdlib.h>
#include <time.h>
//---------- Estructuras
struct Balas{
    int x , y;
    int dx , dy;
};
struct ARMAS{
    int x, y;
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
    void dispara(struct Balas disparos[], BITMAP* buffer);
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

bool elimina_bala_objeto(struct ARMAS& N, struct ARMAS& E, struct Balas B[]){
    if ( N.n_disp > 0 && N.n_disp < N.max_disp){
            for ( int cont = 1; cont <= N.n_disp; cont++){
                if(colision(E.x, E.y, E.ancho_p, E.alto_p, B[cont].x , B[cont].y, N.ancho_b, N.alto_b)&& E.vida > 0){
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
//-------------------------------
void ARMAS::dispara(struct Balas disparos[], BITMAP* buffer){
        //----------- Variables
        int dsw = 0, dsww = 0, contt = 0, cont = 0;
        //---------- Rutina de disparos
        if(key[KEY_SPACE] && !key[KEY_RIGHT] && !key[KEY_LEFT] && dsw ==0 && dsww < 2)
       {
           crear_bala(n_disp, max_disp, disparos, 757, 325, 0, -2);
           dsw++;
           dsww++;
       }
       //---------- disparos torreta izq
       else if(key[KEY_SPACE] && !key [KEY_RIGHT] && key [KEY_LEFT] && dsw ==0 && dsww < 2)
       {
           crear_bala(n_disp, max_disp, disparos, 70, 380, 5, -1);
           dsw++;
           dsww++;
       }
       //---------- Torreta derecha
       else if(key[KEY_SPACE] && key [KEY_RIGHT] && !key [KEY_LEFT] && dsw ==0 && dsww < 2)
       {
           crear_bala(n_disp, max_disp, disparos, 1520, 320, -5, -1);
           dsw++;
           dsww++;
       }
       //----------
       if (contt++ > 60){
            dsw = 0; contt = 0;
            if (cont++ > 6){
                dsww = 0;
                cont=0;
            }
        }
       //---------- Pintando y Eliminando la bala
        pintar_bala(n_disp, max_disp, disparos, buffer, img_bala, ancho_b, alto_b);
        elimina_bala(n_disp, max_disp, disparos, ANCHO, ALTO);
}
void ARMAS::dispara_E(struct Balas disparos[], BITMAP* buffer){
        crear_bala(n_disp, max_disp, disparos, x + 80, y + 40, 0, 1);
        pintar_bala(n_disp, max_disp, disparos, buffer, img_bala, ancho_b, alto_b);
        elimina_bala(n_disp, max_disp, disparos, ANCHO, ALTO);
}
//---------- Crear y Pintar todos los enemigos
void acomoda_enemigos(struct ARMAS E[]){
    int indice = -1;
    for(int i = 0; i < 1; i++){
            for(int j = 0; j < 6; j++){
                indice++;
                E[indice].inicia("./img/nave_1.bmp", "./img/Bala2.bmp", 6, 12, 172, 56, 280 + j * 200, 100 + i * 100, 1);
            }
    }
}
void pintar_enemigo(struct ARMAS E[], BITMAP* buffer){
    int indice = -1;
    for(int i = 0; i < 1; i++){
            for(int j = 0; j < 6; j++){
                indice++;
                if(E[indice].vida > 0){
                    E[indice].pinta(buffer);
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
    rest(50);
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
//---------- Audio
int inicia_audio(int izquierda, int derecha){
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
       allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
       return 1;
    }
	set_volume(izquierda, derecha);
}
//---------------------------
int main(){
    srand(time(NULL));
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, ANCHO, ALTO, 0, 0);
	inicia_audio(70,70);

	// Estructuras
    BITMAP *estructura = load_bitmap("./img/estructura_juego.bmp", NULL);
    BITMAP *estruc_1 = load_bitmap("./img/estruc_1.bmp", NULL);
    BITMAP *estruc_2 = load_bitmap("./img/estruc_2.bmp", NULL);
    BITMAP *estruc_3 = load_bitmap("./img/estruc_3.bmp", NULL);
    BITMAP *estruc_4 = load_bitmap("./img/estruc_4.bmp", NULL);
    BITMAP *estruc_5 = load_bitmap("./img/estruc_5.bmp", NULL);
    BITMAP *estruc_6 = load_bitmap("./img/estruc_6.bmp", NULL);
    BITMAP *buffer = create_bitmap(ANCHO, ALTO);


    //-------------------------
    int azar = rand() % 6;

    ARMAS N;
    N.inicia("./img/arma_centro.bmp", "./img/Bala2.bmp", 6, 5, 83, 48, 725, 515, 1);

    ARMAS E[6];
    acomoda_enemigos(E);

    Balas disparos[N.max_disp];
    Balas disp_E[E[0].max_disp];
    //-------------------------
    while(!key[KEY_ESC]){
        clear_to_color(buffer,0x000000);
        masked_blit(estructura,buffer,0,0,0,510,1619,495);
        masked_blit(estruc_1,buffer,0,0,148,805,183,57);
        masked_blit(estruc_2,buffer,0,0,755,570,183,57);
        masked_blit(estruc_3,buffer,0, 0, 962,750,183,57);
        masked_blit(estruc_4,buffer,0,0,1355,638,183,57);
        masked_blit(estruc_5,buffer,0,0,555,638,183,57);
        masked_blit(estruc_6,buffer,0,0,370,695,183,57);

        N.pinta(buffer);
        N.dispara(disparos, buffer);

        for(int i = 0; i < 6; i++){
                if(elimina_bala_objeto(N,E[i],disparos)){
                    explosion_1(E[i], buffer);
                }
        }

        pintar_enemigo(E, buffer);
        if(E[azar].n_disp == 0){
            azar = rand() % 6;
        }
        E[azar].dispara_E(disp_E, buffer);
        if(elimina_bala_objeto(E[azar], N, disp_E)){
            explosion_2(N,buffer);
        }

        blit(buffer,screen,0,0,0,0,ANCHO,ALTO);
    }
    destroy_bitmap(buffer);
	return 0;
}
END_OF_MAIN();
