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
#define A_OUT_LSB_REGISTER (A_D_BASE_ADDRESS + 6)
#define A_OUT_MSB_REGISTER (A_D_BASE_ADDRESS + 7)

#define D_I_O_PORT_LENGTH (1)
#define D_I_O_CONTROL_REGISTER (A_D_BASE_ADDRESS + 0x0b)
#define D_I_O_PORT_A (A_D_BASE_ADDRESS + 0x08)
#define D_I_O_PORT_B (A_D_BASE_ADDRESS + 0x09)

// make static as good programming practice (not in global symbol table)
uintptr_t a_d_command_handle ;
uintptr_t a_d_LSB_handle ;
uintptr_t a_d_MSB_handle ;
uintptr_t a_d_channel_handle ;
uintptr_t a_d_input_status_handle ;  // also used once for analog input gain setting

uintptr_t d_i_o_control_handle ;     // control register for ports A, B, and C
uintptr_t d_i_o_port_a_handle ;
uintptr_t d_i_o_port_b_handle ;

// analog output register handles
uintptr_t a_out_value_MSB_handle ;   // bits 0 - 3 are 4 MSB bits. Bits 6 & 7 are channel
uintptr_t a_out_value_LSB_handle ;   // write before write MSB

void SetSingleAtoDchannel( int channelNumber );

short MeasureVoltageOnChannel( int channelNumber );

//not really needed
void SetupDIO();
void TestPorts();

//set up the A to D
void SetupAtoD();

// need root access to memory map device modules
int GetRootAccess();

//Sets up DAC, for analog output
void SetupAout();

// For generating a specific analog output at specified channel
void GenerateAout( int voltage, int output_channel );

#endif //_ADC_H
