/**
 * Holds the schedulers implemented
 */
#include <stdio.h>
#include "Project1.h"
#include "Scheduler.h"

//TODO actually test these schedulers

/**
 * \brief Takes a workload with the number of tasks loaded to
 * present the task id with the earliest deadline first
 * \param workload, holds tasks and task data in current implementation to 
 * iterate through
 */
unsigned int EDF (Workload* workload, clock_t curTime) {
	unsigned int earliest=-1; //earliest deadline, hopefully max unsigned int
	unsigned int eT=0; //thread w/ earliest deadlin
	unsigned int approxLastDeadline;
	int i; //workload iterator
	//find all next deadlines, assuming first thread indexed 0
	for (i = 0; i < workload->task_num; i++) {
		//compare deadline to earliest
		if ( (workload->tasks[i])->next_deadline_us < earliest) {
			//check if task is ready
			approxLastDeadline= (workload->tasks[i])->next_deadline_us - (workload->tasks[i])->period_time_us;
		//	printf("[%u] LD:%u >= LE:%u\n",i, approxLastDeadline, (workload->tasks[i])->last_exec_us);
			if ( approxLastDeadline >= (workload->tasks[i])->last_exec_us) { //TODO account for on deadline exe
			//if deadline is earlier, then task hasn't ran this period 
				if (curTime > approxLastDeadline) { //period of execution isn't now
					earliest = (workload->tasks[i])->next_deadline_us;
					eT=(workload->tasks[i])->id;
				} else {//period of execution isn't now
		//			printf("[%d] not ready for this period\n",i);
				}
			} // hadn't finished w/i the last period
			else{ //not read
				//printf("[%d] E, NR\n",i);
			}
		}
	}
	//check for no ready tasks
	if (earliest ==-1) {
		return -1;
	}
	return eT;
}

/**
 * \brief Takes a workload with the number of taskes loaded to
 * present the task id with the least slack time first
 * \param workload, holds tasks and task data in current implementation to 
 * iterate through
 */
unsigned int LST (Workload* workload, clock_t curTime) {
	unsigned int lSlack = -1; // least slack time, hopefully max unsigned int
	unsigned int lT=0; //task w/ least slack time
	unsigned int calSlack; //holds the calculated slack time for i task
	unsigned int approxLastDeadline;
	int i; //workload iterator
	for (i = 0; i < workload->task_num; i++) {
                //calc slack
		//calSlack = (workload->tasks[i])->next_deadline_us - (workload->tasks[i])->last_finish_us;
                //TODO pass in current time???
		approxLastDeadline= (workload->tasks[i])->next_deadline_us - (workload->tasks[i])->period_time_us;
		if ( approxLastDeadline >= (workload->tasks[i])->last_finish_us) { //hadn't executed this period
//printf("[%d]slack: %u, ND:%u LE:%u\n",i,calSlack,(workload->tasks[i])->next_deadline_us ,(workload->tasks[i])->last_finish_us);
			//is the task ready to run though?
			calSlack = ((clock_t)( workload->tasks[i])->next_deadline_us -curTime) -
					(clock_t) (workload->tasks[i])->exec_time_us;
                //printf("[%d] slack:(%lu-%lu)-%lu=%u\n",i, ( (clock_t)( workload->tasks[i])->next_deadline_us),curTime,
		//	(clock_t) (workload->tasks[i])->exec_time_us, calSlack  );
                	if ( calSlack < lSlack) {
                        //if slack is less, update lSlack and the task id
				if (curTime > approxLastDeadline) {
                        		lSlack = calSlack;
                        		lT=(workload->tasks[i])->id;
				} else {
		//			printf("[%d] not ready this period\n",i);
				}
			}
                }
        }
        return lT;
}

/**
 * \brief Takes a workload with the number of tasks loaded to
 * present the task id with the earliest deadline first
 * \param workload, holds tasks and task data in current implementation to
 * iterate through
 */
unsigned int RMS (Workload* workload,clock_t curTime) {
        unsigned int shortest=-1; //shortest period, hopefully max unsigned int
        unsigned int sT=0; //thread w/ earliest deadlin
        unsigned int approxLastDeadline;
        int i; //workload iterator
        //find all periods, assuming first thread indexed 0
        for (i = 0; i < workload->task_num; i++) {
                //compare deadline to earliest
                if ( (workload->tasks[i])->period_time_us < shortest) {
                        //check if task is ready
//printf("[%d] updated earliest\n",i);
                        approxLastDeadline= (workload->tasks[i])->next_deadline_us - (workload->tasks[i])->period_time_us;
                        if ( approxLastDeadline >= (workload->tasks[i])->last_exec_us) {
				if(curTime > approxLastDeadline) { //if we are beyond the point of last deadline
//printf("[%d] is ready. LD:%u LE:%u\n",i,approxLastDeadline,(workload->tasks[i])->last_exec_us);
                        	//if deadline is earlier, then task hasn't ran this period
                                shortest = (workload->tasks[i])->period_time_us;
                                sT=(workload->tasks[i])->id;
				}
				else {
					//printf("[%d] not this period\n",i);
				}
                        } // hadn't finished w/i the last period
                        else{ //not read
                                //printf("[%d] E, NR\n",i);
                        }
                }
        }
        //check for no ready tasks
        if (shortest ==-1) {
                return -1;
        }
        return sT;
}


/**
 * \brief Takes the workload to pass to the given algorithm
 * \param workload, holds tasks and task data in current implementation to 
 * iterate through
 * \param alg, the algorithm to use for scheduling
 */
unsigned int scheduleTask( Workload *workload, SCHED_ALG alg, clock_t curTime) {
	//find which algorithm to use
	switch (alg) {
		case EARLIEST_DEADLINE:
			return EDF(workload, curTime);
		case LEAST_SLACK:
			return LST(workload, curTime);
		case RATE_MONOTONIC:
                        return RMS(workload,curTime);
		default:
			printf("scheduleTask:: algorithm not implemented yet\n");
			return -1;
	}
}
