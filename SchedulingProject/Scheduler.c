/**
 * Holds the schedulers implemented
 */
#include "Project1.h"

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
	int i; //workload iterator
	//find all next deadlines, assuming first thread indexed 0
	for (i = 0; i < workload->task_num; i++) {
		//compare deadline to earliest
		if ( (workload->tasks[i]).next_deadline_us < earliest) {
			//if deadline is earlier, update earliest and eT
			earliest = (workload->tasks[i]).next_deadline_us;
			eT=(workload->tasks[i]).id;
		}
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
		calSlack = (workload->tasks[i]).next_deadline_us - (workload->tasks[i]).last_finish_us;
                if ( calSlack < lSlack) {
                        //if slack is less, update lSlack and the task id
                        lSlack = calSlack;
                        lT=(workload->tasks[i]).id;
                }
        }
        return lT;
}
