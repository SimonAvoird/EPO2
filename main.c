#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Station {
    int row,column;
};
struct Cross {
    char cross;
    int row;
    int column;
};
struct Cross route[25];
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

int route_finder(struct Station begin, struct Station end)
{
    int l,i,j, index = 0;
    i = end.row;
    j = end.column;
    maze[i][j] = 1;
    while(maze[begin.row][begin.column] == 0)
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
    return 0;
}

struct Station stations(int station) // this converts the station number from the input to an array of coordinates in the matrix
{
    struct Station coords;
    switch(station)
    {
        case(1): case(2): case(3):
            coords.row = 12;
            coords.column = station * 2 + 2;
            break;
        case(4): case(5): case(6):
            coords.row = 12 - ((station - 3) * 2 + 2);
            coords.column = 12;
            break;
        case(7): case(8): case(9):
            coords.row = 0;
            coords.column = 12 - ((station - 6) * 2 + 2);
            break;
        case(10): case(11): case(12):
            coords.row = (station - 9) * 2 + 2;
            coords.column = 0;
            break;
    }
    return coords;
}  
int route_define(struct Station begin, struct Station end)
{
    int index = maze[end.row][end.column];
    int i = end.row;
    int j = end.column;
    int amount = (index - 3) / 2;
    route[amount].cross = 'e';
    amount--;
    while(index > 0)
    {
        if(i%2 == 0 && j&2 == 0 && i != 0 && i != 12 && j != 0 && j != 12)
        {
            route[amount].cross = 'c';
            route[amount].row = (i - 2)/2;
            route[amount].column = (j-2)/2;
            amount--; 
        }
        if(maze[i+1][j] == index - 1)
        {   
            i++; 
        }
        else if(maze[i][j+1] == index - 1)
        {   
            j++; 
        }
        else if(maze[i-1][j] == index - 1)
        {   
            i--; 
        }
        else if(maze[i][j-1] == index - 1)
        {   
            j--; 
        }
        index--;
    }
    return 0;
}
int print_maze(void)
{
    for(int i = 0; i < 13; i++) {
        for(int j = 0; j < 13; j++) {
            if(maze[i][j] < 0 || maze[i][j] > 9)
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
}

int main(void)
{
    int k = 0, i = 0,stop, begin_station, end_station;
    print_maze();
    scanf("%d", &cross_in[0]);
    stop = cross_in[0]*2 + 1;
    for(k = 1; k < stop; k = k + 2)
    {
        scanf("%d %d %c", &cross_in[k], &cross_in[k+1], &cross_dir[(k-1)/2]);       
    }
    scanf("%d %d", &begin_station, &end_station);
    field_define(stop);
    struct Station begin = stations(begin_station);
    struct Station end = stations(end_station);
    route_finder(begin, end);
    print_maze();
    route_define(begin, end);
    while(route[i].cross != 'e')
    {
        printf("%c %d %d", route[i].cross, route[i].row, route[i].column);
        i++;
    }
}