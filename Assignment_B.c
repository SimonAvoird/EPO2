#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <Windows.h>
#include <wchar.h>
#include <math.h>

struct Station {                                                                        //the structure for the station coordinates
    int row,column;
};
struct Cross {                                                                          //the structure for the final route
    char cross;
    int row;
    int column;
};
struct Cross route[25];
int maze[13][13] = {                                                                    //defining the maze
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
char output[25][8];

//code for the serial communication
#define COMPORT "COM5"
#define BAUDRATE CBR_9600

//--------------------------------------------------------------
// Function: initSio
// Description: intializes the parameters as Baudrate, Bytesize, 
//           Stopbits, Parity and Timeoutparameters of
//           the COM port
//--------------------------------------------------------------
void initSio(HANDLE hSerial){

    COMMTIMEOUTS timeouts ={0};
    DCB dcbSerialParams = {0};

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        //error getting state
        printf("error getting state \n");
    }

    dcbSerialParams.BaudRate = BAUDRATE;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;

    if(!SetCommState(hSerial, &dcbSerialParams)){
        //error setting serial port state
        printf("error setting state \n");
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;

    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if(!SetCommTimeouts(hSerial, &timeouts)){
    //error occureed. Inform user
        printf("error setting timeout state \n");
    }
}

//--------------------------------------------------------------
// Function: readByte
// Description: reads a single byte from the COM port into
//              buffer buffRead
//--------------------------------------------------------------
int readByte(HANDLE hSerial, char *buffRead) {

    DWORD dwBytesRead = 0;
    if (!ReadFile(hSerial, buffRead, 1, &dwBytesRead, NULL))
    {
        printf("error reading byte from input buffer \n");
    }
    printf("Byte read from read buffer is: %c \n", buffRead[0]);
    return(0);
}

//--------------------------------------------------------------
// Function: writeByte
// Description: writes a single byte stored in buffRead to
//              the COM port 
//--------------------------------------------------------------
int writeByte(HANDLE hSerial, char *buffWrite){

    DWORD dwBytesWritten = 0;
    
    if (!WriteFile(hSerial, buffWrite, 1, &dwBytesWritten, NULL))
    {
        printf("error writing byte to output buffer \n");
    }
    printf("Byte written to write buffer is: %c \n", buffWrite[0]);

    return(0);
}


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

int direction(void)
{
    int i,j, x = 0, y;
    int n=0,e=0,s=0,w=0;
    char direction;
    while(x==0)
    {
            switch(maze[i][j-1])
            {
                case(0):
                    direction = 'w';
                    x=1;
                    break;
                case(-1):
                    n = -1;
                    break;
                case(1):
                    n = 1;
                    break;
                if(x==1)
                {
                    break;
                }
            }

            switch(maze[i-1][j])
            {
                case(0):
                    direction = 'n';
                    x=1;
                    break;
                case(-1):
                    n = -1;
                    break;
                case(1):
                    n = 1;
                    break;
                if(x==1)
                {
                    break;
                }
            }

            switch(maze[i][j+1])
            {
                case(0):
                    direction = 'e';
                    x=1;
                    break;
                case(-1):
                    n = -1;
                    break;
                case(1):
                    n = 1;
                    break;
                if(x==1)
                {
                    break;
                }
            }
            switch(maze[i+1][j])
            {
                case(0):
                    direction = 's';
                    x=1;
                    break;
                case(-1):
                    n = -1;
                    break;
                case(1):
                    n = 1;
                    break;
                if(x==1)
                {
                    break;
                }
            }
    } 
    
}

int route_maker(void)
{
    int k = 0, i = 0,stop, begin_station, end_station;
    print_maze();                                                                       //print the initial maze
    /*scanf("%d", &cross_in[0]);                                                          //scan the amount of mines
    stop = cross_in[0]*2 + 1;                                                           //calculate the needed amount of values in the location array
    for(k = 1; k < stop; k = k + 2)
    {
        scanf("%d %d %c", &cross_in[k], &cross_in[k+1], &cross_dir[(k-1)/2]);           //scan for the locations of the mines
    }*/
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
        i++;
    }
}

int synth_output(void)
{
    int i = 0, row, column;
    int output[8] = {0};
    HANDLE hSerial;

    char byteBuffer[BUFSIZ+1] = {0};

    //----------------------------------------------------------
    // Open COMPORT for reading and writing
    //----------------------------------------------------------
    hSerial = CreateFile(COMPORT,
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0
    );

    if(hSerial == INVALID_HANDLE_VALUE){
        if(GetLastError()== ERROR_FILE_NOT_FOUND){
            //serial port does not exist. Inform user.
            printf(" serial port does not exist \n");
        }
        //some other error occurred. Inform user.
        printf(" some other error occured. Inform user.\n");
    }

    //----------------------------------------------------------
    // Initialize the parameters of the COM port
    //----------------------------------------------------------

    initSio(hSerial);

    while(route[i].cross != 'e')
    {
        row = route[i].row;
        column = route[i].column;
        for(int j = 2; j >-1; j--)
        {
            if(row != 0)
            {
                if(row % 2 == 0)
                {    
                    output[j] = 0;
                }
                else
                {
                    output[j] = 1;
                }
                row = row/2;
            }
            else 
            {
                output[j] = 0;
            }
            
        }
        for(int j = 5; j > 2; j--)
        {
            if(column != 0)
            {
                if(column % 2 == 0)
                {
                    output[j] = 0;
                }
                else
                {
                    output[j] = 1;
                }
                column = column/2;
            }
            else
            {
                output[j] = 0;
            }
        }
        byteBuffer[0] = 0;
        for(int k=0; k<8; k++)
        {
            byteBuffer[0] += pow(2, (7-k))*output[k];
        }
        writeByte(hSerial, byteBuffer);
        Sleep(3000);
        i++;
    }
    CloseHandle(hSerial);
    return 0;
}
int main(void)
{
    route_maker();
    synth_output();
}