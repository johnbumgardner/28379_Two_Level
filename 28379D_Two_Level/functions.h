/*
 * File:   functions.h
 * Author: john.bumgardner
 *
 * Created on October 10, 2019, 6:01 PM
 */

#include "driverlib.h"
#include "device.h"

//functions to help the DSP determine what vectors it needs to modulate, and for how long
int get_sector(double *angle_ptr);
int* get_modulated_array(int* sector_ptr, int* magnitude_ptr);
int* get_percents_to_modulate(int* top_of_vector_array, double* angle_ptr, int* magnitude_ptr);
int get_upper_angle(int* sector_ptr);
int get_lower_angle(int* sector_ptr);

int roundLocal(float d);
float round_to_tenths(float d);


//functions to control the switching of the DSP
void control_switches(char* top_of_vector_arr, unsigned char* top_of_percent_arr);
void turn_on_switch(char* vector_ptr);
void turn_off_switches();

//GPIO setup and control
void init_switches();
void turn_on_switch_1();
void turn_on_switch_2();
void turn_on_switch_3();
void turn_on_switch_4();
void turn_on_switch_5();
void turn_on_switch_6();

void turn_off_switch_1();
void turn_off_switch_2();
void turn_off_switch_3();
void turn_off_switch_4();
void turn_off_switch_5();
void turn_off_switch_6();

void activate_positive_u();
void activate_positive_v();
void activate_negative_w();
void activate_positive_w();
void activate_negative_v();
void activate_negative_u();
void activate_zero_vector();

// Function prototypes for the timer interrupts
__interrupt void cpuTimer0ISR(void);
__interrupt void cpuTimer1ISR(void);
__interrupt void cpuTimer2ISR(void);
void initCPUTimers(void);
void configCPUTimer(uint32_t, float, float);

// Linked List node definition
struct Node {
    char vectors[3]; // stores vectors to modulate
    unsigned char percentages[3]; // stores percents to modulate
    struct Node* next; // points to the next node in the list
};

// Linked List helper functions
struct Node * get_vector_list(int * size);
struct Node * get_end_node(struct Node * head);

// functions to handle the frequency
int get_interrupt_time(long* switching);
int get_divisions(long* switching, long* fundamental);
double get_differential_angle(long* switching, long* fundamental);
