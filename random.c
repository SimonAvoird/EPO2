#include<stdio.h>
#include<time.h>
#include<stdlib.h>


int main(void)
{
    for(int i = 0; i < 1000; i++)
    {
        int r = rand() % 10;
        if(r ==1)
        {
            r = 1;
        }
        else
        {
            r = 0;
        }
        printf("%d", r);
    }
    return 0;
}