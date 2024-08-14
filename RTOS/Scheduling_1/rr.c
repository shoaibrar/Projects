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

#define QUANTUM1 38000
#define QUANTUM2 38000
#define QUANTUM3 38000
#define QUANTUM4 38000



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



int main(int argc, char const *argv[])
{
	pid_t pid1, pid2, pid3, pid4;
	int running1, running2, running3, running4;

	clock_t start, pt1, pt2, pt3, pt4,bpt1, bpt2, bpt3, bpt4 ;		//INITIALIZATION OF VARIABLE

	int f1[2], f2[2], f3[3], f4[2];

	long double rt1 =0.0, rt2 =0.0, rt3 =0.0, rt4 =0.0;
	long double avg_t;

	pipe(f1);
	pipe(f2);
	pipe(f3);
	pipe(f4);

	pid1 = fork();

	if (pid1 == 0){

		close(f1[0]);

		
	
		myfunction(WORKLOAD1);
		
		pt1 = clock();
	
		write(f1[1], &pt1, sizeof(pt1));
		exit(0);
	}
	kill(pid1, SIGSTOP);

	pid2 = fork();

	if (pid2 == 0){

		close(f2[0]);

		
		
		myfunction(WORKLOAD2);
		
		pt2 = clock();

		write(f2[1], &pt2, sizeof(pt2));
		exit(0);
	}
	kill(pid2, SIGSTOP);

	pid3 = fork();

	if (pid3 == 0){

		close(f3[0]);

		
		
		myfunction(WORKLOAD3);						// getting the cpu time for each child process when it finishes
		
		pt3 = clock();
		write(f3[1], &pt3, sizeof(pt3));
		exit(0);
	}
	kill(pid3, SIGSTOP);

	pid4 = fork();

	if (pid4 == 0){

		close(f4[0]);

		
		
		myfunction(WORKLOAD4);
		
		pt4 = clock();

		write(f4[1], &pt4, sizeof(pt4));
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

	running1 = 1;
	running2 = 1;
	running3 = 1;
	running4 = 1;
	long double ctx_time = 0.0;
	int runt1 =0,runt2 =0,runt3 =0,runt4 =0,ctx =0;

	start = clock();

	clock_t t1,t2,t3,t4,t5,t6,t7,t8 =start;


	while (running1 > 0 || running2 > 0 || running3 > 0 || running4 > 0)
	{
		if (running1 > 0){

			t1 = clock();
			
			kill(pid1, SIGCONT);
			usleep(QUANTUM1);
			kill(pid1, SIGSTOP);
			
			runt1 += 1;
			ctx +=1;
			t2 = clock();								//RR execution
		}
		if (running2 > 0){
			t3 = clock();

			kill(pid2, SIGCONT);
			usleep(QUANTUM2);
			kill(pid2, SIGSTOP);
			
				runt2 += 1;
				ctx +=1;
				t4 = clock();						//calculating the time difference between each process
		}											// to find the context overhead
		if (running3 > 0){
			t5 = clock();
			kill(pid3, SIGCONT);
			usleep(QUANTUM3);
			kill(pid3, SIGSTOP);
			
				runt3 += 1;
				ctx +=1;
				t6 = clock();
		}
		if (running4 > 0){
			t7 = clock();
			kill(pid4, SIGCONT);
			usleep(QUANTUM4);
			kill(pid4, SIGSTOP);
			waitpid(pid3, &running3, WNOHANG);
				runt4 += 1;
				ctx +=1;
				t8 = clock();
		}
		ctx_time = ctx_time + (long double)((t2-t3)+(t4-t5)+(t6-t7)+(t8-t2))/(CLOCKS_PER_SEC);
		waitpid(pid1, &running1, WNOHANG);
		waitpid(pid2, &running2, WNOHANG);
		waitpid(pid3, &running3, WNOHANG);
		waitpid(pid4, &running4, WNOHANG);

	}

	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/
	close(f1[1]);
	close(f2[1]);
	close(f3[1]);
	close(f4[1]);

	read(f1[0], &pt1, sizeof(pt1));
	read(f2[0], &pt2, sizeof(pt2));
	read(f3[0], &pt3, sizeof(pt3));
	read(f4[0], &pt4, sizeof(pt4));



	rt1 += (long double) ((pt1 - start))/(CLOCKS_PER_SEC);
	rt2 += (long double) ((pt2 - start))/(CLOCKS_PER_SEC);
	rt3 += (long double) ((pt3 - start))/(CLOCKS_PER_SEC);
	rt4 += (long double) ((pt4 - start))/(CLOCKS_PER_SEC);		//calculating response time and context overhead

	avg_t = (rt1 + rt2 + rt3 + rt4);
	

	printf("response time 1 = %Lf sec\n", rt1);
	printf("response time 2 = %Lf sec\n", rt2);
	printf("response time 3 = %Lf sec\n", rt3);
	printf("response time 4 = %Lf sec\n", rt4);
	printf("total response time = %Lf sec\n\n", avg_t);
	printf("avg response time = %Lf sec\n\n", avg_t/4);


	
	printf(" context switch For P1 : %d\n", runt1);
	printf(" context switch For P2: %d\n", runt2);
	printf(" context switch For P3 : %d\n", runt3);
	printf(" context switch For P4 : %d\n", runt4);

	printf("Total # context switch has been done : %d\n", ctx);

	printf("TOTAL context switch time/OVERHEAD %Lf \n",ctx_time );
	printf("AVg context switch time/OVERHEAD %Lf \n",ctx_time/(ctx) );



	return 0;
}
