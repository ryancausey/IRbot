/*
 * IRbotStates.c
 *
 * Created: 12/2/2012 2:02:10 PM
 *  Author: Nukesforbreakfast
 */ 

#include "IRbotStates.h"

void scanState()
{
	/************************************************
	* PWM Setup for the servo using PORTE, TCE0_CCA *
	************************************************/
	/*
	* Timer E0 setup for servo PWM
	*/
	TCE0_CTRLA = TC_CLKSEL_DIV64_gc; //set timer to div/64
	TCE0_CTRLB = 0x10 | TC_WGMODE_SS_gc; //turn on capture(CCAEN) and set waveform generation mode to PWM
	TCE0_CTRLC = 0x00; //turn off compares
	TCE0_CTRLD = 0x00; //turn off events
	TCE0_CTRLE = 0x00; //turn off byte mode
	TCE0_PER = 10000; //set the top of the period to 20ms
	TCE0_CCA = 350; //lower bound, upper bound should be 1150
	TCE0_INTCTRLA = 0x01; //turn on Overflow interrupt at low priority.
	/*
	* PORT E configuration
	*/
	PORTE_DIR = 0xFF; //set pin 0 to output without messing up other pins
	
	/**************************************************
	* Setup for IR receiver using pulse width capture *
	**************************************************/
	/*
	* Timer C1 configuration
	*/
	TCC1_CTRLA = TC_CLKSEL_DIV64_gc; //set clock source sysclk/64= 500KHz
	TCC1_CTRLB = 0x10 | TC_WGMODE_NORMAL_gc; //turn on capture channel A and set waveform generation mode normal
	TCC1_CTRLD = 0xC8; //set events to Pulse Width capture, no timer delay, and CCA listens to event channel 0, CCB to 1, etc... see datasheet
	TCC1_CTRLE = 0x00; //turn off byte mode
	TCC1_INTCTRLB = 0x00; //set CCA interrupt to low
	TCC1_PER = 0xFFFF; //set the top of the period to max 16-bit value
	/*
	* Port F configuration
	*/
	PORTF_DIR = 0xFF; //all pins as input
	PORTF_PIN2CTRL = 0x40; //set pin 2 to detect a rising and falling edges and invert the input to allow for pulse-width capture
	/*
	* Event System Configuration
	*/
	EVSYS_CH0MUX = EVSYS_CHMUX_PORTF_PIN2_gc; //set the event system to send events generated from PortF pin 2 to channel 0
	EVSYS_CH0CTRL = 0x00; //turn off sample filtering
	
	while(stateVar == 0)
	{
		;
	}
	
}

/*
* State for handling the acquisition of the signal and calculating the movement vector.
* Use the value of CCA to see what degree we are at.
*/
void acquireState()
{
	degreeVar = TCE0_CCA * 2; //double TCE0_CCA gives you microseconds
	
	if(degreeVar > 1500)
	{
		//we need to turn right
		degreeVar = 2400 - degreeVar; //normalize this to be between 0-900 microseconds
		degreeVar /= 10; //this will give us a value in degrees as 10 microseconds = 1 degree
	}
	else
	{
		//we need to turn left
		degreeVar = 1500 - degreeVar; //normalize this to be between 0-900 microseconds
		degreeVar /= 10; //this will guve us a value in degrees as 10 microseconds = 1 degree
	}
	
	//need to do something with this value and motors here.
}