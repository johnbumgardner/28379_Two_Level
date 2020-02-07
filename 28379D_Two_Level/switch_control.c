/*
 * switch_cotntrol.c
 *
 *  Created on: Oct 29, 2019
 *      Author: John Bumgardner
 */


#include "device.h"
#include "driverlib.h"
#include "macros.h"
#include "functions.h"

void init_switches(){
    //configure switch 1
    GPIO_setPadConfig(SWITCH_1, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(SWITCH_1, GPIO_DIR_MODE_OUT);

    //configure switch 2
    GPIO_setPadConfig(SWITCH_2, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(SWITCH_2, GPIO_DIR_MODE_OUT);

    //configure switch 3
    GPIO_setPadConfig(SWITCH_3, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(SWITCH_3, GPIO_DIR_MODE_OUT);

    //configure switch 4
    GPIO_setPadConfig(SWITCH_4, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(SWITCH_4, GPIO_DIR_MODE_OUT);

    //configure switch 5
    GPIO_setPadConfig(SWITCH_5, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(SWITCH_5, GPIO_DIR_MODE_OUT);

    //configure switch 6
    GPIO_setPadConfig(SWITCH_6, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(SWITCH_6, GPIO_DIR_MODE_OUT);

    //configure the LED to blink
    GPIO_setPadConfig(DEVICE_GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);
}

void turn_on_switch_1(){
    GPIO_writePin(SWITCH_1, 1);
}
void turn_on_switch_2(){
    GPIO_writePin(SWITCH_2, 1);
}
void turn_on_switch_3(){
    GPIO_writePin(SWITCH_3, 1);
}
void turn_on_switch_4(){
    GPIO_writePin(SWITCH_4, 1);
}
void turn_on_switch_5(){
    GPIO_writePin(SWITCH_5, 1);
}
void turn_on_switch_6(){
    GPIO_writePin(SWITCH_6, 1);
}

void turn_off_switch_1(){
    GPIO_writePin(SWITCH_1, 0);
}
void turn_off_switch_2(){
    GPIO_writePin(SWITCH_2, 0);
}
void turn_off_switch_3(){
    GPIO_writePin(SWITCH_3, 0);
}
void turn_off_switch_4(){
    GPIO_writePin(SWITCH_4, 0);
}
void turn_off_switch_5(){
    GPIO_writePin(SWITCH_5, 0);
}
void turn_off_switch_6(){
    GPIO_writePin(SWITCH_6, 0);
}


// <100>
void activate_positive_u(){
    turn_on_switch_1();
    turn_off_switch_2();
    turn_off_switch_3();
    turn_on_switch_4();
    turn_off_switch_5();
    turn_on_switch_6();
}
// <010>
void activate_positive_v(){
    turn_off_switch_1();
    turn_on_switch_2();
    turn_on_switch_3();
    turn_off_switch_4();
    turn_off_switch_5();
    turn_on_switch_6();
}
// <011>
void activate_negative_w(){
    turn_on_switch_1();
    turn_off_switch_2();
    turn_on_switch_3();
    turn_off_switch_4();
    turn_off_switch_5();
    turn_on_switch_6();
}
// <100>
void activate_positive_w(){
    turn_off_switch_1();
    turn_on_switch_2();
    turn_off_switch_3();
    turn_on_switch_4();
    turn_on_switch_5();
    turn_off_switch_6();
}
// <101>
void activate_negative_v(){
    turn_on_switch_1();
    turn_off_switch_2();
    turn_off_switch_3();
    turn_on_switch_4();
    turn_on_switch_5();
    turn_off_switch_6();
}
// <110>
void activate_negative_u(){
    turn_off_switch_1();
    turn_on_switch_2();
    turn_on_switch_3();
    turn_off_switch_4();
    turn_on_switch_5();
    turn_off_switch_6();
}
// <000>
void activate_zero_vector(){
    turn_off_switch_1();
    turn_on_switch_2();
    turn_off_switch_3();
    turn_on_switch_4();
    turn_off_switch_5();
    turn_on_switch_6();
}
