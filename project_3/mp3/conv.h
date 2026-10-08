#include"proto.h"

//covert little to big endian as well as big to little endian
uint convert(uint num)
{
    uint temp=0;

    for(int i=0;i<4;i++)
    temp = temp | ((num>>(i*8))& 0xFF) << ((3-i)*8);
    return temp;
}

//FF if->1111 so 2f->8bits '1'
//ryt shift 8 bits do '&' with 8bits FF, so it take that particular 8bit olny others as '0'
//now it left shift to msb side repeat based on byte size
