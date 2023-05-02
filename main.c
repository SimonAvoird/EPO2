#include<stdio.h>
#include<stdlib.h>
#define s1 = [12][4];
int maze[13][13] = {
    {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1},
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
int cross_in[81];
char cross_dir[40];
int field_define(int stop)
{
    int i,j,l;
    char k;
    for(l = 1; l < stop; l = l + 2)
    {
        i = cross_in[l];
        j = cross_in[l+1];
        k = cross_dir[(l-1)/2];
        i = 2 * i + 2;
        j = 2 * j + 2;
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

int route_finder(int begin[], int end[])
{
    int l,i,j, index = 0;
    i = end[0];
    j = end[1];
    maze[i][j] = 1;
    while(maze[begin[0]][begin[1]] == 0)
    {
        index++;
        for(int k = 0; k < 13; k++)
        {
            for(int l = 0; l< 13; l++)
            {
                if(maze[k][l] == index)
                {
                    if(maze[k+1][l] == 0)
                    {
                        maze[k+1][l] = index + 1;
                    }
                    if(maze[k-1][l] == 0)
                    {
                        maze[k-1][l] = index + 1;
                    }
                    if(maze[k][l+1] == 0)
                    {
                        maze[k][l+1] = index + 1;
                    }
                    if(maze[k][l-1] == 0)
                    {
                        maze[k][l-1] = index + 1;
                    }
                }
            }
        }
        
    }
}

int stations(int station) // this converts the station number from the input to an array of coordinates in the matrix
{
    int coords[2];
    switch(station)
    {
        case(1): case(2): case(3):
            coords[0] = 12;
            coords[1] = station * 2 + 2;
            break;
        case(4): case(5): case(6):
            coords[0] = 12 - (station * 2 + 2);
            coords[1] = 12;
            break;
        case(7): case(8): case(9):
            coords[0] = 0;
            coords[1] = 12 - (station * 2 + 2);
            break;
        case(10): case(11): case(12):
            coords[0] = station * 2 + 2;
            coords[1] = 0;
            break;
    }
    return coords;
}   
int print_maze()
{
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

int main()
{
    int k = 0,stop, begin_station, end_station, begin[2], end[2];
    print_maze();
    scanf("%d", &cross_in[0]);
    stop = cross_in[0]*2 + 1;
    for(k = 1; k < stop; k = k + 2)
    {
        scanf("%d %d %c", &cross_in[k], &cross_in[k+1], &cross_dir[(k-1)/2]);       
    }
    scanf("%d %d", &begin_station, &end_station);
    field_define(stop);
    
    route_finder(begin_station, end_station);
    print_maze();
    return 0;
}