---------------------------------README-----------------------------------

				COMPE 571 - EMBEDDED OPERATING SYSTEM 

						Programming Assignment -1



STUDENTS: MOHAMMED SHOAIB IBRAR MOHAMMED AKMAL - 828380911
		  VAIBHAV JOSHI - 828558777

THERE ARE 6 FILES IN TOTAL INCLUDING README FILE.

'base.c' 'frk.c' 'thrd.c' 'ppn.c' 'eppn.c' 'README.txt'

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

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