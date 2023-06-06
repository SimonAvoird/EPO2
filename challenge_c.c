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
int save[13][13] = {0};
int mine[2];
struct Destination {
    int row, column, previous_row, previous_column;
};

char route_cross[25];
int route_row[25];
int route_column[25];

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


struct Destination route_finder(int i_current, int j_current)                             
{
    int k = 0 , l = 0;
    struct Destination target;
    int found = 0, i = i_current, j = j_current, index = 2;
    maze[i][j] = index;
    done[i][j] = 1;
    
    while(found == 0)
    {   
        if (found == 1)
        {
            break;
        }
        for(int k = 0; k < 13; k++)
        {
            if (found == 1)
            {
                break;
            }
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
                                maze[k][l-2] = index + 2;
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
                                maze[k-2][l] = index + 2;
                                break;
                            }
                    }
                    if(maze[k][l+1] == 0 && l < 12)
                    {
                        maze[k][l+1] = index + 1;
                        if (done[k][l+1] == 0 && l < 13)
                            {
                                target.row = k;
                                target.column = l + 2 ;
                                found = 1;
                                target.previous_row = k;
                                target.previous_column = l;
                                maze[k][l+2] = index + 2;
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
                                maze[k+2][l] = index + 2;
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

int route_define(int i_current, int j_current, int i_target, int j_target)                             
{   
    int index = maze[i_target][j_target];
    int i = i_target;
    int j = j_target;
    int amount = 0, condition =1;
    int z = 0;
    
    while(maze[i][j] != maze[i_current][j_current])                                                                  
    {

        if(maze[i][j-1] == index - 1)                                                   
        {   
            if (done[i][j-1] == 0 || condition == 0)
                {   
                    save[i][j-1] = 1;
                    j = j - 2; 
                    index = index - 2;
                    condition =  0;
                }
            
        }
        if(maze[i-1][j] == index - 1)
        {   
            if (done[i-1][j] == 0 || condition == 0)
                {   
                    save[i-1][j] = 1;
                    i = i - 2; 
                    index = index - 2;
                    condition =  0;
                }
          
        }
        if(maze[i][j+1] == index - 1)
        {   
            if (done[i][j+1] == 0 || condition == 0)
                {   
                    save[i][j+1] = 1;
                    j = j + 2; 
                    index = index - 2;
                    condition =  0;
                }
        }
        if(maze[i+1][j] == index - 1)
        {   
            if (done[i+1][j] == 0 || condition == 0)
                {   
                    save[i+1][j] = 1;
                    i = i + 2; 
                    index = index - 2;
                    condition =  0;
                }
        }

    }

    while(maze[i][j] != maze[i_target][j_target] || maze[i][j] != 0)       
    {   
        if(z == 0)
            {
                index = 2;
                z = 1;
            }

        if(maze[i][j-1] == index + 1 && save[i][j-1] == 1)                                                  
        {   
            done[i][j-1] = 1;
            j = j - 2; 
            if(maze[i][j-2] == maze[i_target][j_target])
               {
                mine[0] = i;
                mine[1] = j-1;
               }
        }
        else if(maze[i-1][j] == index + 1 && save[i-1][j] == 1)
        {   
            done[i-1][j] = 1;
            i = i - 2;
            if(maze[i-2][j] == maze[i_target][j_target])
               {
                mine[0] = i-1;
                mine[1] = j;
               }
        }
        else if(maze[i][j+1] == index + 1 && save[i][j+1] == 1)
        {   
            done[i][j+1] = 1;
            j = j + 2;
            if(maze[i][j+2] == maze[i_target][j_target])
               {
                mine[0] = i;
                mine[1] = j+1;
               }
        }
        else if(maze[i+1][j] == index + 1 && save[i+1][j] == 1)
        {   
            done[i+1][j] = 1;
            i = i + 2;
            if(maze[i+2][j] == maze[i_target][j_target])
               {
                mine[0] = i+1;
                mine[1] = j;
               }
        }

        route_cross[amount] = 'c';                                                
        route_row[amount] = (i - 2)/2;
        route_column[amount] = (j - 2)/2;
        amount++; 
        
        index = index + 2;    
        
        if (maze[i][j] == maze[i_target][j_target])
            {
                break;
            }
    }

    route_cross[amount] = 'e';  

    return 0;
    
}

int main ()
{
int mines = 0;
int found = 0;
int i = 12, j = 4, a;
int m = 0, n = 0, x = 0, y = 0;
int mine_x = 0, mine_y = 0;



while (mines < 13)
{  
    struct Destination target = route_finder(i, j);

    
    route_define(i, j, target.row, target.column);


    i = target.row;
    j = target.column;
    
    a = 0;
    while(route_cross[a] != 'e')                                                        
    {
        printf("%c%d%d ", route_cross[a], route_row[a], route_column[a]);
        a++;
    }
    
    mine_x = mine[1];
    mine_y = mine[2];

    //here the code should get the input from the robot if there is a mine or not

    if (found == 1)
    {
        mines++;
        i = target.previous_row;
        j = target.previous_column;
        maze[mine_x][mine_y] = -1;
        found = 0;
    }
    
    for(x = 0; x < 13; x++)
   {
        for(y = 0; y < 13; y++)
            {
                if(save[x][y] == 1)
                    {
                        save[x][y] = 0;
                    }
            }
   }

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
   
}


return 0;
}