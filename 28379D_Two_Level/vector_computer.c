/*
 * File:   vector_computer.c
 * Author: john.bumgardner
 *
 * Created on October 10, 2019, 3:51 PM
 */

#include "macros.h"
#include "functions.h"

/*
 * returns an array indicating what vectors need to be modulated to get desired output
 * input1 - pointer to the location of the desired output vector
 * input2 - pointer to the desired magnitude of the desired output vector
 * output - pointer to the top of the vector array
 */
int* get_modulated_array(int* sector_ptr, int* magnitude_ptr){
    static int vectors[4] = {-1, -1, -1, -1}; //create array to store the modulated PWM vectors

    int sector = *(sector_ptr); //get the actual value of the sector
    int magnitude = *(magnitude_ptr);
    if(magnitude == 100){
        switch(sector){
            //for the instances where the angle lies on a single vector
            case POSITIVE_U:
                vectors[0] = POSITIVE_U;
                break;
            case POSITIVE_W:
                vectors[0] = POSITIVE_W;
                break;
            case POSITIVE_V:
                vectors[0] = POSITIVE_V;
                break;
            case NEGATIVE_U:
                vectors[0] = NEGATIVE_U;
                break;
            case NEGATIVE_W:
                vectors[0] = NEGATIVE_W;
                break;
            case NEGATIVE_V:
                vectors[0] = NEGATIVE_V;
                break;
            //for angles not along a vector
            case SECTOR_1:
                vectors[0] = POSITIVE_U;
                vectors[1] = POSITIVE_W;
                break;
            case SECTOR_2:
                vectors[0] = POSITIVE_W;
                vectors[1] = POSITIVE_V;
                break;
            case SECTOR_3:
                vectors[0] = POSITIVE_V;
                vectors[1] = NEGATIVE_U;
                break;
            case SECTOR_4:
                vectors[0] = NEGATIVE_U;
                vectors[1] = NEGATIVE_W;
                break;
            case SECTOR_5:
                vectors[0] = NEGATIVE_W;
                vectors[1] = NEGATIVE_V;
                break;
            case SECTOR_6:
                vectors[0] = NEGATIVE_V;
                vectors[1] = POSITIVE_U;
                break;
        }
    }
    else{
        //modulate similarly using the 000 and 111 vectors to adjust the magnitude
        switch(sector){
            //for the instances where the angle lies on a single vector
            case POSITIVE_U:
                vectors[0] = POSITIVE_U;
                vectors[2] = ZERO_VECTOR;
                vectors[3] = ZERO_VECTOR;
                break;
            case POSITIVE_V:
                vectors[0] = POSITIVE_W;
                vectors[2] = ZERO_VECTOR;
                vectors[3] = ZERO_VECTOR;
                break;
            case POSITIVE_W:
                vectors[0] = POSITIVE_V;
                vectors[2] = ZERO_VECTOR;
                vectors[3] = ZERO_VECTOR;
                break;
            case NEGATIVE_U:
                vectors[0] = NEGATIVE_U;
                vectors[2] = ZERO_VECTOR;
                vectors[3] = ZERO_VECTOR;
                break;
            case NEGATIVE_W:
                vectors[0] = NEGATIVE_W;
                vectors[2] = ZERO_VECTOR;
                vectors[3] = ZERO_VECTOR;
                break;
            case NEGATIVE_V:
                vectors[0] = NEGATIVE_V;
                vectors[2] = ZERO_VECTOR;
                vectors[3] = ZERO_VECTOR;
                break;
            //for angles not along a vector
            case SECTOR_1:
                vectors[0] = POSITIVE_U;
                vectors[1] = POSITIVE_W;
                vectors[2] = ZERO_VECTOR;
                vectors[3] = ZERO_VECTOR;
                break;
            case SECTOR_2:
                vectors[0] = POSITIVE_W;
                vectors[1] = POSITIVE_V;
                vectors[2] = ZERO_VECTOR;
                vectors[3] = ZERO_VECTOR;
                break;
            case SECTOR_3:
                vectors[0] = POSITIVE_V;
                vectors[1] = NEGATIVE_U;
                vectors[2] = ZERO_VECTOR;
                vectors[3] = ZERO_VECTOR;
                break;
            case SECTOR_4:
                vectors[0] = NEGATIVE_U;
                vectors[1] = NEGATIVE_W;
                vectors[2] = ZERO_VECTOR;
                vectors[3] = ZERO_VECTOR;
                break;
            case SECTOR_5:
                vectors[0] = NEGATIVE_W;
                vectors[1] = NEGATIVE_V;
                vectors[2] = ZERO_VECTOR;
                vectors[3] = ZERO_VECTOR;
                break;
            case SECTOR_6:
                vectors[0] = NEGATIVE_V;
                vectors[1] = POSITIVE_U;
                vectors[2] = ZERO_VECTOR;
                vectors[3] = ZERO_VECTOR;
                break;
            }
        }
    return &vectors[0];
}



/*
 * function that returns the sector the control vector resides within
 * input (angle) of the control vector in integer format
 * if the angle lies along a vector, return the 0-5 corresponding to the vector
 * output integer corresponding to the sector, 6-11
 */
int get_sector(int *angle_ptr){
    //correct the angle if it exceeds 360
    int sector;
    int angle = *(angle_ptr);
    while(angle > 360){
        angle = angle - 360;
    }
    //if the angle falls along a vector
    switch(angle){
        case 0:
        sector = POSITIVE_U;
        break;
        case 60:
        sector = POSITIVE_W;
        break;
        case 120:
        sector = POSITIVE_V;
        break;
        case 180:
        sector = NEGATIVE_U;
        break;
        case 240:
        sector = NEGATIVE_W;
        break;
        case 300:
        sector = NEGATIVE_V;
        break;
        case 360:
        sector = POSITIVE_U;
        break;
    }
    //if the vector lies in between vectors
    if(angle > 0 && angle < 60){
        sector = SECTOR_1;
    }
    else if(angle > 60 && angle < 120){
        sector = SECTOR_2;
    }
    else if(angle > 120 && angle < 180){
        sector = SECTOR_3;
    }
    else if(angle > 180 && angle < 240){
        sector = SECTOR_4;
    }
    else if(angle > 240 && angle < 300){
        sector = SECTOR_5;
    }
    else if(angle > 300 && angle < 360){
        sector = SECTOR_6;
    }

    return sector;
}

/*
 * Returns an array containing the percentage of time to modulate the nth vector
 * to achieve desired angle and magnitude
 * Inputs: pointer to top of the vectors to be modulated array, pointer to the
 * angle, and pointer to the magnitude
 * Output: pointer to the top of the array containing the percentages
 * Implements an algo to compute amount to be modulated
 */
int* get_percents_to_modulate(int* top_of_vector_array, int* angle_ptr, int* magnitude_ptr){
    //get the sector the function is located in
    int sector = get_sector(angle_ptr);
    static int percentages[4]; //array to store the percentages
    //dereference the input pointers
    int angle = *angle_ptr;
    int magnitude = *magnitude_ptr;

    // if the desired angle lies between a 2 vectors
    if(sector >= SECTOR_1 && sector <= SECTOR_6){
        //get the upper and lower angles for the vector
        int upper_angle = get_upper_angle(&sector);
        int lower_angle = get_lower_angle(&sector);

        int total_percent = 100; //start with 100 and distribute it out

        //figure out how long to keep the 0/7 vector on
        percentages[2] = total_percent - magnitude;
        total_percent = total_percent - (100-magnitude);

        //multiple to deal with the cases where the 0 and 7 vector must be kept on
        float percent_multiple = (float)total_percent / 100;

        //set upper limit vector turn on time
        percentages[1] = (int)roundLocal(((float)angle / upper_angle) * 100) * percent_multiple;
        percentages[0] = total_percent - percentages[1];
    }
    else{
        //for when the angle we desire lies along one vector perfectly
        percentages[2] = 100 - magnitude; //sees how long to modulate zero
        percentages[0] = magnitude; //sees how long to modulate the solo state vector

    }
    return percentages;
}

int get_upper_angle(int* sector_ptr){
    int upper_angle = 0;
    switch(*sector_ptr){
        case SECTOR_1:
            upper_angle = 60;
            break;
        case SECTOR_2:
            upper_angle = 120;
            break;
        case SECTOR_3:
            upper_angle = 180;
            break;
        case SECTOR_4:
            upper_angle = 240;
            break;
        case SECTOR_5:
            upper_angle = 300;
            break;
        case SECTOR_6:
            upper_angle = 360;
            break;
    }
    return upper_angle;
}

int get_lower_angle(int* sector_ptr){
    int lower_angle = 0;
    switch(*sector_ptr){
        case SECTOR_1:
            lower_angle = 0;
            break;
        case SECTOR_2:
            lower_angle = 60;
            break;
        case SECTOR_3:
            lower_angle = 120;
            break;
        case SECTOR_4:
            lower_angle = 180;
            break;
        case SECTOR_5:
            lower_angle = 240;
            break;
        case SECTOR_6:
            lower_angle = 300;
            break;
    }
    return lower_angle;
}

int roundLocal (float d){
    if(d < 0.0)
        return (int) (d - .5);
    else
        return (int) (d + .5);
}
