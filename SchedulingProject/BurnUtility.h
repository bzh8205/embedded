#ifndef BURNUTILITY_H_
#define BURNUTILITY_H_

#define US_10_EXE (0xFF) //TODO measure actual 10ms 

double fudge; //factor used for fudging spin

//performs a calibration procedure so spin works as expected
//across processors
void initSpinUtility();

//burns cpu time for int microseconds.
//must be multiple of 10us.
void spin( int us );

void spinTest();

#endif /* BurnUtility.h */
