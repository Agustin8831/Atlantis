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
//------------- Colision
bool colision(int x1, int y1, int w1, int h1,
              int x2, int y2, int w2, int h2)
{
    return((x1<x2+w2)&&(x2<x1+w1)&&(y1<y2+h2)&&(y2<y1+h1));
}
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
    // Estructuras
    BITMAP *estruc_1 = load_bitmap("./img/estruc_1.bmp", NULL);
    BITMAP *estruc_2 = load_bitmap("./img/estruc_2.bmp", NULL);
    BITMAP *estruc_3 = load_bitmap("./img/estruc_3.bmp", NULL);
    BITMAP *estruc_4 = load_bitmap("./img/estruc_4.bmp", NULL);
    BITMAP *estruc_5 = load_bitmap("./img/estruc_5.bmp", NULL);
    BITMAP *estruc_6 = load_bitmap("./img/estruc_6.bmp", NULL);
    // Naves enemigas
    BITMAP *nave_1 = load_bitmap("./img/nave_1.bmp", NULL);
    BITMAP *buffer = create_bitmap(1620,800);

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
        masked_blit(nave_1,buffer,0,0,700,60,175,57);



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
