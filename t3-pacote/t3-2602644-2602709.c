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

    for (i = 0; i < img->altura; i++)
    {
        soma = 0;
        media = 0;
        for (j = 0; j < img->largura; j++)
        {
            soma = img->dados[i][j+1] + img->dados[i][j-1] + img->dados[i+1][j] + img->dados[i-1][j] + img->dados[i][j];
            media = soma/5;
            if(media < 120)
            {
                img->dados[i][j] = 255;
            }
            else
            {
                img->dados[i][j] = 0;
            }
        }
    }
    salvaImagem1C(img, "sem_ruido.bmp");
}
