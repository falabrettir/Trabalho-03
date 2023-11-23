#include "trabalho3.h"
#include "imagem.h"
#include <math.h>

double calculaAngulo(Coordenada l, Coordenada r);
Coordenada* achaCentros(Imagem1C img);
double detectaSensorBar(Imagem1C* img, Coordenada* l, Coordenada* r);
void diminuiRuido(Imagem1C* img);


double detectaSensorBar(Imagem1C* img, Coordenada* l, Coordenada* r)
{
    //diminuiRuido(img);
    return 0.0;
}

double calculaAngulo(Coordenada l, Coordenada r)
{
    return 0.0;
}

Coordenada* achaCentros(Imagem1C img)
{
    // Acha o centro de uma bola
}

void diminuiRuido(Imagem1C* img)
{
    int i, j, soma, media;

    for (i = 1; i < img->altura-1; i++)
    {
        for (j = 1; j < img->largura-1; j++)
        {
            soma = img->dados[i][j+1]*20 + img->dados[i][j-1]*20 + img->dados[i+1][j]*20 + img->dados[i-1][j]*20 + img->dados[i][j]*30 + 
                   img->dados[i+1][j-1]*10 + img->dados[i+1][j+1]*10 + img->dados[i-1][j-1]*10 + img->dados[i-1][j+1]*10;

            media = soma/150;

            if(media < 120)
                img->dados[i][j] = 0;
            else
                img->dados[i][j] = 255;
            
        }
    }
}
