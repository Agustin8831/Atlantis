#include <iostream>
#include <allegro.h>
using namespace std;
//--------------
const int maxdisp = 5;
int ndisparos = 0;
int cont = 0;
int contt = 0;
int dsw = 0;
int dsww = 0;

struct Balas{
   int x, y;
   int dx, dy;
}disparos[maxdisp];
//-------------


int main (){
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,1620,800,0,0);

    // Creando los BITMAP
    BITMAP *estructura = load_bitmap("./img/estructura_juego.bmp", NULL);
    BITMAP *arma_centro =load_bitmap("./img/arma_centro.bmp", NULL);
    BITMAP *bala = load_bitmap("./img/Bala2.bmp", NULL);
    BITMAP *buffer = create_bitmap(1620,800);

    while(!key[KEY_ESC]){
        clear_to_color(buffer,0x000000);
        masked_blit(estructura,buffer,0,0,0,320,1619,495);
        masked_blit(arma_centro,buffer,0,0,725,320,83,48);

////rutina de disparos
       if(key[KEY_SPACE] && !key[KEY_RIGHT] && !key[KEY_LEFT] && dsw ==0 && dsww < 2)
       {
         ndisparos++;
         disparos[ndisparos].x = 757;
         disparos[ndisparos].y = 325;
         disparos[ndisparos].dx = 0;
         disparos[ndisparos].dy = -3;
         dsw ++;
         dsww++;
        if(ndisparos < maxdisp)
        {
        }

       }//disparos torreta izq
       else if(key[KEY_SPACE] && !key [KEY_RIGHT] && key [KEY_LEFT] && dsw ==0 && dsww < 2)
       {
         ndisparos++;
         disparos[ndisparos].x = 77;
         disparos[ndisparos].y = 390;
         disparos[ndisparos].dx = +5;
         disparos[ndisparos].dy = -3;
         dsw ++;
         dsww++;
       }//Torreta derechaa (corregir las coordenadas de inicio)
       else if(key[KEY_SPACE] && key [KEY_RIGHT] && !key [KEY_LEFT] && dsw ==0 && dsww < 2)
       {
         ndisparos++;
         disparos[ndisparos].x = 1536; //estas también
         disparos[ndisparos].y = 330; //estas coordenadas
         disparos[ndisparos].dx = -5;
         disparos[ndisparos].dy = -3;
         dsw ++;
         dsww++;
       }
       else
       if (contt++ > 100){
            dsw = 0; contt =0;
            if (cont++ > 3){
                dsww = 0;cont=0;
            }
        }
       if(ndisparos > 0)
       {
        for(int cont = 1; cont <= ndisparos; cont++)
        {
         disparos[cont].x += disparos[cont].dx;
         disparos[cont].y += disparos[cont].dy;
         masked_blit(bala, buffer, 0, 0,disparos[cont].x ,disparos[cont].y, 6, 6);
        }
       }

        blit(buffer,screen,0,0,0,0,1620,800);
    };
    destroy_bitmap(buffer);
    return 0;
}
END_OF_MAIN();
