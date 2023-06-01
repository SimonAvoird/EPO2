#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <wchar.h>

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

int def_main()
{
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

    while ( 1 ) {
        
        gets(byteBuffer);

        if (byteBuffer[0] == 'q') // end the loop by typing 'q'
            break;

        writeByte(hSerial, byteBuffer);
        readByte(hSerial, byteBuffer);
    }

    printf("ZIGBEE IO DONE!\n");
    return 0;

    CloseHandle(hSerial);
}

char convert_array (const int array[8])
{
  char result = 0;
  for(size_t i=0; i<8; i++)
  {
    result |= array[i] << (7-i);
  }
  return result;
}

int main()
{
    HANDLE hSerial;
    char values[][8] =
    {
        {0,1,1,0,1,1,1,1},
        {1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0}
    };
    char results[5][8];

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
    for(size_t i=0; i<8; i++)
    {
        byteBuffer[0] |= values[0][i] << (7-i);
    }
    writeByte(hSerial, byteBuffer);
    Sleep(3000);
    readByte(hSerial, byteBuffer);
    atoi(itoa(byteBuffer[0], results[0], 2));

for(size_t i=0; i<8; i++)
    {
        byteBuffer[0] |= values[1][i] << (7-i);
    }
    writeByte(hSerial, byteBuffer);
    Sleep(3000);
    readByte(hSerial, byteBuffer);
    atoi(itoa(byteBuffer[0], results[1], 2));

for(size_t i=0; i<8; i++)
    {
        byteBuffer[0] |= values[2][i] << (7-i);
    }
    writeByte(hSerial, byteBuffer);
    Sleep(3000);
    readByte(hSerial, byteBuffer);
    atoi(itoa(byteBuffer[0], results[2], 2));

    printf("ZIGBEE IO DONE!\n");
    return 0;

    CloseHandle(hSerial);
}