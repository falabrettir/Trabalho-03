#include "trabalho3.h"
#include "imagem.h"
#include <math.h>
#include <stdlib.h>

#define MAX2(a, b) (a > b ? a:b)
#define MAX3(a, b, c) (MAX2(MAX2(a, b), c))
#define MAX4(a, b, c, d) (MAX2(MAX3(a, b, c), d))

double calculaAngulo(Coordenada l, Coordenada r);
Coordenada* achaCentros(Imagem1C img);
double detectaSensorBar(Imagem1C* img, Coordenada* l, Coordenada* r);
void diminuiRuido(Imagem1C* img);
int** rotulaMatriz(Imagem1C* img);
int menorDiferenteDeZero(int a, int b, int c, int d);


double detectaSensorBar(Imagem1C* img, Coordenada* l, Coordenada* r)
{
    int** matriz_rotulada;
    int i;

    diminuiRuido(img);
    
    matriz_rotulada = rotulaMatriz(img);

    for (i = 0; i < img->altura; i++)
        free(matriz_rotulada[i]);
    free(matriz_rotulada);
    
    return 0.0;
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

            if(media < 120)
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
    int rotulo, soma_vizinhanca;
    int **matriz_rotulada;

    matriz_rotulada = (int**) malloc(sizeof(int*) * img->altura);
    for (i = 0; i < img->largura; i++)
        matriz_rotulada[i] = (int*) malloc(sizeof(int) * img->largura);

    for (i = 0; i < img->altura; i++)
    {
        for (j = 0; j < img->largura; j++)
        {
            if (img->dados[i][j])
                matriz_rotulada[i][j] = -1;
        }
    }

    rotulo = 1;
    for (i = 1; i < img->altura - 1; i++)
    {
        for (j = 1; j < img->largura - 1; j++)
        {
            if (img->dados[i][j])
            {
                soma_vizinhanca = img->dados[i][j-1] + img->dados[i-1][j-1] + img->dados[i-1][j] + img->dados[i-1][j+1];
                if (!soma_vizinhanca)
                {
                    matriz_rotulada[i][j] = rotulo++;
                }
                else
                {
                    matriz_rotulada[i][j] = menorDiferenteDeZero(matriz_rotulada[i][j-1], matriz_rotulada[i-1][j-1], matriz_rotulada[i-1][j], matriz_rotulada[i-1][j+1]);
                }
            }
        }
    }

    for (i = img->altura - 2; i > 0; i--)
    {
        for (j = img->largura - 2; j > 0 ; j--)
        {
            if (matriz_rotulada[i][j])
            {
                soma_vizinhanca = matriz_rotulada[i][j+1] + matriz_rotulada[i+1][j+1] + matriz_rotulada[i+1][j] + matriz_rotulada[i+1][j-1];
                if (soma_vizinhanca)
                    {
                        matriz_rotulada[i][j] = menorDiferenteDeZero(matriz_rotulada[i][j+1], matriz_rotulada[i+1][j+1], matriz_rotulada[i+1][j], matriz_rotulada[i+1][j-1]);
                    }
            }
        }
    }
    return matriz_rotulada;
}

int menorDiferenteDeZero(int a, int b, int c, int d)
{
    int i;
    int vals[] = {a, b, c, d};
    int menor;
    menor = MAX4(a, b, c, d);
    for (i = 0; i < 4; i++)
    {
        if (vals[i] && (vals[i] < menor))
            menor = vals[i];
    }
    return menor;
}