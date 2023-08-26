/***********************************************************************************

math rom

INPUTS
 - argument A, 8 bits
 - argument B, 8 bits
 - function select, 2 bits
   0 A * B
   1 A / B
   2 sqrt(A) * B (use B for fixed point math adjust)
   3 binary revrse A

OUTPUTS
 - result
 
FEEDBACK
 N/A



Address bits      8 bit rom size

       -- no parallel roms available --
     8                  2 k
     9                  4 k
     10                 8 k
     
       -- eeproms available from here --
     11                 16 k  (28C16)
     12                 32 k  (28C32)
     
       -- eprom practical sizes from here --
     13                 64 k  (2764)
     14                 128 k (27128)
     15                 256 k 
     16                 512 k
     17                 1 M  (27010)
     18                 2 M
     19                 4 M
     20                 8 M

       -- flash from here up --



**************************************************************************************/


#include <stdio.h>
#include <stdint.h>
#include "ROMLib.h"
#include <math.h>


// the number of address lines you need
#define InputBits 6

// the output data size, 8 or 16
#define OutputBits 8

// default output value
#define DFOutput  0x00



// geez, tuck this one away.  Please dont use this in real fft code,
//   YOU KNOW how many bits your working with, and you can use a 
//   specific case generator for it.
uint8_t uniReverse(uint8_t i, uint8_t bits) {

  uint8_t r, m, b;
  r = 0;             // result
  m = 1 << (bits-1); // mask will travel right
  b = 1;             // bit will travel left
  
  while(m) {
    if (i&b) r |=m;
    b <<= 1;
    m >>= 1;  
  }
  
  return r;  

}



int main(void) {

  uint16_t V1I, V2I;     // values are 16 bits      
  uint32_t resultO ;                 // bit fields are 32 bits

  uint32_t out;

  setup();  // open output file.
  
  
  // loop thru each address
  for( A=0; A<(1<<InputBits); A++) { // A is a bitfield (a mash-up of the vars we want)
  
     // assign default values for outputs     
     out       = DFOutput;
     
     // build input values
     spliceValueFromField( &V1I,            A,  3,   0, 1, 2);        // argument A, 8 bits
     spliceValueFromField( &V2I,            A,  3,   3, 4, 5);  // argument B, 8 bits



     // do task
     if (V2I != 4) {
       resultO = 0xFF;
     } else {
       resultO = (1<<V1I) ^ 0xFF;
     }


     
   
     // reconstitute the output
     spliceFieldFromValue( &out, resultO,     8,  0,1,2,3,4,5,6,7);
     
     
     // submit to file
     write(fd, &out, OutputBits>>3);  // in bytes
     printf("%d, ", out);
  }
  
  cleanup(); // close file
//  printf("}\n");
  return 0;
}




