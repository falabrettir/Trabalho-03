#include "trabalho3.h"
#include "imagem.h"
#include <math.h>
#include <stdlib.h>

#define MIN2(a, b) (a < b ? a:b)
#define MIN3(a, b, c) (MIN2(MIN2(a, b), c))
#define MIN4(a, b, c, d) (MIN2(MIN3(a, b, c), d))

double calculaAngulo(Coordenada l, Coordenada r);
Coordenada* achaCentros(Imagem1C img);
double detectaSensorBar(Imagem1C* img, Coordenada* l, Coordenada* r);
void diminuiRuido(Imagem1C* img);
int** rotulaMatriz(Imagem1C* img);


double detectaSensorBar(Imagem1C* img, Coordenada* l, Coordenada* r)
{
    diminuiRuido(img);
    rotulaMatriz(img);
    return 0.0;
}

double calculaAngulo(Coordenada l, Coordenada r)
{
    return 0.0;
}

Coordenada* achaCentro(Imagem1C img) // Função bolacha do mar
{
    
}

void diminuiRuido(Imagem1C* img)
{
    int i, j, soma, media;
    Imagem1C copia = *img;

    for (i = 1; i < img->altura-1; i++)
    {
        for (j = 1; j < img->largura-1; j++)
        {
            soma = img->dados[i][j+1]*20 + img->dados[i][j-1]*20 + img->dados[i+1][j]*20 + img->dados[i-1][j]*20 + img->dados[i][j]*30 + 
                   img->dados[i+1][j-1]*10 + img->dados[i+1][j+1]*10 + img->dados[i-1][j-1]*10 + img->dados[i-1][j+1]*10;

            media = soma/150;

            if(media < 100)
                copia.dados[i][j] = 0;
            else
                copia.dados[i][j] = 255;
        }
        img->dados = copia.dados;
    }
}

int** rotulaMatriz(Imagem1C* img)
{
    int i, j;
    int rotulo = 1;
    int **matriz_rotulada;

    matriz_rotulada = (int**) malloc(sizeof(int*) * img->altura);
    for (i = 0; i < img->largura; i++)
        matriz_rotulada[i] = (int*) malloc(sizeof(int) * img->largura);

    for (i = 0; i < img->altura; i++)
    {
        for (j = 0; j < img->largura; j++)
        {
            matriz_rotulada[i][j] = 0;
        }
    

    for (i = 1; i < img->altura - 1; i++)
    {
        for (j = 1; j < img->largura - 1; j++)
        {
            if (img->dados[i][j])
            {
                if (!(img->dados[i][j-1] + img->dados[i-1][j-1] + img->dados[i-1][j] + img->dados[i-1][j+1]))
                {
                    matriz_rotulada = rotulo++;
                }
                else
                {

                }
            }
        }
    }

    for (i = img->altura - 2; i > 0; i--)
    {
        for (j = img->largura - 2; j > 0 ; j--)
        {

        }
    }
}
