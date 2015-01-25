#include "platform.h"

/*         platform specific time handling code         */
static volatile mytime_t jiffies = 0; /* configure 4ms for each tick */

ISR(TIMER0_OVF_vect, ISR_NOBLOCK) {
  jiffies++;
}

int16_t mydifftime(mytime_t time1, mytime_t time0) {
  int16_t result = time1;
  result -= (int16_t)time0;
  return result;
}

mytime_t mytime(mytime_t *timer) {
  mytime_t result;
  do {
    result = jiffies;
  } while (result != jiffies);
  
  if (timer) (*timer)=result;
  return result;
}

void init_timer(void) {
#if (F_CPU == 16000000)
  TCNT0 = 0;
#	if !(defined (__AVR_ATmega8__) || defined (__AVR_ATmega8A__) || defined (__AVR_ATmega16__) || defined (__AVR_ATmega32__))
      TIMSK0	|= _BV(TOIE0);
      TCCR0B	 = 1*_BV(CS02) | 0*_BV(CS01) | 0*_BV(CS00); // prescaler 256 - so timer0 overflows about every 4ms
#	else
      TIMSK	|= _BV(TOIE0);
      TCCR0	 = 1*_BV(CS02) | 0*_BV(CS01) | 0*_BV(CS00); // prescaler 256 - so timer0 overflows about every 4ms
#	endif
#else
#	error unknown frequency - timer calibration not implemented for this clock
#endif
      sei();
}

/* **END** platform specific time handling code **END** */
