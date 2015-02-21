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
unsigned int EDF (Workload* workload) {
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
			//printf("[%u] LD:%u >= LE:%u\n",i, approxLastDeadline, (workload->tasks[i])->last_exec_us);
			if ( approxLastDeadline > (workload->tasks[i])->last_exec_us) {
			//if deadline is earlier, then task hasn't ran this period 
				earliest = (workload->tasks[i])->next_deadline_us;
				eT=(workload->tasks[i])->id;
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
unsigned int LST (Workload* workload) {
	unsigned int lSlack = -1; // least slack time, hopefully max unsigned int
	unsigned int lT=0; //task w/ least slack time
	unsigned int calSlack; //holds the calculated slack time for i task
	int i; //workload iterator
	for (i = 0; i < workload->task_num; i++) {
                //calc slack
		calSlack = (workload->tasks[i])->next_deadline_us - (workload->tasks[i])->last_finish_us;
                if ( calSlack < lSlack) {
			//is the task ready to run though?
			 if ( ( (workload->tasks[i])->next_deadline_us - (workload->tasks[i])->period_time_us) >=
                                (workload->tasks[i])->last_finish_us) { //hadn't executed this period
                        //if slack is less, update lSlack and the task id
                        	lSlack = calSlack;
                        	lT=(workload->tasks[i])->id;
			}
                }
        }
        return lT;
}

/**
 * \brief Takes the workload to pass to the given algorithm
 * \param workload, holds tasks and task data in current implementation to 
 * iterate through
 * \param alg, the algorithm to use for scheduling
 */
unsigned int scheduleTask( Workload *workload, SCHED_ALG alg) {
	//find which algorithm to use
	switch (alg) {
		case EARLIEST_DEADLINE:
			return EDF(workload);
		case LEAST_SLACK:
			return LST(workload);
		default:
			printf("scheduleTask:: algorithm not implemented yet\n");
			return -1;
	}
}
