
// Included Files
#include "driverlib.h"
#include "device.h"
#include "macros.h"
#include "functions.h"


// Globals
int control_angle = 45; //location on the unit circle we want modulate to
int control_magnitude = 90; //basically percent of maximum



// Main
void main(void)
{
    // Initialize device clock and peripherals
    Device_init();

    // Initialize GPIO and configure the GPIO pin as a push-pull output
    Device_initGPIO();


    //initialize the pins for motor control
    init_switches();

    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    Interrupt_initModule();

    // Initialize the PIE vector table with pointers to the shell Interrupt Service Routines (ISR).
    Interrupt_initVectorTable();

    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    EINT;
    ERTM;

    // Compute control parameters to control motor
    int sector = get_sector(&control_angle);
    int* array_of_vectors = get_modulated_array(&sector, &control_magnitude);
    int* percentages = get_percents_to_modulate(&array_of_vectors[0], &control_angle, &control_magnitude);

    // Loop Forever
    while(1)
    {
        control_switches(array_of_vectors, percentages);
    }
}