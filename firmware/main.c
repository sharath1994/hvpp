
#include "main.h"
#include "tinyusbboard.h"
#include "apipage.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

#include "usbdrv.h"
#include "oddebug.h"
#include <util/delay.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#include "platform.c"

void updatePolls(void) {
  usbPoll();
}

void toggle_all(void) {
  TOGGLE(LED_B);
  TOGGLE(LED_PWM);
  TOGGLE(LED_LEFT);
  TOGGLE(LED_RIGHT);
}

uint8_t rotate_led(const uint8_t state) {
  if (state==0) {
    SET_LOW(LED_LEFT);
    SET_HIGH(LED_B);
  } else if (state==1) {
    SET_LOW(LED_B);
    SET_HIGH(LED_PWM);   
  } else if (state==2) {
    SET_LOW(LED_PWM);   
    SET_HIGH(LED_RIGHT);   
  } else if (state==3) {
    SET_LOW(LED_RIGHT);   
    SET_HIGH(LED_LEFT);   
    return 0;
  } else {
    return 0;
  }
  return state+1;
}


USB_PUBLIC usbMsgLen_t usbFunctionSetup(uchar data[8]) {
  return 0;
}

int main(void) {
  uint8_t rotcounter;
  mytime_t last, current;

// basic initialization of the controller
#ifdef MCUCSR
  MCUCSR=0;
#else
  MCUSR=0;
#endif
  wdt_disable();
  usbDeviceDisconnect();
  init_timer();

// peripheral initialization
  CFG_PULLUP(BUTTON_PROG);
  CFG_OUTPUT(LED_B);
  CFG_OUTPUT(LED_PWM);
  CFG_OUTPUT(LED_LEFT);
  CFG_OUTPUT(LED_RIGHT);

// USB connect delay: about 300ms
  last=mytime(NULL);
  while (mydifftime(mytime(&current), last) > TOSYSTEMTIME(300)) {
    wdt_reset();
  }

  usbDeviceConnect();
  usbInit();

// all the firmware does at the moment: rotate leds every 60ms
  rotcounter=0;
  while (1) {
    wdt_reset();
    if (mydifftime(mytime(&current), last) >= TOSYSTEMTIME(60)) {
      rotcounter=rotate_led(rotcounter);
      last=current;
    }
    updatePolls();
  }

// disconnect the USB and start the bootloader
  usbDeviceDisconnect();
  SET_HIGH(LED_RIGHT);
  bootloader_startup(); /* on success it never returns */

  /* 
   * this should never be reached by the IP 
   * probably the bootloader doesn't support the firmware entry point or
   * some unknown watchdog timeout happend (even when the watchdog was disabled)...
   * anyhow: Show watchdog persistent error if it happens 
   */
  wdt_reset(); /* in case sth. went wrong in bootloader_startup and watchdog staid on */
  SET_LOW(LED_PWM);
  SET_LOW(LED_LEFT);
  SET_HIGH(LED_B);
  SET_HIGH(LED_RIGHT);
  while (1) {
    wdt_reset();
    if (mydifftime(mytime(&current), last) >= TOSYSTEMTIME(500)) {
      toggle_all();
      last=current;
    }
  }
  
  return 0;
}
