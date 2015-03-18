#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/trace.h>      // to support TraceEvent calls
#include <sys/netmgr.h>
#include <sys/syspage.h>
#include <stdint.h>       /* for uintptr_t */
#include <hw/inout.h>     /* for in*() and out*() functions */
#include <sys/mman.h>     /* for mmap_device_io() */
#include "ADC.h"



void SetSingleAtoDchannel( int channelNumber )
{
    if ( channelNumber <= 15 && channelNumber >= 0 )
    {
        // configure to only use just one input channel
        out8( a_d_channel_handle, channelNumber | ( channelNumber << 4 ) ) ;
        while ( in8( a_d_input_status_handle ) & 0x20 ) ;   // wait for WAIT bit to go low
                                                            //-- means channel mux is ready to use (takes 9 microseconds)
    }
}

short MeasureVoltageOnChannel( int channelNumber )
{
    unsigned short value ;
    unsigned short lsb_value ;

    SetSingleAtoDchannel( channelNumber ) ;
    // start the capture of this channel
    out8( a_d_command_handle, 0x90 ) ;  // reset the FIFO and start the measurement
    while ( in8( a_d_input_status_handle ) & 0x80 ) ;       // wait for STS bit to go false meaning data is ready

    lsb_value = in8( a_d_LSB_handle ) ;
    value = in8( a_d_MSB_handle ) << 8 ;
    value |= lsb_value ;
    return (short)value ;
}

void SetupDIO()
{
    d_i_o_control_handle = mmap_device_io( D_I_O_PORT_LENGTH, D_I_O_CONTROL_REGISTER ) ;
    d_i_o_port_a_handle = mmap_device_io( D_I_O_PORT_LENGTH, D_I_O_PORT_A ) ;
    d_i_o_port_b_handle = mmap_device_io( D_I_O_PORT_LENGTH, D_I_O_PORT_B ) ;
}

void TestPorts()
{
    unsigned int testValue = 1 ;
    int count ;
    unsigned int portAvalue ;
    unsigned int portBvalue ;

    out8( d_i_o_control_handle, 0x02) ;     // make port B input

    // test output port A one bit at a time from low bit to high bit
    for ( count = 0 ; count < 8 ; count++, testValue <<= 1 )
    {
        out8( d_i_o_port_a_handle, testValue ) ;
        portBvalue = in8( d_i_o_port_b_handle ) ;
        if ( testValue != portBvalue )
            printf( "\nERROR on Port A Out to Port B In at bit %d -- got %u expected %u", count, portBvalue, testValue ) ;
    }

    out8( d_i_o_control_handle, 0x10) ;     // make port A input

    // test output port B one bit at a time from low bit to high bit
    testValue = 1 ;
    for ( count = 0 ; count < 8 ; count++, testValue <<= 1 )
    {
        out8( d_i_o_port_b_handle, testValue ) ;
        portAvalue = in8( d_i_o_port_a_handle ) ;
        if ( testValue != portAvalue )
            printf( "\nERROR on Port B Out to Port A In at bit %d -- got %u expected %u", count, portAvalue, testValue ) ;
    }
    printf( "\nDigital I O ports A and B testing completed\n" ) ;
}

void SetupAtoD()
{
    uintptr_t i_o_control_handle ;

    /* Get handles to the A to D registers */
    a_d_command_handle = mmap_device_io( A_D_PORT_LENGTH, A_D_COMMAND_REGISTER );
    a_d_LSB_handle = a_d_command_handle ;   // read on command port to get the A/D LSB
    a_d_MSB_handle = mmap_device_io( A_D_PORT_LENGTH, A_D_MSB_REGISTER );
    a_d_channel_handle = mmap_device_io( A_D_PORT_LENGTH, A_D_CHANNEL_REGISTER );
    a_d_input_status_handle = mmap_device_io( A_D_PORT_LENGTH, A_D_INPUT_GAIN_REGISTER );   // set to gain of 1
    i_o_control_handle = mmap_device_io( A_D_PORT_LENGTH, I_O_CONTROL_REGISTER ) ;          // only need for init

    /* Initialize the A/D converter */
    out8( a_d_command_handle, 0x7f );       // clear everything but do not start a new A/D conversion
    out8( a_d_input_status_handle, 0 );     // set to 10 volt range and clear scan mode enable
    out8( i_o_control_handle, 0 ) ;         // set  AINTE to off for polling mode for trigger via A/D command register
}

int GetRootAccess()
{
    int status = 0 ;
    int privity_err ;

    /* Give this thread root permissions to access the hardware */
    privity_err = ThreadCtl( _NTO_TCTL_IO, NULL );
    if ( privity_err == -1 )
    {
        fprintf( stderr, "can't get root permissions\n" );
        status = -1;
    }

    return status ;
}

//Sets up DAC, for analog output
void SetupAout()
{
    /* Get handles to the D to A registers */
    a_out_value_MSB_handle = mmap_device_io( A_D_PORT_LENGTH, A_OUT_MSB_REGISTER );
    a_out_value_LSB_handle = mmap_device_io( A_D_PORT_LENGTH, A_OUT_LSB_REGISTER );
}

// For generating a specific analog output at specified channel
void GenerateAout( int voltage, int output_channel )
{
    unsigned int lsb_value = 0 ;
    unsigned int msb_and_channel = 0 ;
    unsigned int msb_and_lsb = 0 ;
    double converted_voltage = ( 2048 * voltage ) / 10.0 ;  // assume output goes positive and negative

    msb_and_lsb = 2048 + (int) ( ( converted_voltage + 0.5 ) ) ;    // round by adding 1/2 and truncating.
    if ( msb_and_lsb > 4095 )
        msb_and_lsb = 4095 ;                    // clip at largest possible value.
    lsb_value = msb_and_lsb & 0xff ;            // just get the low byte
    msb_and_channel = msb_and_lsb >> 8 ;        // just get the top 4 bits
    msb_and_channel |= ( output_channel << 6 ) ;    // move channel to the top two bits.
    printf( "MSB %02x  LSB %02x   ", msb_and_channel, lsb_value ) ;
    out8( a_out_value_LSB_handle, lsb_value ) ;
    out8( a_out_value_MSB_handle, msb_and_channel ) ;
}
#ifdef TEST_ADC
int main(int argc, char *argv[])
{
    int loop ;
    int aout_channel ;
    int voltage ;
    int analog_input_channel_lookup_table[] =
            {0, 8, 1, 9} ;  // Vout 0 to Vin 0, Vout 1 to Vin 8
                            // Vout 2 to Vin 1, Vout 3 to Vin 9

    //TraceEvent( _NTO_TRACESTART ) ;
    //TraceEvent( _NTO_TRACE_INSERTUSRSTREVENT, _NTO_TRACE_USERFIRST, "start test" ) ;
    if ( ! GetRootAccess() )
    {
        SetupAtoD() ;
        SetupDIO() ;
        SetupAout() ;

        printf( "\nInstall Aout / Ain test cable and press a key: " ) ;
        getchar() ;

        for ( aout_channel = 0 ; aout_channel < 4 ; aout_channel++ )
            for ( voltage = -1 ; voltage <= 1 ; voltage++ )
            {
                GenerateAout( voltage, aout_channel ) ;
                sleep( 1 ) ;
                printf( "Voltage %3d Channel %2d A/D in: %6d\n", voltage, aout_channel,
                        MeasureVoltageOnChannel( analog_input_channel_lookup_table[ aout_channel ] ) ) ;
            }

        printf( "\nInstall Ain and DIO cable and press a key: " ) ;
        getchar() ;
        printf( "\nStarting measurement on analog input line 0\n" ) ;
        for ( loop = 0 ; loop < 16 ; loop++ )
        {
            printf( "\n%6d", MeasureVoltageOnChannel( loop ) ) ;
        }
        printf( "\n\nStarting Port A and Port B tests\n" ) ;
        TestPorts() ;
    }
    else
        printf( "\nFailure getting root access for I/O register mapping\n") ;

    //TraceEvent( _NTO_TRACE_INSERTUSRSTREVENT, _NTO_TRACE_USERFIRST, "stop test" ) ;
    //TraceEvent( _NTO_TRACESTOP ) ;
    return EXIT_SUCCESS;
}

#endif
