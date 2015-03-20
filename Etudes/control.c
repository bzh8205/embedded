/**
 * \brief implementing the transfer function in z-transform etudes
 * \author Alyssa Colyette
 */

#include <stdio.h>

#define SAMPLE_SIZE 103

int main () {
    int k =-2;
    int curIndex;
    //alpha beta and lamba will be calculated by user input for PID tuning
    double alpha = 0.368;
    double beta = 0.264;
    double lambda = 0.632;
    double lastY[2]; //from k to k-1
    double lastU[4]; //from k to k-3
    double y_k,y_km1 =0;
    double u_k,u_km1=0;//,u_km2,u_km3=0;
    double y_kp1;
    double yArray[SAMPLE_SIZE],uArray[SAMPLE_SIZE], kArray[SAMPLE_SIZE];
    //init arrays
    yArray[0]=uArray[0] = 0;
    yArray[1] = uArray[1] = 0;
    kArray[0] = -2;
    kArray[1] = -1;
    //with a step input of 5 means u=5 always?
    for (k = 0; k < SAMPLE_SIZE -2; k++) { //for 103 samples
        //TODO sample periodically, make separate thread
        curIndex = k+2;
        y_kp1 = yArray[curIndex-1] -lambda*yArray[curIndex-2]+alpha*uArray[curIndex-1] + beta*uArray[curIndex-2];
        //save u_k, k, and y_kp1
        //printf("indexing %d for %d\n",k+2,k);
        yArray[curIndex]=y_kp1;
        uArray[curIndex]= 5; //TODO read via analog in
        kArray[curIndex] = k;
    }
    
    for (k=-2; k < SAMPLE_SIZE-2; k++){
        printf("k:%d\tu:%f\ty:%f\n",(int) kArray[ k+2],uArray[k+2],yArray[k+2] );
    } 
    return 1;
}

