#include<stdio.h>
#include<stdlib.h>

int maze[13][13] = {
    {-1,  2, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1},
    {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1},
    {-1, -1,  0,  0, 0,  0, 0,  0, 0,  0,  0, -1, -1},
    {-1, -1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1, -1},
    { 0,  0,  0,  0, 0,  0, 0,  0, 0,  0,  0,  0,  0},
    {-1, -1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1, -1},
    { 0,  0,  0,  0, 0,  0, 0,  0, 0,  0,  0,  0,  0},
    {-1, -1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1, -1},
    { 0,  0,  0,  0, 0,  0, 0,  0, 0,  0,  0,  0,  0},
    {-1, -1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1, -1},
    {-1, -1,  0,  0, 0,  0, 0,  0, 0,  0,  0, -1, -1},
    {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1},
    {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1}
};
long int cross_in[81];
char cross_dir[40];

int field_define(int stop)
{
    int i,j,l;
    char k;
    for(l = 1; l < (cross_in[0]*3+1); l = l + 2)
    {
        i = cross_in[l];
        j = cross_in[l+1];
        k = cross_dir[(l-1)/2];
        i = 2 * i + 2;
        j = 2 * i + 2;
        switch(k)
        {
            case 's': 
                i++;
                break;
            case 'e': 
                j++;
                break;
        }
        maze[i][j] = -1;
    }
    return 0;
}

int main()
{
    int k = 0;
    for(int i = 0; i < 13; i++) {
        for(int j = 0; j < 13; j++) {
            if(maze[i][j] < 0)
            {
                printf(" %d", maze[i][j]);
            }
            else
            {
                printf("  %d", maze[i][j]);
            }
        }
        printf("\n");
    }
    scanf("amount: %d", &cross_in[0]);
    for(k = 1; k <= (cross_in[0]*3+1); k = k + 2)
    {
        scanf("i: %d", &cross_in[k]);
        scanf("j: %d", &cross_in[k+1]);
        scanf("direction: %c", &cross_dir[(k-1)/2]);        
    }
    field_define(k);
    for(int i = 0; i < 13; i++) {
        for(int j = 0; j < 13; j++) {
            if(maze[i][j] < 0)
            {
                printf(" %d", maze[i][j]);
            }
            else
            {
                printf("  %d", maze[i][j]);
            }
        }
        printf("\n");
    }
    return 0;
}