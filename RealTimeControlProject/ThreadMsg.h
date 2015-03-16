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
  int value;
} ThreadMessage;


#endif /* THREADMSG_H_ */
