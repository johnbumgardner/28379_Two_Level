/*
 * File:   operation.c
 * Author: john.bumgardner
 *
 * Created on October 10, 2019, 3:51 PM
 */


#include "functions.h"
#include "macros.h"


/*
 * Function actually operates the switch operation
 * Inputs:array of the vectors that it must modulate, along with the percent to spend on each
 * Should run from call to power off condition
 */
void control_switches(int* top_of_vector_arr, int* top_of_percent_arr){
   //run until the power turns off
    //while(1){
        int i;
        //during a cycle of 100, spend time on each vector based on how long the percent calls for
        for(i = 0; i < 100; i++){
            int timer = 0;
            int j;
            for(j = 0; j < SIZEOFMODULATEDARRAY; j++){
                if(top_of_vector_arr[j] >= 0){ //make sure we want to modulate said vector
                    while(timer < top_of_percent_arr[j]){

                        //turn on the switches to correspond to current modulating vector
                        turn_on_switch(&top_of_vector_arr[j]);
                        timer++;
                    }
                    timer = 0; //reset timer
                }
            }
            turn_off_switches();
        }
    //}
}

/*
 * Function configures the pins on the DSP to achieve a given switch configuration
 * Input - pointer to a given state vector we wish the hardware to exhibit
 *
 */
void turn_on_switch(int* vector_ptr){
    int vector = *vector_ptr;
    switch(vector){
        case POSITIVE_U: //+U
            //printf("+U is on\n");
            activate_positive_u();
            break;
        case POSITIVE_V: //+V
            //printf("+V is on\n");
            activate_positive_v();
            break;
        case POSITIVE_W: //+W
            //printf("-W is on\n");
            activate_positive_w();
            break;
        case NEGATIVE_W: //-U
            //printf("+W is on\n");
            activate_negative_w();
            break;
        case NEGATIVE_V: //-V
            //printf("-V is on\n");
            activate_negative_v();
            break;
        case NEGATIVE_U: //-U
            //printf("-U is on\n");
            activate_negative_u();
            break;
        case 0:
        case ZERO_VECTOR: //zero vector
            //printf("zero vector is on\n");
            activate_zero_vector();
            break;
    }
}


/*
 * Function turns off all the switch pins
 * Designed to avoid having conditions where a switch
 * would be simultaneously off and on
 *
 */
void turn_off_switches(){
    //printf("switches are temp off.\n");
    activate_zero_vector();
}
