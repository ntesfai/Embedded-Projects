/* These definitions are architecture dependent - a
   more portable method would be to use the atmega
   libraries
*/
#define DDRB    (*((char *)0x17))
#define PORTB   (*((char *)0x18))

unsigned int delay(unsigned int delay_count) {
/* delay a certain number of counts - this is really bad
   code.  in practice you should never hard-code delays
   in a way that they might change with hardware changes.
 */
  unsigned int i;
  for (i=0; i<delay_count; i++) {
	/* do nothing - note that a good compiler might
           optimize this all away.
	*/
  }
  return delay_count;
}

int main(void) {
  DDRB = 0xff;          /* make portc an output */
  
  /* infinite loop */
  while(1) 
  {
    PORTB = 0xff; 	/* set all output bits high */
    delay(1000); 	/* wait one second */
    PORTB = 0x00; 	/* turn all output bits off */
    delay(1000); 	/* wait one second */
  }
}


