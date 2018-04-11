// DOSKEY.HPP: scan codes returned by bios.  Allows reading of cursor
// pad, ctrl, alt, shift, etc.  Not a complete set.
#ifndef _DOSKEY_HPP_
#define  _DOSKEY_HPP_
#include <bios.h>

static enum doskey {
  CAP_A = 0x1e41,  // Capital Letter A
  LC_A = 0x1e61,  // Lower-Case letter a
  CAP_B = 0x3042,
  LC_B = 0x3062,
  CAP_C = 0x2e43,
  LC_C = 0x2e63,
  CAP_E = 0x1245,
  LC_E = 0x1265,
  CAP_N = 0x314e,
  LC_N = 0x316e,
  CAP_P = 0x1950,
  LC_P = 0x1970,
  CAP_R = 0x1352,
  LC_R = 0x1372,
  CAP_S = 0x1f53,
  LC_S = 0x1f73,
  CAP_T = 0x1454,
  LC_T = 0x1474,
  LEFT_ARROW = 0x4b00,
  RIGHT_ARROW = 0x4d00,
  UP_ARROW = 0x4800,
  DOWN_ARROW = 0x5000,
  ESCAPE = 0x11b
};

inline doskey get_doskey() {
  while(!bioskey(1));  // wait for a keypress
  return (doskey)bioskey(0);
}

#endif  _DOSKEY_HPP_
#if 0  // above keys are found with the following program:
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
#endif
