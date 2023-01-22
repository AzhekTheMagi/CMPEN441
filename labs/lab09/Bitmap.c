/**
 * Name: Anthony Vallin, aav5195
 * CMPEN 441, Lab09
 * Date: 20210718
 **/

#include<math.h>
#include<stdio.h>
#include<stdlib.h>

#define bitmapsize 8

unsigned int Mem[] = {143, 112, 255, 15, 0, 255, 227, 7};

int Mallocate(int Mem_Size);
int binary_conversion(int num);
void Free(int Address, int Size);

int main(void) 
{
    unsigned int address_space;
    unsigned int request_space;

    //Test case 1
    request_space = 6;
    address_space = Mallocate(request_space);
    printf("Address: %i \n", binary_conversion(address_space));
    
    Free(address_space, request_space);
    
    printf("Address Bitmap after Free: \n");
    for (int i = 0; i < bitmapsize; i++)
    {
        printf("%08i \n", binary_conversion(Mem[i]));
    }
    
    return 0;
}

int Mallocate(int Mem_Size)
{
    unsigned int mask = 0;
    int counter = 7;
    int count2Kb = (int)ceil(Mem_Size / 2);
    unsigned int result;
    
    for (int i = 0; i < count2Kb; i++)
    {
        mask = mask | (1 << counter);
        counter--;
    }

    //printf("%i\n", binary_conversion(mask)); //debug

    for (int i = 0; i < bitmapsize; i++)
    {
        result = mask ^ Mem[i];
        result = mask & result;
        //printf("%08i\n", binary_conversion(result)); //debug

        if (result != mask)
        {
            mask = mask >> 1;
            result = mask ^ Mem[i];
            result = mask & result;
            //printf("%08i\n", result); //debug
            
            if (result == mask)
            {
                //printf("%08i\n", binary_conversion(result)); //debug
                for (int k = 0; k < bitmapsize; k++)
                {
                    if (Mem[k] && result)
                    {
                        return Mem[k];
                    }
                }
            }
        }
        else
            return Mem[i];
    }

    return -1;
}

void Free(int Address, int Size)
{
    for (int i = 0; i < bitmapsize; i++)
    {
        if (binary_conversion(Address) == binary_conversion(Mem[i]))
        {
            Mem[i] = 0;
        }
    }
}

int binary_conversion(int num)
{
    if (num == 0)
    {
        return 0;
    }
    else
    {
        return (num % 2) + 10 * binary_conversion(num / 2);
    }
}