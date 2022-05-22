#include <allegro.h>
//---------- Disparos
struct Balas{
    int x , y;
    int dx , dy;
};

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
                 BITMAP* buffer, BITMAP* bala)
{

     if ( n_disparos > 0 && n_disparos < max_disparos){
            for ( int cont = 1; cont <= n_disparos; cont++){
                       disparos[cont].x += disparos[cont].dx;
                       disparos[cont].y += disparos[cont].dy;
                       masked_blit( bala, buffer, 0, 0, disparos[cont].x, disparos[cont].y, 6, 4);
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

//---------- Audio
int inicia_audio(int izquierda, int derecha){
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
       allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
       return 1;
    }
	set_volume(izquierda, derecha);
}
int main(){
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, ANCHO, ALTO, 0, 0);
	inicia_audio(70,70);

     // Creando los BITMAP
    BITMAP *estructura = load_bitmap("./img/estructura_juego.bmp", NULL);
    BITMAP *arma_centro =load_bitmap("./img/arma_centro.bmp", NULL);
    BITMAP *bala = load_bitmap("./img/Bala2.bmp", NULL);
    // Estructuras
    BITMAP *estruc_1 = load_bitmap("./img/estruc_1.bmp", NULL);
    BITMAP *estruc_2 = load_bitmap("./img/estruc_2.bmp", NULL);
    BITMAP *estruc_3 = load_bitmap("./img/estruc_3.bmp", NULL);
    BITMAP *estruc_4 = load_bitmap("./img/estruc_4.bmp", NULL);
    BITMAP *estruc_5 = load_bitmap("./img/estruc_5.bmp", NULL);
    BITMAP *estruc_6 = load_bitmap("./img/estruc_6.bmp", NULL);
    // Naves enemigas
    BITMAP *buffer = create_bitmap(ANCHO, ALTO);

    //----------- Variables
    int dsw = 0, dsww = 0, contt = 0, cont = 0, ndisparos = 0;
    const int maxdisp = 5;
    Balas disparos[maxdisp];
    //-------------------------

    while(!key[KEY_ESC]){
        clear_to_color(buffer,0x000000);
        masked_blit(estructura,buffer,0,0,0,320,1619,495);
        masked_blit(arma_centro,buffer,0,0,725,320,83,48);
        masked_blit(estruc_1,buffer,0,0,148,614,183,57);
        masked_blit(estruc_2,buffer,0,0,755,380,183,57);
        masked_blit(estruc_3,buffer,0,0,962,560,183,57);
        masked_blit(estruc_4,buffer,0,0,1355,448,183,57);
        masked_blit(estruc_5,buffer,0,0,555,448,183,57);
        masked_blit(estruc_6,buffer,0,0,370,505,183,57);

        //---------- Rutina de disparos
        if(key[KEY_SPACE] && !key[KEY_RIGHT] && !key[KEY_LEFT] && dsw ==0 && dsww < 2)
       {
           crear_bala(ndisparos, maxdisp, disparos, 757, 325, 0, -3);
           dsw++;
           dsww++;
       }
       //---------- disparos torreta izq
       else if(key[KEY_SPACE] && !key [KEY_RIGHT] && key [KEY_LEFT] && dsw ==0 && dsww < 2)
       {
           crear_bala(ndisparos, maxdisp, disparos, 70, 380, 5, -1);
           dsw++;
           dsww++;
       }
       //---------- Torreta derecha
       else if(key[KEY_SPACE] && key [KEY_RIGHT] && !key [KEY_LEFT] && dsw ==0 && dsww < 2)
       {
           crear_bala(ndisparos, maxdisp, disparos, 1520, 320, -5, -1);
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
        pintar_bala(ndisparos, maxdisp, disparos, buffer, bala);
        elimina_bala(ndisparos, maxdisp, disparos, ANCHO, ALTO);




       blit(buffer,screen,0,0,0,0,ANCHO,ALTO);
       //rest(1);
    }


    destroy_bitmap(buffer);
	return 0;
}
END_OF_MAIN();