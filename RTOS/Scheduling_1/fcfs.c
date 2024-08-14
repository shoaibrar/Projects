#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/wait.h> 
#include <string.h> 
#include <time.h> 
#include <signal.h>
#include <sys/time.h>

/************************************************************************************************ 
		These DEFINE statements represent the workload size of each task and 
		the time quantum values for Round Robin scheduling for each task.
*************************************************************************************************/

#define WORKLOAD1 100000
#define WORKLOAD2 100000
#define WORKLOAD3 100000
#define WORKLOAD4 100000

#define QUANTUM1 1000
#define QUANTUM2 1000
#define QUANTUM3 1000
#define QUANTUM4 1000



/************************************************************************************************ 
					DO NOT CHANGE THE FUNCTION IMPLEMENTATION
*************************************************************************************************/
void myfunction(int param){

	int i = 2;
	int j, k;

	while(i < param){
		k = i; 
		for (j = 2; j <= k; j++)
		{
			if (k % j == 0){
				k = k/j;
				j--;
				if (k == 1){
					break;
				}
			}
		}
		i++;
	}
}
/************************************************************************************************/

clock_t start, pt1, pt2, pt3, pt4;

int main(int argc, char const *argv[])
{
	pid_t pid1, pid2, pid3, pid4;
	int running;
	int fd1[2], fd2[2], fd3[3], fd4[2];						//INITIALIZATION OF VARIABLE
	int  pid_order[4], i;
	long double rt1 =0.0, rt2 =0.0, rt3 =0.0, rt4 =0.0, avg_t;

	pipe(fd1);
	pipe(fd2);
	pipe(fd3);
	pipe(fd4);

	pid1 = fork();

	if (pid1 == 0){

		close(fd1[0]);

		myfunction(WORKLOAD1);

		pt1 = clock();

		write(fd1[1], &pt1, sizeof(pt1));					// calculating the cpu time for each child process
															// and sendign the value through pipe
		exit(0);
	}
	kill(pid1, SIGSTOP);

	pid2 = fork();

	if (pid2 == 0){

		close(fd2[0]);

		myfunction(WORKLOAD2);

		pt2 = clock();
		write(fd2[1], &pt2, sizeof(pt2));

		exit(0);
	}
	kill(pid2, SIGSTOP);

	pid3 = fork();

	if (pid3 == 0){

		close(fd3[0]);

		myfunction(WORKLOAD3);

		pt3 = clock();
		write(fd3[1], &pt3, sizeof(pt3));

		exit(0);
	}
	kill(pid3, SIGSTOP);

	pid4 = fork();

	if (pid4 == 0){

		close(fd4[0]);

		myfunction(WORKLOAD4);

		pt4 = clock();
		write(fd4[1], &pt4, sizeof(pt4));

		exit(0);
	}
	kill(pid4, SIGSTOP);

	/************************************************************************************************ 
		At this point, all  newly-created child processes are stopped, and ready for scheduling.
	*************************************************************************************************/



	/************************************************************************************************
		- Scheduling code starts here
		- Below is a sample schedule. (which scheduling algorithm is this?)
		- For the assignment purposes, you have to replace this part with the other scheduling methods 
		to be implemented.
	************************************************************************************************/

	running = 1;
	int ctx = 0;
	pid_order[0] = 1;
	pid_order[1] = 2;
	pid_order[2] = 3;
	pid_order[3] = 4;

	long double ctx_time = 0.0;
	clock_t t1,t2;

	
	start = clock();
	t2 =start;

	for (i = 0; i < 4; i++) {
		if (pid_order[i] == 1){
			t1 = clock();
			ctx_time = ctx_time + (long double)((t1-t2))/(CLOCKS_PER_SEC);
			kill(pid1, SIGCONT);											//executing the process in order for FCFS

			while(running){
				waitpid(pid1, &running, WNOHANG);
			}
			t2 = clock();
			running = 1;
			ctx +=1;
		}
		else if (pid_order[i] == 2){
			t1 = clock();
			ctx_time = ctx_time + (long double)((t1-t2))/(CLOCKS_PER_SEC);
			kill(pid2, SIGCONT);

			while(running){
				waitpid(pid2, &running, WNOHANG);
			}
			t2 = clock();
			running = 1;
			ctx +=1;
		}
		else if (pid_order[i] == 3){
			t1 = clock();
			ctx_time = ctx_time + (long double)((t1-t2))/(CLOCKS_PER_SEC);
			kill(pid3, SIGCONT);
			ctx +=1;

			while(running){
				waitpid(pid3, &running, WNOHANG);
			}
			t2 = clock();
			running = 1;
		}
		else if (pid_order[i] == 4){
			t1 = clock();
			ctx_time = ctx_time + (long double)((t1-t2))/(CLOCKS_PER_SEC);
			kill(pid4, SIGCONT);

			while(running){
				waitpid(pid4, &running, WNOHANG);
			}
			t2 = clock();
			running = 1;
			ctx +=1;
		}
	}

	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/
	close(fd1[1]);
	close(fd2[1]);
	close(fd3[1]);
	close(fd4[1]);

	read(fd1[0], &pt1, sizeof(pt1));
	read(fd2[0], &pt2, sizeof(pt2));
	read(fd3[0], &pt3, sizeof(pt3));
	read(fd4[0], &pt4, sizeof(pt4));

	rt1 += (long double) ((pt1 - start))/(CLOCKS_PER_SEC);
	rt2 += (long double) ((pt2 - start))/(CLOCKS_PER_SEC);
	rt3 += (long double) ((pt3 - start))/(CLOCKS_PER_SEC);
	rt4 += (long double) ((pt4 - start))/(CLOCKS_PER_SEC);		//calculating response time and context overhead

	avg_t = (rt1 + rt2 + rt3 + rt4);
	

	printf("response time 1 = %Lf sec\n", rt1);
	printf("response time 2 = %Lf sec\n", rt2);
	printf("response time 3 = %Lf sec\n", rt3);
	printf("response time 4 = %Lf sec\n", rt4);
	printf("total response time = %Lf sec\n", avg_t);
	printf("avg response time = %Lf sec\n\n", avg_t/4);

	printf("Total # context switch has been done : %d\n", ctx);

	printf("TOTAL context switch time/OVERHEAD %Lf \n",ctx_time );
	printf("AVg context switch time/OVERHEAD %Lf \n",ctx_time/ctx );

	return 0;
}
