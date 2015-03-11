#ifndef _ADC_H_
#define _ADC_H_
/**
 * \file: ADC.h
 * \brief ADC and DAC interface
 * \author  Larry Kiser
 * \modifided Alyssa Colyette
 */
#include <stdint.h>       /* for uintptr_t */

#define A_D_PORT_LENGTH (1)
#define A_D_BASE_ADDRESS (0x280)
#define A_D_COMMAND_REGISTER (A_D_BASE_ADDRESS)
#define A_D_MSB_REGISTER (A_D_BASE_ADDRESS + 1)
#define A_D_CHANNEL_REGISTER (A_D_BASE_ADDRESS + 2)
#define A_D_INPUT_GAIN_REGISTER (A_D_BASE_ADDRESS + 3)
#define A_D_INPUT_STATUS_REGISTER (A_D_INPUT_GAIN_REGISTER)
#define I_O_CONTROL_REGISTER (A_D_BASE_ADDRESS + 4)

#define D_I_O_PORT_LENGTH (1)
#define D_I_O_CONTROL_REGISTER (A_D_BASE_ADDRESS + 0x0b)
#define D_I_O_PORT_A (A_D_BASE_ADDRESS + 0x08)
#define D_I_O_PORT_B (A_D_BASE_ADDRESS + 0x09)

// make static as good programming practice (not in global symbol table)
static uintptr_t a_d_command_handle ;
static uintptr_t a_d_LSB_handle ;
static uintptr_t a_d_MSB_handle ;
static uintptr_t a_d_channel_handle ;
static uintptr_t a_d_input_status_handle ;  // also used once for analog input gain setting

static uintptr_t d_i_o_control_handle ;     // control register for ports A, B, and C
static uintptr_t d_i_o_port_a_handle ;
static uintptr_t d_i_o_port_b_handle ;

static void SetSingleAtoDchannel( int channelNumber );

static short MeasureVoltageOnChannel( int channelNumber );

//not really needed
static void SetupDIO();
static void TestPorts();

//set up the A to D
static void SetupAtoD();

// need root access to memory map device modules
int GetRootAccess();


#endif //_ADC_H
