#include <stdio.h>
#include <bios.h>

int main(void)
{
   char ch;
   while(1) {
     printf("Input a character:");
     while(!bioskey(1));
     int key = bioskey(0);
     printf("key = 0x%x\n", key);
     if(key == 0x11b) return 0; // escape to quit
   }
}
