#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Station {                                                                        //the structure for the station coordinates
    int row,column;
};
struct Station destination;
struct Station current_coord;
struct Cross {                                                                          //the structure for the final route
    char cross;
    int row;
    int column;
};
struct Cross find_current[2];
struct Cross current;
struct Cross route[25];
int maze[13][13] = {                                                                    //defining the maze
    {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1},
    {-1, -1, -1, -1, -2, -1, -2, -1, -2, -1, -1, -1, -1},
    {-1, -1,  0,  0, 0,  0, 0,  0, 0,  0,  0, -1, -1},
    {-1, -1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1, -1},
    { 0,  -2,  0,  0, 0,  0, 0,  0, 0,  0,  0,  -2,  0},
    {-1, -1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1, -1},
    { 0,  -2,  0,  0, 0,  0, 0,  0, 0,  0,  0,  -2,  0},
    {-1, -1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1, -1},
    { 0,  -2,  0,  0, 0,  0, 0,  0, 0,  0,  0,  -2,  0},
    {-1, -1,  0, -1, 0, -1, 0, -1, 0, -1,  0, -1, -1},
    {-1, -1,  0,  0, 0,  0, 0,  0, 0,  0,  0, -1, -1},
    {-1, -1, -1, -1, -2, -1, -2, -1, -2, -1, -1, -1, -1},
    {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1}
};
int maze_a[13][13] = {                                                                    //defining the maze
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
int field_define(int stop)                                                              //function to put the mines in the array
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

int route_finder(struct Station begin, struct Station end)                              //this maps all the possible routes, by increasing every number next to the start, and then the next number
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

struct Station stations(int station)                                                    // this converts the station number from the input to an array of coordinates in the matrix
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
int route_define(struct Station begin, struct Station end)                              //function that maps the crossroads in the chosen route into an array
{
    int index = maze[begin.row][begin.column];
    int i = begin.row;
    int j = begin.column;
    int amount = 0;
    while(index > 0)                                                                    //go from the highest value at the beginning, down to the end where the index is low
    {
        if((i % 2 == 0) && (j % 2 == 0) && (i != 0) && (i != 12) && (j != 0) && (j != 12))
        {
            route[amount].cross = 'c';                                                  //if a crossroad is passed it should be placed in the array
            route[amount].row = (i - 2)/2;
            route[amount].column = (j-2)/2;
            amount++; 
        }
        if(maze[i+1][j] == index - 1)                                                   //find in which place is the lower value to go to next
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
        index--;                                                                        //lower index value, so it goes to the next maze cell
    }
    route[amount].cross = 'e';                                                          //end of input in the array
    return 0;
}
int print_maze(void)                                                                    //function to print the whole 2d-maze array
{
    for(int i = 0; i < 13; i++) 
    {
        for(int j = 0; j < 13; j++)
        {
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
    return 0;
}

int direction(void)                        //I still have to change the inputs, but I am unfamiliar with this coding
{
    int i,j, index = 0;
    i = current_coord.row;
    j = current_coord.column; //these are youre current coordinates
    int route_found = 0;
    maze[i][j] = 1;
    int k,l;
    int k_target_cross,l_target_cross;
    while(route_found == 0) //and mines found is lower then 13
    {
        index++;
        for(k = 0; k < 13; ++k)
        {
            for(l = 0; l< 13; ++l)
            {
                if(maze[k][l] == index) //&& ((abs(i-k)+abs(j-l)) % 2 == 0)
                {
                    if(maze[k][l-1] == 0  && k+1 < 13 && l+1 < 13 && k-1 > -1 && l-1 > -1)
                    {
                        maze[k][l-1] = index + 1;
                        if (k % 2 == 0 && l % 2 == 0)
                        {
                        maze[k][l-2] = index + 2;
                        k_target_cross = k;
                        destination.row = k;
                        l_target_cross = l-1;
                        destination.column = l-2;  
                        route_found = 1;
                        break;
                        }
                    }
                    if(maze[k-1][l] == 0  && k+1 < 13 && l+1 < 13 && k-1 > -1 && l-1 > -1)
                    {
                        maze[k-1][l] = index + 1;
                        if (k % 2 == 0 && l % 2 == 0){
                        maze[k-2][l] = index + 2;  
                        k_target_cross = k-1;
                        destination.row = k-2;
                        l_target_cross = l;
                        destination.column = l;
                        route_found = 1;
                        break;
                        }
                    }
                    if(maze[k+1][l] == 0 && k+1 < 13 && l+1 < 13 && k-1 > -1 && l-1 > -1)
                    {
                        maze[k+1][l] = index + 1;
                        if (k % 2 == 0 && l % 2 == 0){
                        maze[k+2][l] = index + 2; //length is index + 2 dont know how to have this variable global
                        k_target_cross = k+1;
                        destination.row = k+2;
                        l_target_cross = l;
                        destination.column = l;
                        route_found = 1;
                        break;
                        }
                    }
                    if(maze[k][l+1] == 0  && k+1 < 13 && l+1 < 13 && k-1 > -1 && l-1 > -1)
                    {
                        maze[k][l+1] = index + 1;
                        if (k % 2 == 0 && l % 2 == 0){
                        maze[k][l+2] = index + 2;
                        k_target_cross = k;
                        destination.row = k;
                        l_target_cross = l+1;
                        destination.column = l+2;  
                        route_found = 1;
                        break;
                        }
                    }
                    if(maze[k+1][l] == -2)
                    {
                        maze[k+1][l] = index + 1;
                    }
                    if(maze[k-1][l] == -2)
                    {
                        maze[k-1][l] = index + 1;
                    }
                    if(maze[k][l+1] == -2)
                    {
                        maze[k][l+1] = index + 1;
                    }
                    if(maze[k][l-1] == -2)
                    {
                        maze[k][l-1] = index + 1;
                    }
                    }
                }
            if (route_found == 1){
                break;
            }  
            }
        }
        
    for (int m = 0; m<13; ++m){
        for (int n = 0; n<13; ++n){
            if (m != k_target_cross || n!= l_target_cross){ //change the values which are the same as the one which needs to be checked to remove the change of the robot routing the wrong way
                if (maze[m][n] == maze[k_target_cross][l_target_cross]){ //All other coordinates with the same value gets deleted so it doesnt go over a different road then planned 
                    maze[m][n] = -2;
                }
            }
        }
    }
    return 0;
}

int find_curr_location(int mine_found) //should get input mine found (run this command when a new crossing has been checked)
{
    //int mine_found = 0; //the mine_found input should be external
    current = find_current[mine_found];
    current_coord.row = current.row*2+2;
    current_coord.column = current.column*2+2; //this is used to design the route to the next closest not checked cross
    if (mine_found == 1){
        int row, column;
        row = find_current[1].row + find_current[0].row + 2;
        column = find_current[1].column + find_current[0].column + 2;
        maze[row][column] = -1;
    }
    return 0;
}

int clear_board(void) //unfamilar with these inputs so might need to be done differently 
{
    int i,j;

    for (i=0; i<13; ++i){
        for (j=0; j<13; ++j){
            if (maze[i][j] > 0){  //change all values which are higher then 0 to -2 since they are being visited or already visited
                maze[i][j] = -2;
            }
        }
    }
    return 0;
}

int route_define_c(void)  //change inputs                            //function that maps the crossroads in the chosen route into an array And input needs to change!!!
{
    int index = maze[destination.row][destination.column]; //end inputs of found crossroad as input
    int i = destination.row; //same here
    int j = destination.column; //same here
    int amount = 0;
    struct Cross temp;
    while(index > 0)                                                                    //go from the highest value at the beginning, down to the end where the index is low
    {
        if((i % 2 == 0) && (j % 2 == 0) && (i != 0) && (i != 12) && (j != 0) && (j != 12))
        {
            route[amount].cross = 'c';                                                  //if a crossroad is passed it should be placed in the array
            route[amount].row = (i - 2)/2;
            route[amount].column = (j-2)/2;
            amount++; 
        }
        if(maze[i+1][j] == index - 1)                                                   //find in which place is the lower value to go to next
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
        if(index == 1)
        {
            break;
        }
        index--;                                                                    //lower index value, so it goes to the next maze cell
    }
    find_current[0] = route[0]; //if mine is found we can decide which of these is our current location
    find_current[1] = route[1];
    for (int k = 0; k <= amount-1-k; ++k){ //rotate the array reverse it
        temp = route[k];
        route[k] = route[amount - 1 - k];
        route[amount - 1 -k] = temp;
    }
    route[amount].cross = 'e';                                                          //end of input in the array
    return 0;
}

int bernoulli(double p) {                   //This is only for testing
    double r = (double)rand() / RAND_MAX;
    if (r < p)
        return 1; // Success
    else
        return 0; // Failure
}

int main(void)
{
    current_coord.row = 12;
    current_coord.column = 4;  //these are the coordinates of the start location
    int mine_found = 0, j = 0;
    for (int i=0; i <= 13; ++i)
    {
        while(mine_found == 0){
            direction(); //this function finds the closest non-checked road
            print_maze(); //unnesecarry but I want to see if it works
            printf("\n");
            route_define_c(); //this function generates a route to the road found in direction
            while(route[j].cross != 'e')                                                        //print the crossroad array
            {
                printf("%c%d%d ", route[j].cross, route[j].row, route[j].column);
                route[j].cross = 0;
                route[j].row = 0;
                route[j].column = 0;
                j++;
            }
            printf("\n");
            route[j].cross = 0;
            clear_board();
            double p = 0.1;
            mine_found = bernoulli(p);
            //wait untill we have an update that the robot has completed its path ##but dont know how I get this signal
            find_curr_location(mine_found); //defines the location of the robot when the route is completed
        }
        mine_found = 0;
    }

}


int main_a(void)
{
    int k = 0, i = 0,stop, begin_station, end_station;
    print_maze();                                                                       //print the initial maze
    scanf("%d", &cross_in[0]);                                                          //scan the amount of mines
    stop = cross_in[0]*2 + 1;                                                           //calculate the needed amount of values in the location array
    for(k = 1; k < stop; k = k + 2)
    {
        scanf("%d %d %c", &cross_in[k], &cross_in[k+1], &cross_dir[(k-1)/2]);           //scan for the locations of the mines
    }
    scanf("%d %d", &begin_station, &end_station);                                       //scan for begin and end
    field_define(stop);                                                                 //put the mines in the 2d-maze array
    struct Station begin = stations(begin_station);
    struct Station end = stations(end_station);                                         //change the station values into station coordinates
    route_finder(begin, end);                                                           //map the possible routes
    print_maze();                                                                       //print the maze with the mines and the possible routes
    route_define(begin, end);                                                           //choose a route and put the crossroads in an array
    while(route[i].cross != 'e')                                                        //print the crossroad array
    {
        printf("%c%d%d ", route[i].cross, route[i].row, route[i].column);
        route[i].cross = 0;
        route[i].row = 0;
        route[i].column = 0;
        i++;
    }
    route[i].cross = 0;
    return 0;
}

