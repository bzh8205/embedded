/*
 * ThreadMsg.h
 *
 *  Created on: Mar 16, 2015
 *      Author: Brian
 */

#ifndef THREADMSG_H_
#define THREADMSG_H_

typedef struct {
  //should it exit?
  int exit;
  //data payload
  double value;  //feed back of the plant output usually
#ifdef A_Diff_EQ
  double sp; // the user input that we want to reach
#endif
} ThreadMessage;


#endif /* THREADMSG_H_ */
