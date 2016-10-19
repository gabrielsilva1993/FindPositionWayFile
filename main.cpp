#include <iostream>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

int ***data;

bool readFile(const char* filename, unsigned int &W, unsigned int &H, unsigned int &Z);
bool allocMemory(unsigned int W, unsigned int H, unsigned int Z);
bool releaseMemory(unsigned int W, unsigned int H);
unsigned int findLastPosition(unsigned int &x, unsigned int &y, unsigned int &z, unsigned int W, unsigned int H, unsigned int Z);
unsigned int validateNextposition(int x,  int y,  int z, unsigned int W, unsigned int H, unsigned int Z);

using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 2) {
        printf("Incorrect usage: binary inputFile\n");
        return -1;
    }

    //read data
    unsigned int W, H, Z; W=H=Z=0;
    bool status = readFile(argv[1], W, H, Z);
    if(!status)
    {
        printf("\nErro, nao foi possivel abrir o arquivo...!\n");
        return -1;
    }
    printf("Arquivo %s lido com as dimensioes %dx%dx%d\n", argv[1], W, H, Z);

    //find last position
    unsigned int x; unsigned int y; unsigned int z; x=y=z=0;
    unsigned int length=0;

    length = findLastPosition(x, y, z, W, H, Z);
    printf("Comprimento %d em x:%d y:%d z:%d\n", length, x,y,z);


    //release data memory
    releaseMemory(W,H);
    return 0;
}

bool allocMemory(unsigned int W, unsigned int H, unsigned int Z)
{
    data = (int ***) calloc(H, sizeof(int **));
    for(unsigned int i=0; i<H; i++)
    {
        data[i] = (int **) calloc(W, sizeof(int *));
        for(unsigned int j=0; j<W; j++)
            data[i][j] = (int *) calloc(Z, sizeof(int));
    }
    return true;
}

bool releaseMemory(unsigned int W, unsigned int H)
{
    for(unsigned int i=0; i<H; i++)
    {
        for(unsigned int j=0; j<W; j++)
            free(data[i][j]);
        free(data[i]);
    }
    free(data);
}

bool readFile(const char* filename, unsigned int &W, unsigned int &H, unsigned int &Z)
{
    FILE *fp = fopen(filename, "r");
    if(!fp) return false;

    //memory alloc
    fscanf(fp, "%d %d %d", &W, &H, &Z);
    allocMemory(W,H,Z);

    //read file and store into data memory
    printf("\n...Lendo dados....\n");
    for(int z=0; z<Z; z++)
    {
        for(int i=0; i<H; i++)
        {
            for(int j=0; j<W; j++)
            {
                int v=0;
                fscanf(fp, "%d ", &v);
                data[i][j][z] = v;
            }
        }
    }

    fclose(fp);
    return true;
}

unsigned int findLastPosition(unsigned int &x, unsigned int &y, unsigned int &z, unsigned int W, unsigned int H, unsigned int Z)
{
        unsigned int lenght = 0;

        while(true)
        {
            int position = validateNextposition(x,y,z, W, H, Z);
            lenght++;

            if(position == 0) break;

            //Reposionando
            data[y][x][z] = 0;

            if(position == 1) y=y-1;
            if(position == 2) x=x+1;
            if(position == 3) y=y+1;
            if(position == 4) x=x-1;
            if(position == 5) z=z-1;
            if(position == 6) z=z+1;
        }
        return lenght;
}


unsigned int validateNextposition(int x,  int y,  int z, unsigned int W, unsigned int H, unsigned int Z)
{
    //UP
    if(y-1 >= 0 && data[y-1][x][z] == 1)  return 1;

    //R
    if(x+1 < W &&data[y][x+1][z] == 1)    return 2;

    //D
    if(y+1 < H && data[y+1][x][z] == 1)   return 3;

    //L
    if(x-1 >= 0 && data[y][x-1][z] == 1)  return 4;

    //FRONT
    if(z-1 >= 0 && data[y][x][z-1] == 1)  return 5;

    //Bening
    if(z+1 < Z && data[y][x][z+1] == 1)   return 6;

    return 0;
}
