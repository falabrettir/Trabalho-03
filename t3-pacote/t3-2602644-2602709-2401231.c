#include "trabalho3.h"
#include "imagem.h"
#include <stdlib.h>
#include<math.h>

#define CINZA 120

// ============================================================================== //
// Tipo Caixa

typedef struct
{
    int esquerda;
    int direita;
    int fundo;
    int topo;
} Caixa;

// ============================================================================== //
// Cabeçalhos de funções

double detectaSensorBar(Imagem1C* img, Coordenada* l, Coordenada* r);
void diminuiRuido(Imagem1C* img);
void achaCentros(Imagem1C* img, Coordenada* l, Coordenada* r);
int** rotulaMatriz(Imagem1C* img);
int menorRotulo(int a, int b, int c, int d);
double calculaAngulo(Coordenada* coord1, Coordenada* coord2);

// ============================================================================== //
// Implementação das funções

double detectaSensorBar(Imagem1C* img, Coordenada* l, Coordenada* r)
{
    achaCentros(img, l, r);

    return calculaAngulo(l, r);
}

void diminuiRuido(Imagem1C* img)
{
    int i, j, soma, media;
    unsigned char** copia;

    copia = (unsigned char**) malloc(sizeof(unsigned char*) * img->altura);
    for (i = 0; i < img->altura; i++)
        copia[i] = (unsigned char*) malloc(sizeof(unsigned char) * img->largura);

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
            if (i<2 || j<2 || i >= img->altura-2 || j >= img->largura-2) // tirando o ruído das bordas
                img->dados[i][j] = 0;
            else
                img->dados[i][j] = copia[i][j];
        }
    }

    for (i = 0; i < img->altura; i++)
        free(copia[i]);
    free(copia);
}

void achaCentros(Imagem1C* img, Coordenada* l, Coordenada* r)
{
    int **matriz_rotulada;
    int i, j, rotulo;
    Caixa esquerda_caixa, direita_caixa;

    diminuiRuido(img);

    matriz_rotulada = rotulaMatriz(img);

    // ============================================================================== //
    // Cercando bola da esquerda

    for (j = 0; j < img->largura; j++) // percorrer a matriz procurando objeto (coluna por coluna)
    {
        for (i = 0; i < img->altura; i++)
        {
            if (matriz_rotulada[i][j]) // guardar o rotulo do objeto encontrado ACHOU O PONTO MAIS À ESQUERDA
            {
                rotulo = matriz_rotulada[i][j];
                esquerda_caixa.esquerda = j; // cercar pela esquerda
                // cercar pela direita
            }
        }
    }
    
    for (i = 0; i < img->altura; i++) // percorrer a matriz (linha por linha)
    {
        for (j = 0; j < img->largura; j++)
        {
            if (matriz_rotulada[i][j] == rotulo) // procurando objeto com rótulo armazenado ACHOU O TOPO
            {
                esquerda_caixa.topo = i;// cercar por cima
                // cercar por baixo
            }
        }
    }

    // ============================================================================== //
    // Cercando bola da direita

    for (j = 0; j < img->largura; j++) // percorrer a matriz (coluna por coluna) É A BOLA DA DIREITA
    {
        for (i = 0; i < img->altura; i++)
        {
            if (matriz_rotulada[i][j] && matriz_rotulada[i][j] != rotulo) // procurando objeto com rótulo diferente do armazenado ACHOU O EXTREMO
            {                                                                                                                 // ESQUERDO DA BOLA
                direita_caixa.esquerda = j; // cercar pela esquerda
                // cercar pela direita
            }
        }
    }
    
    for (i = 0; i < img->altura; i++) // percorrer a matriz (linha por linha)
    {
        for (j = 0; j < img->largura; j++)
        {
            if (matriz_rotulada[i][j] && matriz_rotulada[i][j] != rotulo) // procurando objeto com rótulo diferente do armazenado ACHOU O TOPO
            {
                direita_caixa.topo = i; // cercar por cima
                // cercar por baixo
            }
        }
    }

    // ============================================================================== //
    // Setando coordenadas dos centros

    l->x = (esquerda_caixa.direita + esquerda_caixa.esquerda) / 2;
    l->y = (esquerda_caixa.fundo + esquerda_caixa.topo) / 2;

    r->x = (direita_caixa.direita + direita_caixa.esquerda) / 2;
    r->y = (direita_caixa.fundo + direita_caixa.topo) / 2;

    // ============================================================================== //

    for (i = 0; i < img->altura; i++)
        free(matriz_rotulada[i]);
    free(matriz_rotulada);
}

int** rotulaMatriz(Imagem1C* img)
{
    int i, j, rotulo;
    int** matriz_rotulada;

    matriz_rotulada = (int**) malloc(sizeof(int*) * img->altura);
    for (i = 0; i < img->altura; i++)
        matriz_rotulada[i] = (int*) malloc(sizeof(int) * img->largura);

    for (i = 0; i < img->altura; i++)
    {
        for (j = 0; j < img->largura; j++)
        {
            matriz_rotulada[i][j] = 0;
        }
    }

    rotulo = 1;
    for (i = 1; i < img->altura; i++)
    {
        for (j = 1; j < img->largura; j++)
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

    for (i = img->altura - 2; i >= 0; i--)
    {
        for (j = img->largura - 2; j >= 0; j--)
        {
            if (matriz_rotulada[i][j]) // quando achar um pixel rotulado
            {
                if (matriz_rotulada[i][j+1] || matriz_rotulada[i+1][j+1] || matriz_rotulada[i+1][j] || matriz_rotulada[i+1][j-1]) 
                    matriz_rotulada[i][j] = menorRotulo(matriz_rotulada[i][j+1], matriz_rotulada[i+1][j+1], matriz_rotulada[i+1][j], matriz_rotulada[i+1][j-1]);
            }
        }
    }

    return matriz_rotulada;
}

int menorRotulo(int a, int b, int c, int d)
{
    int menor, i;
    int nums[] = {a, b, c, d};

    for (i = 0; i < 4; i++)
        if (nums[i])
            menor = nums[i];
    
    for (i = 0; i < 4; i++)
        if (nums[i] && nums[i] <= menor)
            menor = nums[i];

    return menor;
}

double calculaAngulo(Coordenada* coord1, Coordenada* coord2)
{
    if(coord1->x == coord2->x)
        return 0.0;
    else
        return atan((coord1->y - coord2->y)/(coord1->x - coord2->x));
}

// ============================================================================== //