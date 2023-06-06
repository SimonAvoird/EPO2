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
const int empty_maze[13][13] = {                                                                    //defining the maze
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
int maze[13][13];
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

int route_define(struct Station begin, struct Station end, int amount)                              //function that maps the crossroads in the chosen route into an array
{
    int index = maze[begin.row][begin.column];
    int i = begin.row;
    int j = begin.column;
    
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
    return amount;
}

int route_maker(void)
{
    int station[4];
    int k = 0, i = 0, amount = 0;
    scanf("%d %d %d %d", &station[0], &station[1], &station[2], &station[3]);                                       //scan for begin and end
    for( k = 0; k<3;k++)
    {
        for(int l = 0; l<13;l++)
        {
            for(int m = 0; m<13; m++)
            {
                maze[l][m] = empty_maze[l][m];
            }
        }
        struct Station begin = stations(station[k]);
        struct Station end = stations(station[k+1]);                                        //change the station values into station coordinates
        route_finder(begin, end);                                                           //map the possible routes
        amount = route_define(begin, end, amount);                                          //choose a route and put the crossroads in an array
    }
    route[amount].cross = 'e';                                                          //end of input in the array
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
        Sleep(500);
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