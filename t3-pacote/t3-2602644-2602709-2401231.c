#include "trabalho3.h"
#include "imagem.h"
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846
#define CINZA 120

// ============================================================================== //
// Tipo Caixa

typedef struct
{
    int esquerda;
    int direita;
    int baixo;
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

    for (i = 2; i < img->altura - 3; i++)
    {
        for (j = 2; j < img->largura - 3; j++)
        {
            soma = (int) (img->dados[i-2][j-2]*0.003 + img->dados[i-2][j-1]*0.013 + img->dados[i-2][j]*0.022 + img->dados[i-2][j+1]*0.013 + img->dados[i-2][j+2]*0.003 +
                          img->dados[i-1][j-2]*0.013 + img->dados[i-1][j-1]*0.060 + img->dados[i-1][j]*0.098 + img->dados[i-1][j+1]*0.060 + img->dados[i-1][j+2]*0.013 +  
                          img->dados[i][j-2]*0.022 + img->dados[i][j-1]*0.098 + img->dados[i][j]*0.162 + img->dados[i][j+1]*0.098 + img->dados[i][j+2]*0.022 +
                          img->dados[i+1][j-2]*0.013 + img->dados[i+1][j-1]*0.060 + img->dados[i+1][j]*0.098 + img->dados[i+1][j+1]*0.060 + img->dados[i+1][j+2]*0.013 + 
                          img->dados[i+2][j-2]*0.003 + img->dados[i+2][j-1]*0.013 + img->dados[i+2][j]*0.022 + img->dados[i+2][j+1]*0.013 + img->dados[i+2][j+2]*0.003);            

            if(soma < CINZA)
                copia[i][j] = 0;
            else
                copia[i][j] = 255;
        }
    }
    
    for (i = 0; i < img->altura; i++)
    {
        for (j = 0; j < img->largura; j++)
        {
            if (i<3 || j<3 || i >= img->altura-3 || j >= img->largura-3) // tirando o ruído das bordas
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
    int i, j, rotulo, achou;
    Caixa esquerda_caixa, direita_caixa;

    diminuiRuido(img);

    matriz_rotulada = rotulaMatriz(img);

    // ============================================================================== //
    // Cercando bola da esquerda

    achou = 0;
    for (j = 0; j < img->largura && !achou; j++) // percorrer a matriz procurando objeto (coluna por coluna)
    {
        for (i = 0; i < img->altura && !achou; i++)
        {
            if (matriz_rotulada[i][j]) // guardar o rotulo do objeto encontrado ACHOU O PONTO MAIS À ESQUERDA
            {
                rotulo = matriz_rotulada[i][j];
                esquerda_caixa.esquerda = j; // cercar pela esquerda
                achou = 1;
            }
        }
    }

    achou = 0;
    for (j = img->largura - 1; j >= 0 && !achou; j--) // percorrer a matriz procurando objeto (coluna por coluna)
    {
        for (i = 0; i < img->altura && !achou; i++)
        {
            if (matriz_rotulada[i][j] == rotulo) // guardar o rotulo do objeto encontrado ACHOU O PONTO MAIS À DIREITA 
            {
                esquerda_caixa.direita = j; // cercar pela direita
                achou = 1;
            }
        }
    }

    achou = 0;
    for (i = 0; i < img->altura && !achou; i++) // percorrer a matriz (linha por linha)
    {
        for (j = esquerda_caixa.esquerda; j < esquerda_caixa.direita && !achou; j++)
        {
            if (matriz_rotulada[i][j] == rotulo) // procurando objeto com rótulo armazenado ACHOU O TOPO
            {
                esquerda_caixa.topo = i;// cercar por cima
                achou = 1;
            }
        }
    }

    achou = 0;
    for (i = img->altura - 1; i > esquerda_caixa.topo && !achou; i--) // percorrer a matriz (linha por linha)
    {
        for (j = esquerda_caixa.esquerda; j < esquerda_caixa.direita && !achou; j++)
        {
            if (matriz_rotulada[i][j] == rotulo) // procurando objeto com rótulo armazenado ACHOU O baixo
            {
                esquerda_caixa.baixo = i;// cercar por baixo
                achou = 1;
            }
        }
    }

    // ============================================================================== //
    // Cercando bola da direita

    achou = 0;
    for (j = img->largura - 1; j > 0 && !achou; j--) // percorrer a matriz (coluna por coluna) É A BOLA DA DIREITA
    {
        for (i = 0; i < img->altura; i++)
        {
            if (matriz_rotulada[i][j]) // procurando objeto com rótulo diferente do armazenado ACHOU O EXTREMO
            {                                                                                                                 // ESQUERDO DA BOLA
                direita_caixa.direita = j; // cercar pela esquerda
                rotulo = matriz_rotulada[i][j];
                achou = 1;
            }
        }
    }

    achou = 0;
    for (j = 0; j < img->largura && !achou; j++) // percorrer a matriz (coluna por coluna) É A BOLA DA DIREITA
    {
        for (i = 0; i < img->altura; i++)
        {
            if (matriz_rotulada[i][j] == rotulo) // procurando objeto com rótulo diferente do armazenado ACHOU O EXTREMO
            {                                                                                                                 // ESQUERDO DA BOLA
                direita_caixa.esquerda = j; // cercar pela esquerda
                achou = 1;
            }
        }
    }

    achou = 0;
    for (i = 0; i < img->altura; i++) // percorrer a matriz (linha por linha)
    {
        for (j = direita_caixa.esquerda - 1; j < direita_caixa.direita + 1; j++)
        {
            if (matriz_rotulada[i][j] == rotulo) // procurando objeto com rótulo diferente do armazenado ACHOU O TOPO
            {
                direita_caixa.topo = i; // cercar por cima
                achou = 1;
            }
        }
    }

    achou = 0;
    for (i = img->altura - 1; i > 0; i--) // percorrer a matriz (linha por linha)
    {
        for (j = direita_caixa.esquerda - 1; j < direita_caixa.direita + 1; j++)
        {
            if (matriz_rotulada[i][j] == rotulo) // procurando objeto com rótulo diferente do armazenado ACHOU O TOPO
            {
                direita_caixa.baixo = i; // cercar por cima
                achou = 1;
            }
        }
    }

    // ============================================================================== //
    // Setando coordenadas dos centros

    l->x = (esquerda_caixa.direita + esquerda_caixa.esquerda) / 2;
    l->y = (esquerda_caixa.baixo + esquerda_caixa.topo) / 2;

    r->x = (direita_caixa.direita + direita_caixa.esquerda) / 2;
    r->y = (direita_caixa.baixo + direita_caixa.topo) / 2;

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
    {
        if (nums[i])
        {
            menor = nums[i];
            i = 4;
        }
    }
    
    for (i = 0; i < 4; i++)
    {
        if (nums[i] && nums[i] <= menor)
        {
            menor = nums[i];
            i = 4;
        }
    }

    return menor;
}

double calculaAngulo(Coordenada* l, Coordenada* r)
{
    float co, ca;
    float tan;
    
        if (l->y < r->y) // Bola da esquerda acima
        {
            if (l->x == r->x)
                return PI/2;
            
            ca = r->x - l->x;
            co = r->y - l->y;
            
            return atan(co/ca);
        }
            
        else if (l->y > r->y) // Bola da direita acima
        {
            if (l->x == r->x)
                return -PI/2;
            
            ca = r->x - l->x;
            co = l->y - r->y;
            
            return -atan(co/ca);
        }
    return 0;
}

// ============================================================================== //