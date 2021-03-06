#include "main.h"

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {
  (void)arg;
  chRegSetThreadName("blinker");
  uint8_t half_period = 15; //ms
  while (true) {
    palSetPad(GPIOA, GPIOA_DIR_OUT);
    chThdSleepMilliseconds(half_period);
    palClearPad(GPIOA, GPIOA_DIR_OUT);
    chThdSleepMilliseconds(half_period);
  }
}

int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();
  init_io();

  // start the blinker thread
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  spiStart(&SPID1, &test_spicfg);
  // refer to testhal/*/SPI example for multithread SPI configuration

  uint8_t txbuffer[2];
  uint8_t rxbuffer[2];
  uint8_t offset = 0;
  uint8_t num_zero = 48;
  txbuffer[0] = 65; // A

  while (true) {
    chThdSleepMilliseconds(1);
    txbuffer[1] = num_zero + offset;
    spiExchange(&SPID1, 2, txbuffer, rxbuffer);
    txbuffer[0] = rxbuffer[1]; 
    offset++;
    if (offset > 9u) {
      offset = 0;  
    }
  }
}
