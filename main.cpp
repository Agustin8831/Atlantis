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

    BITMAP* img_arma;
    BITMAP* img_bala;

    void inicia(char* ruta_arma, char* ruta_bala, int _ancho_b, int _alto_b,
                int _ancho_p, int _alto_p,int _x, int _y);
    void pinta(BITMAP* buffer);
    void dispara(struct Balas disparos[], BITMAP* buffer);
    void dispara_E(struct Balas disparos[], BITMAP* buffer);
};
void ARMAS::inicia(char* ruta_arma, char* ruta_bala, int _ancho_b, int _alto_b,
                   int _ancho_p, int _alto_p, int _x, int _y){
    x = _x; y = _y;
    n_disp = 0; max_disp = 2;
    img_arma = load_bitmap(ruta_arma, NULL);
    img_bala = load_bitmap(ruta_bala, NULL);
    ancho_b = _ancho_b; alto_b = _alto_b;
    ancho_p = _ancho_p; alto_p = _alto_p;
}
void ARMAS::pinta(BITMAP* buffer){
    masked_blit(img_arma,buffer,0,0,x,y,ancho_p,alto_p);
}

//---------- Disparos
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

void elimina_bala(int& n_disparos, const int max_disparos,struct Balas disparos[],
                  const int ANCHO, const int ALTO)
{
      Balas Btemp;
      if ( n_disparos > 0 && n_disparos < max_disparos){
            for ( int cont = 1; cont <= n_disparos; cont++){
                      if ( disparos[cont].y > ALTO || disparos[cont].y < 0 ||
                           disparos[cont].x > ANCHO|| disparos[cont].x < 0  )
                       {
                                Btemp = disparos[cont];
                                disparos[cont] = disparos[n_disparos];
                                disparos[n_disparos] = Btemp;
                                n_disparos--;
                                if ( n_disparos < 0 ) n_disparos=0;
                       }
            }
      }
}

//---------- Medida de la Pantalla
#define ANCHO 1620
#define ALTO  800
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
                E[indice].inicia("./img/nave_1.bmp", "./img/Bala2.bmp", 6, 12, 172, 56, 140 + j * 200, 100 + i * 100);
            }
    }
}
void pintar_enemigo(struct ARMAS E[], BITMAP* buffer){
    int indice = -1;
    for(int i = 0; i < 1; i++){
            for(int j = 0; j < 6; j++){
                indice++;
                E[indice].pinta(buffer);
            }
    }
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

    int azar = rand() % 6;

    // Creando los BITMAP
    ARMAS N;
    N.inicia("./img/arma_centro.bmp", "./img/Bala2.bmp", 6, 5, 83, 48, 725, 320);

    ARMAS E[6];
    acomoda_enemigos(E);

    Balas disparos[N.max_disp];
    Balas disp_E[E[0].max_disp];
    //-------------------------


    // Estructuras
    BITMAP *estructura = load_bitmap("./img/estructura_juego.bmp", NULL);
    BITMAP *estruc_1 = load_bitmap("./img/estruc_1.bmp", NULL);
    BITMAP *estruc_2 = load_bitmap("./img/estruc_2.bmp", NULL);
    BITMAP *estruc_3 = load_bitmap("./img/estruc_3.bmp", NULL);
    BITMAP *estruc_4 = load_bitmap("./img/estruc_4.bmp", NULL);
    BITMAP *estruc_5 = load_bitmap("./img/estruc_5.bmp", NULL);
    BITMAP *estruc_6 = load_bitmap("./img/estruc_6.bmp", NULL);
    BITMAP *buffer = create_bitmap(ANCHO, ALTO);


    while(!key[KEY_ESC]){
        clear_to_color(buffer,0x000000);
        masked_blit(estructura,buffer,0,0,0,320,1619,495);
        masked_blit(estruc_1,buffer,0,0,148,614,183,57);
        masked_blit(estruc_2,buffer,0,0,755,380,183,57);
        masked_blit(estruc_3,buffer,0,0,962,560,183,57);
        masked_blit(estruc_4,buffer,0,0,1355,448,183,57);
        masked_blit(estruc_5,buffer,0,0,555,448,183,57);
        masked_blit(estruc_6,buffer,0,0,370,505,183,57);


        N.pinta(buffer);
        N.dispara(disparos, buffer);

        pintar_enemigo(E, buffer);
        if(E[azar].n_disp == 0){
            azar = rand() % 6;
        }
        E[azar].dispara_E(disp_E, buffer);

        blit(buffer,screen,0,0,0,0,ANCHO,ALTO);
    }
    destroy_bitmap(buffer);
	return 0;
}
END_OF_MAIN();
