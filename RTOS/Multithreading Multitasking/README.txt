---------------------------------README-----------------------------------

This is a Coursework project I did durig my Masters
--------------------------------------------------------------------------

THERE ARE 6 FILES IN TOTAL INCLUDING README FILE.

'base.c' 'frk.c' 'thrd.c' 'ppn.c' 'eppn.c' 'README.txt'

////////////////////////////////////////////////////////////////////////////
This project simulates Multitasking and Multithreading and observed its performance.
it is executed in an Linux environment.

base.c file does a simple task of incrementing sum by 1 till Limit is reached

thrd.c file does the same task but uses multiple threads

frk.c files uses multitaksing to do the task. It creates taks by using fork

eppn.c and ppn.c file does multitasking using popen method.




							HOW TO RUN

CASE 1:

	For Baseline case, compile
										"gcc base.c"
	execute the file with the arguments " ./a.out N "
	N: upper limit for sum

CASE 2:
	For Multithreading case, compile
										"gcc thrd.c"
	execute the file with arguments     "./a.out NUM_THREAD N"
	
	NUM_THREAD: # of threads wanted
	N: upper limit for sum


CASE 3:
	For MultiTasking case using FORK, compile	
												"gcc frk.c"
	execute the file with arguments     		"./a.out NUM_TASK N"
	
	NUM_TASK: # of processes wanted
	N: upper limit for sum

	For MultiTasking case using POPEN, compile
												"gcc eppn.c -o test"
								then compile
												"gcc ppn.c"
	execute the file with arguments     		"./a.out NUM_TASK N"
	
	NUM_TASK: # of processes wanted
	N: upper limit for sum


-------------------------------------THE END-------------------------------------------
