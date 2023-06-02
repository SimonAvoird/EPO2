#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int maze[13][13] = {                                                                    //defining the maze
    {-1, -1, -1, -1,-1, -1,-1, -1,-1, -1, -1, -1, -1},
    {-1, -1, -1, -1,-1, -1,-1, -1,-1, -1, -1, -1, -1},
    {-1, -1,  0,  0, 0,  0, 0,  0, 0,  0,  0, -1, -1},
    {-1, -1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1, -1},
    {-1, -1,  0,  0, 0,  0, 0,  0, 0,  0,  0, -1, -1},
    {-1, -1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1, -1},
    {-1, -1,  0,  0, 0,  0, 0,  0, 0,  0,  0, -1, -1},
    {-1, -1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1, -1},
    {-1, -1,  0,  0, 0,  0, 0,  0, 0,  0,  0, -1, -1},
    {-1, -1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1, -1},
    {-1, -1,  0,  0, 0,  0, 0,  0, 0,  0,  0, -1, -1},
    {-1, -1, -1, -1, 0, -1,-1, -1,-1, -1, -1, -1, -1},
    {-1, -1, -1, -1, 0, -1,-1, -1,-1, -1, -1, -1, -1}
};
int done[13][13] = {0};
struct Destination {
    int row, column, previous_row, previous_column;
};

char route_cross[25];
int route_row[25];
int route_column[25];

int print_maze(void)                                                                    //function to print the whole 2d-maze array
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


struct Destination route_finder(int i_current, int j_current)                              //this maps all the possible routes, by increasing every number next to the start, and then the next number
{
    int k = 0 , l = 0;
    struct Destination target;
    int found = 0, i = i_current, j = j_current, index = 2;
    maze[i][j] = index;
    done[i][j] = 1;
    
    while(found == 0)
    {
        for(int k = 0; k < 13; k++)
        {
            for(int l = 0; l< 13; l++)
            {
                if(maze[k][l] == index)
                {   
                    if(maze[k][l-1] == 0 && l > 0)
                    {
                        maze[k][l-1] = index + 1;
                        if (done[k][l-1] == 0)
                            {
                                target.row = k;
                                target.column = l-2 ;
                                found = 1;
                                target.previous_row = k;
                                target.previous_column = l;
                                break;
                            }
                    }
                    if(maze[k-1][l] == 0 && k > 0)
                    {   
                        maze[k-1][l] = index + 1;
                        if (done[k-1][l] == 0)
                            {
                                target.row = k-2;
                                target.column = l ;
                                found = 1;
                                target.previous_row = k;
                                target.previous_column = l;
                                break;
                            }
                    }
                    if(maze[k][l+1] == 0 && l < 12)
                    {
                        maze[k][l+1] = index + 1;
                        if (done[k][l+1] == 0 && l < 13)
                            {
                                target.row = k;
                                target.column = l+ + 2 ;
                                found = 1;
                                target.previous_row = k;
                                target.previous_column = l;
                                break;
                            }
                    }
                    if(maze[k+1][l] == 0 && k <12)
                    {
                        maze[k+1][l] = index + 1;
                        if (done[k+1][l] == 0)
                            {
                                target.row = k + 2;
                                target.column = l ;
                                found = 1;
                                target.previous_row = k;
                                target.previous_column = l;
                                break;
                            }
                    }
                   
                }
         
            }
        }
      index++;  
    }
    
    return target;
}

int route_define(int i_current, int j_current, int i_target, int j_target)                              //function that maps the crossroads in the chosen route into an array
{   
    int index = maze[i_current][j_current];
    int i = i_current;
    int j = j_current;
    int amount = 0;
    
    while(maze[i][j] != 0)                                                                    //go from the highest value at the beginning, down to the end where the index is low
    {

        if(maze[i+1][j] == index + 1)                                                   //find in which place is the lower value to go to next
        {   
            done[i+1][j] = 1;
            i = i + 2; 
            
        }
        else if(maze[i][j+1] == index + 1)
        {   
            done[i][j+1] = 1;
            j = j + 2; 
            
        }
        else if(maze[i-1][j] == index + 1)
        {   
            done[i-1][j] = 1;
            i = i - 2; 
            
        }
        else if(maze[i][j-1] == index + 1)
        {   
            done[i][j-1] = 1;
            j = j - 2; 
            
        }

        if (maze[i][j] == 0)
            {
            route_cross[amount] = 'c';                                                  //if a crossroad is passed it should be placed in the array
            route_row[amount] = (i - 2)/2;
            route_column[amount] = (j - 2)/2;
            amount++; 
            }
    //printf("%c", ' ');
    index++;                                                                        //lower index value, so it goes to the next maze cell
    }

    route_cross[amount] = 'e';  

    return 0;
    
}

int main ()
{
int mines = 0;
int found = 0;
int i = 12, j = 4, a;
int m =0, n = 0;

//print_maze();

while (mines < 13)
{  
    struct Destination target = route_finder(i, j);

    
    route_define(i, j, target.row, target.column);


    i = target.row;
    j = target.column;
    
    if (found == 1)
    {
        mines++;
        i = target.previous_row;
        j = target.previous_column;
        maze[target.row][target.column] = -1;
    }
    a = 0;
    while(route_cross[a] != 'e')                                                        
    {
        printf("%c%d%d ", route_cross[a], route_row[a], route_column[a]);
        a++;
    }

    //print_maze();

   for(m = 0; m < 13; m++)
   {
        for(n = 0; n < 13; n++)
            {
                if(maze[m][n] > 1)
                    {
                        maze[m][n] = 0;
                    }
            }
   }
   //printf("%c", ' ');
}


return 0;
}