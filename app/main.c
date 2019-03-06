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

  // start PWM
  pwmStart(&pulse_PWM_dr, &pwmcfg);
  uint16_t percentage = 2000; // unit: 1/10_000
  uint8_t period = pwmcfg.period;
  pwmEnableChannel(&pulse_PWM_dr, pulse_PWM_ch, PWM_PERCENTAGE_TO_WIDTH (&pulse_PWM_dr, percentage));

  uint8_t state = 1;
  while (true) {
    chThdSleepMilliseconds(10);
    if (period > 40){
        period -= 30;
    }
    pwmChangePeriod(&pulse_PWM_dr, period);
    pwmEnableChannel(&pulse_PWM_dr, pulse_PWM_ch, PWM_PERCENTAGE_TO_WIDTH (&pulse_PWM_dr, percentage));

    // test the output
    //palWritePad(GPIOA, GPIOA_TEST_OUTPUT, state);
    //state ^= 1;

    if (! palReadPad(GPIOA, GPIOA_TEST_INPUT)){
        palSetPad(GPIOA, GPIOA_TEST_OUTPUT);        //// debugger
    } else {
        palClearPad(GPIOA, GPIOA_TEST_OUTPUT);
    }

  }
}
