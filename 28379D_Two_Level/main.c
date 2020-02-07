
// Included Files
#include "driverlib.h"
#include "device.h"
#include "macros.h"
#include "functions.h"


// Globals
int control_angle; //location on the unit circle we want modulate to
int control_magnitude = 90; //basically percent of maximum
int differential_angle = 10;

volatile unsigned int iterate_flag;


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
    configCPUTimer(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ, 55); // every 55 micro seconds trigger the interrupt, based on a fundamental frequency of 500 Hz
    //configCPUTimer(CPUTIMER1_BASE, DEVICE_SYSCLK_FREQ, 1000000);
    //configCPUTimer(CPUTIMER2_BASE, DEVICE_SYSCLK_FREQ, 1000000);

    //
    // To ensure precise timing, use write-only instructions to write to the
    // entire register. Therefore, if any of the configuration bits are changed
    // in configCPUTimer and initCPUTimers, the below settings must also
    // be updated.
    //
    CPUTimer_enableInterrupt(CPUTIMER0_BASE);
    // CPUTimer_enableInterrupt(CPUTIMER1_BASE);
    // CPUTimer_enableInterrupt(CPUTIMER2_BASE);

    //
    // Enables CPU int1, int13, and int14 which are connected to CPU-Timer 0,
    // CPU-Timer 1, and CPU-Timer 2 respectively.
    // Enable TINT0 in the PIE: Group 1 interrupt 7
    //
    Interrupt_enable(INT_TIMER0);
    //Interrupt_enable(INT_TIMER1);
    //Interrupt_enable(INT_TIMER2);


    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    EINT;
    ERTM;

    // Perform the pre compuation
    // Generate data structure to store the vectors
    struct Node* head = NULL;
    int size = SECTIONS;
    head = get_vector_list(&size); //get an unfilled list of the nodes we need

    // Create current node pointer
    struct Node * current;

    // Start at the head of the list at angle 0
    current = head;
    int initial_angle = 0;

    // Compute and store the percentages
    while(current->next != NULL){
        current = current->next;
        int sector = get_sector(&initial_angle); //compute the first sector
        current->sector = sector;
        int* array_of_vectors = get_modulated_array(&sector, &control_magnitude); // get what arrays are needed
        int i;
        for(i = 0; i < 4; i++){
            current->vectors[i] = array_of_vectors[i]; // store them into the list
        }
        int* percentages = get_percents_to_modulate(&array_of_vectors[0], &initial_angle, &control_magnitude); // compute the percentages
        for(i = 0; i < 4; i++){
            current->percentages[i] = percentages[i]; // store them into the data struct
        }
        initial_angle = initial_angle + differential_angle;

    }

    // Now that all the nodes are filled, make the list circular
    struct Node* end = get_end_node(head);
    end->next = head;

    current = head;



    while(1){
        if(iterate_flag == HIGH){
            current = current -> next;
            iterate_flag = LOW;
        }
        int* array_of_vectors = current -> vectors;;
        int* percentages = current -> percentages;
        control_switches(array_of_vectors, percentages);

    }


}
