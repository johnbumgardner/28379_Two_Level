/*
 * File:   functions.h
 * Author: john.bumgardner
 *
 * Created on October 10, 2019, 6:01 PM
 */

//functions to help the DSP determine what vectors it needs to modulate, and for how long
int get_sector(int *angle_ptr);
int* get_modulated_array(int* sector_ptr, int* magnitude_ptr);
int* get_percents_to_modulate(int* top_of_vector_array, int* angle_ptr, int* magnitude_ptr);
int get_upper_angle(int* sector_ptr);
int get_lower_angle(int* sector_ptr);

int roundLocal(float d);



//functions to control the switching of the DSP
void control_switches(int* top_of_vector_arr, int* top_of_percent_arr);
void turn_on_switch(int* vector_ptr);
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





