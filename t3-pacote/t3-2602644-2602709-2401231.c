#include "trabalho3.h"
#include "imagem.h"
#include <stdlib.h>
#include <stdio.h>

#define CINZA 120
#define ALT_MAX 1080
#define LARG_MAX 1920
#define BOLA_ESQUERDA 1
#define BOLA_DIREITA 2

typedef struct
{
    int left;
    int right;
    int top;
    int bottom;
} Cerca;


double detectaSensorBar(Imagem1C* img, Coordenada* l, Coordenada* r);
void diminuiRuido(Imagem1C* img);
void achaCentro(Imagem1C* img, Coordenada* l, Coordenada* r);
void rotulaMatriz(Imagem1C* img);
//int menorRotulo(int a, int b, int c, int d);

double detectaSensorBar(Imagem1C* img, Coordenada* l, Coordenada* r)
{
    int i, j;

    diminuiRuido(img); 
    // achaCentro(img, l, r);
    return 0;
}

void diminuiRuido(Imagem1C* img)
{
    int i, j, soma, media;
    unsigned char copia[ALT_MAX][LARG_MAX];

    for (i = 1; i < img->altura - 1; i++)
    {
        for (j = 1; j < img->largura - 1; j++)
        {
            soma = img->dados[i][j+1]*20 + img->dados[i][j-1]*20 + img->dados[i+1][j]*20 + img->dados[i-1][j]*20 + img->dados[i][j]*30 + 
                   img->dados[i+1][j-1]*10 + img->dados[i+1][j+1]*10 + img->dados[i-1][j-1]*10 + img->dados[i-1][j+1]*10;

            media = soma/150;

            if(media < CINZA)
                copia[i][j] = 0;
            else
                copia[i][j] = 255;
        }
    }

    for (i = 0; i < img->altura; i++)
    {
        for (j = 0; j < img->largura; j++)
        {
            if (i<2 || j<2 || i >= img->altura-2 || j >= img->largura-2)
                img->dados[i][j] = 0;
            else
                img->dados[i][j] = copia[i][j];
        }
    }
}

void achaCentro(Imagem1C* img, Coordenada* l, Coordenada* r)
{
    int soma_x[LARG_MAX] = {0}, soma_y[ALT_MAX] = {0};
    int i, j;

    for (i = 0; i < img->altura; i++)
    {
        for (j = 0; j < img->largura; j++)
        {
            soma_y[i] += img->dados[i][j];
        }
    }
    for (j = 0; j < img->largura; j++)
    {
        for (i = 0; i < img->altura; i++)
        {
            soma_x[j] += img->dados[i][j];
        }
    }
    i = 0;
}


void rotulaMatriz(Imagem1C* img)
{
    int i, j, rotulo;
    int** matriz_rotulada;

    matriz_rotulada = (int**) malloc(sizeof(int*) * img->altura);
    for (i = 0; i < img->altura; i++)
        matriz_rotulada[i] = (int) malloc(sizeof(int) * img->largura);

    for (i = 0; i < img->altura; i++)
    {
        for (j = 0; j < img->largura; j++)
        {
            matriz_rotulada[i][j] = 0;
        }
    }

    rotulo = 1;
    for (i = 0; i < img->altura; i++)
    {
        for (j = 0; j < img->largura; j++)
        {
            if (img->dados[i][j]) // se tiver um pixel branco
            {
                if (matriz_rotulada[i-1][j] || matriz_rotulada[i-1][j-1] || matriz_rotulada[i][j-1] || matriz_rotulada[i+1][j-1])
                    matriz_rotulada[i][j] = menorRotulo(matriz_rotulada[i-1][j], matriz_rotulada[i-1][j-1], matriz_rotulada[i][j-1], matriz_rotulada[i+1][j-1]);
                else
                    matriz_rotulada[i][j] = rotulo++;
            }
        }
    }

    for (i = img->altura - 1; i >= 0; i--)
    {
        for (j = img->largura - 1; j >= 0; j--)
        {
            if (matriz_rotulada[i][j]) // quando achar um pixel rotulado
            {
                if (1) 
                    matriz_rotulada[i][j] = 1;
                else
                    matriz_rotulada[i][j] = rotulo++;
            }
        }
    }
}
