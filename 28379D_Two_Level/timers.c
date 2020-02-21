/*
 * timers.c
 *
 *  Created on: Feb 5, 2020
 *      Author: john.bumgardner
 */

// Include statements

#include "functions.h"
#include "macros.h"
#include "driverlib.h"
#include "device.h"


// Globals
uint16_t cpuTimer0IntCount;
uint16_t cpuTimer1IntCount;
uint16_t cpuTimer2IntCount;

extern volatile unsigned int iterations_needed;


//
// initCPUTimers - This function initializes all three CPU timers
// to a known state.
//
void initCPUTimers(void)
{
    //
    // Initialize timer period to maximum
    //
    CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFF);
    CPUTimer_setPeriod(CPUTIMER1_BASE, 0xFFFFFFFF);
    CPUTimer_setPeriod(CPUTIMER2_BASE, 0xFFFFFFFF);

    //
    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
    //
    CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);
    CPUTimer_setPreScaler(CPUTIMER1_BASE, 0);
    CPUTimer_setPreScaler(CPUTIMER2_BASE, 0);

    //
    // Make sure timer is stopped
    //
    CPUTimer_stopTimer(CPUTIMER0_BASE);
    CPUTimer_stopTimer(CPUTIMER1_BASE);
    CPUTimer_stopTimer(CPUTIMER2_BASE);

    //
    // Reload all counter register with period value
    //
    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);
    CPUTimer_reloadTimerCounter(CPUTIMER1_BASE);
    CPUTimer_reloadTimerCounter(CPUTIMER2_BASE);

    //
    // Reset interrupt counter
    //
    cpuTimer0IntCount = 0;
    cpuTimer1IntCount = 0;
    cpuTimer2IntCount = 0;
}


//
// configCPUTimer - This function initializes the selected timer to the
// period specified by the "freq" and "period" parameters. The "freq" is
// entered as Hz and the period in uSeconds. The timer is held in the stopped
// state after configuration.
//
void configCPUTimer(uint32_t cpuTimer, float freq, float period)
{
    uint32_t temp;

    //
    // Initialize timer period:
    //
    temp = (uint32_t)(freq / 1000000 * period);
    CPUTimer_setPeriod(cpuTimer, temp);

    //
    // Set pre-scale counter to divide by 1 (SYSCLKOUT):
    //
    CPUTimer_setPreScaler(cpuTimer, 0);

    //
    // Initializes timer control register. The timer is stopped, reloaded,
    // free run disabled, and interrupt enabled.
    // Additionally, the free and soft bits are set
    //
    CPUTimer_stopTimer(cpuTimer);
    CPUTimer_reloadTimerCounter(cpuTimer);
    CPUTimer_setEmulationMode(cpuTimer,
                              CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    CPUTimer_enableInterrupt(cpuTimer);

    //
    // Resets interrupt counters for the three cpuTimers
    //
    if (cpuTimer == CPUTIMER0_BASE)
    {
        cpuTimer0IntCount = 0;
    }
    else if(cpuTimer == CPUTIMER1_BASE)
    {
        cpuTimer1IntCount = 0;
    }
    else if(cpuTimer == CPUTIMER2_BASE)
    {
        cpuTimer2IntCount = 0;
    }
}

//
// cpuTimer0ISR - Counter for CpuTimer0
//

/*
 * Interrupt that will be triggered every 55 microsecond.
 * Advances the position in the linked list to go to the next modulation position
 */
__interrupt void cpuTimer0ISR(void)
{
    cpuTimer0IntCount++;
    iterations_needed++;
    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

//
// cpuTimer1ISR - Counter for CpuTimer1
//
__interrupt void cpuTimer1ISR(void)
{
    //
    // The CPU acknowledges the interrupt.
    //
    cpuTimer1IntCount++;
}

//
// cpuTimer2ISR - Counter for CpuTimer2
//
__interrupt void cpuTimer2ISR(void)
{
    //
    // The CPU acknowledges the interrupt.
    //
    cpuTimer2IntCount++;
}

int get_interrupt_time(long* switching){
    unsigned int freq = *switching;
    return (double)1 / freq * 1000000;
}


int get_divisions(long* switching, long* fundamental){
    unsigned int interrupt_time = get_interrupt_time(switching);
    unsigned int fundamental_time = (int)((double)1 / (*fundamental) * 1000000);
    return (fundamental_time / interrupt_time) + 1;
}

double get_differential_angle(long* switching, long* fundamental){
    int divs = get_divisions(switching, fundamental);
    return AREA_OF_CIRCLE / (double)divs;
}
