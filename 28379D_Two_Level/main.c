/*
 * File:   main.c
 * Author: john.bumgardner
 *
 * Created on February 21, 2020
 *
 * Lots of this code is from existing projects involving timer based interrupts
 */



// Included Files
#include "driverlib.h"
#include "device.h"
#include "macros.h"
#include "functions.h"


// Globals
int control_magnitude = 90; //basically percent of maximum


// Flags controlled by interrupts
volatile unsigned int iterations_needed;

// Frequencies in Hertz
long fundamental_frequency = 500;
long switching_frequency = 50000;

// Main
void main(void)
{
    // Initialize device clock and peripherals
    Device_init();

    // Initialize GPIO and configure the GPIO pin as a push-pull output
    Device_initGPIO();
    GPIO_setPinConfig(DEVICE_GPIO_CFG_LED1);
    GPIO_setMasterCore(DEVICE_GPIO_PIN_LED1, GPIO_CORE_CPU1);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);


    //initialize the pins for motor control
    init_switches();

    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    Interrupt_initModule();

    // Initialize the PIE vector table with pointers to the shell Interrupt Service Routines (ISR).
    Interrupt_initVectorTable();

    // ISRs for each CPU Timer interrupt
    Interrupt_register(INT_TIMER0, &cpuTimer0ISR);
    Interrupt_register(INT_TIMER1, &cpuTimer1ISR);
    Interrupt_register(INT_TIMER2, &cpuTimer2ISR);

    //
    // Initializes the Device Peripheral. For this example, only initialize the
    // Cpu Timers.
    //
    initCPUTimers();

    //
    // Configure CPU-Timer 0, 1, and 2 to interrupt every second:
    // 1 second Period (in uSeconds)
    //


    //get the necessary interrupts
    int interrupt_time = get_interrupt_time(&switching_frequency);
    configCPUTimer(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ, interrupt_time);
    configCPUTimer(CPUTIMER1_BASE, DEVICE_SYSCLK_FREQ, 1000000);
    configCPUTimer(CPUTIMER2_BASE, DEVICE_SYSCLK_FREQ, 1000000);




    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    EINT;
    ERTM;

    // Perform the pre compuation
    // Generate data structure to store the vectors
    struct Node* head = NULL;
    int size = get_divisions(&switching_frequency, &fundamental_frequency);
    head = get_vector_list(&size); //get an unfilled list of the nodes we need

    // Create current node pointer
    struct Node * current;

    // Start at the head of the list at angle 0
    current = head;
    double initial_angle = 0;
    double differential_angle = round_to_tenths(get_differential_angle(&switching_frequency, &fundamental_frequency));

    // Now that all the nodes are filled, make the list circular
    struct Node* end = get_end_node(head);

    // Compute and store the percentages
    while(current->next != NULL){
        current = current->next;
        int sector = get_sector(&initial_angle); //compute the first sector

        // Put the vectors needed into a linked list
        int* array_of_vectors = get_modulated_array(&sector, &control_magnitude); // get what arrays are needed
        int i;
        for(i = 0; i < SIZEOFMODULATEDARRAY; i++){
            current->vectors[i] = array_of_vectors[i]; // store them into the list
        }

        // Put the percentages needed into the linked list
        int* percentages = get_percents_to_modulate(&array_of_vectors[0], &initial_angle, &control_magnitude); // compute the percentages
        for(i = 0; i < SIZEOFMODULATEDARRAY; i++){
            current->percentages[i] = percentages[i]; // store them into the data struct
        }

        // Go through the angles
        initial_angle = initial_angle + differential_angle;
    }
    current = head->next;
    end->next = current;

    //
    // To ensure precise timing, use write-only instructions to write to the
    // entire register. Therefore, if any of the configuration bits are changed
    // in configCPUTimer and initCPUTimers, the below settings must also
    // be updated.
    //
    CPUTimer_enableInterrupt(CPUTIMER0_BASE);
    CPUTimer_enableInterrupt(CPUTIMER1_BASE);
    CPUTimer_enableInterrupt(CPUTIMER2_BASE);

    //
    // Enables CPU int1, int13, and int14 which are connected to CPU-Timer 0,
    // CPU-Timer 1, and CPU-Timer 2 respectively.
    // Enable TINT0 in the PIE: Group 1 interrupt 7
    //
    Interrupt_enable(INT_TIMER0);
    Interrupt_enable(INT_TIMER1);
    Interrupt_enable(INT_TIMER2);


    iterations_needed = 0;

    //
    // Starts CPU-Timer 0, CPU-Timer 1, and CPU-Timer 2.
    //
    CPUTimer_startTimer(CPUTIMER0_BASE);
    CPUTimer_startTimer(CPUTIMER1_BASE);
    CPUTimer_startTimer(CPUTIMER2_BASE);

    // Load the initial vectors
    char* array_of_vectors = current -> vectors;;
    unsigned char* percentages = current -> percentages;

    initial_angle = 0;

    // Loop to control the GPIO

    while(1){
        int i = 0;

        // Catch up to the current time based on the interrupt
        // Iterations needed is a global that is altered by the CPU1 Timer in timers.c
        while(i < iterations_needed){
            // for every 20 microseconds, iterate through the circular linked list once
            current = current -> next;

            // update the vectors to modulate and their respective percentages
            array_of_vectors = current -> vectors;;
            percentages = current -> percentages;

            // increment the angle for debugging and keep onto 360 degree
            initial_angle = initial_angle + differential_angle;
            if(initial_angle > AREA_OF_CIRCLE){
                initial_angle = initial_angle - AREA_OF_CIRCLE;
            }

            // iterate
            i++;

        }
        // Reset the iterations needed
        iterations_needed = 0;

        // Pass the vectors into the control GPIO functions
        control_switches(array_of_vectors, percentages);

    }


}
