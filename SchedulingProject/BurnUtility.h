#ifndef BURNUTILITY_H_
#define BURNUTILITY_H_

//performs a calibration procedure so spin works as expected
//across processors
extern void initSpinUtility();

//burns cpu time for int microseconds.
//must be multiple of 10us.
extern void spin( int us );

#endif /* BurnUtility.h */