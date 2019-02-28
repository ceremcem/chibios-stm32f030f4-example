#include "main.h"
#include "io.h"

thread_t * motion_t;
bool motion_enable = false;

int main(void)
{
	halInit();
	chSysInit();

    init_io();

    start_motion();

    pwmStart(&pulse_PWMD, &pwmcfg);
    pwmEnableChannel(&pulse_PWMD, 3, PWM_PERCENTAGE_TO_WIDTH (&pulse_PWMD, 5000));


	/*Main task loop*/
    pwmcnt_t period = 1000000;
	while(!0)
	{
        //chThdSleepMilliseconds(1000);
        //pwmEnableChannel(&pulse_PWMD, 3, PWM_PERCENTAGE_TO_WIDTH (&pulse_PWMD, 3000));
        chThdSleepMilliseconds(1000);
        pwmChangePeriod(&pulse_PWMD, 30);
        pwmEnableChannel(&pulse_PWMD, 3, PWM_PERCENTAGE_TO_WIDTH (&pulse_PWMD, 5000));
        if (period > 10000){
            period -= 1000;
        }
	}
}

void stop_motion(){
    //chThdTerminate(motion_t);
    //chThdWait(motion_t);
    motion_enable = false;
}

void start_motion(){
    motion_t = chThdCreateStatic(wa_ramp, sizeof(wa_ramp),
        HIGHPRIO, ramp, NULL);
}

void set_dir(bool dir){
    if (dir == TRUE){
        palSetPad(GPIOA, DIR_OUT);
    } else {
        palClearPad(GPIOA, DIR_OUT);
    }
    //palWritePad(GPIOA, DIR_OUT, dir);
}

bool reached_top(){
    return ! palReadPad(GPIOA, UPPER_LIMIT_SWITCH);
}

bool reached_bottom(){
    return ! palReadPad(GPIOA, LOWER_LIMIT_SWITCH);
}

void move_up(){
    if (! reached_top()){
        set_dir(UPWARD);
        motion_enable = true;
    }
}

void move_down(){
    if (! reached_bottom()){
        set_dir(DOWNWARD);
        motion_enable = true;
    }
}

void UPWARD_button(bool pressed){
    if (pressed){
        move_up();
    } else {
        stop_motion();
    }
}

void DOWNWARD_button(bool pressed){
    if (pressed){
        move_down();
    } else {
        stop_motion();
    }
}

void button_callback(uint8_t pad){
    bool state = ! palReadPad(GPIOA, pad); //// debugger
    if (pad == UPWARD_BUTTON){
        UPWARD_button(state);
    } else {
        DOWNWARD_button(state);
    }
}

void limit_switch(uint8_t pad){
    bool state = ! palReadPad(GPIOA, pad);

    if (state == PAL_HIGH){
        stop_motion();
    }
}
