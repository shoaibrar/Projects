---------------------------------README-----------------------------------

This is a coursework project i did Durign my masters

--------------------------------------------------------------------------

THERE ARE 2 FILES IN TOTAL INCLUDING README FILE.

'main.py' 'README.txt'

////////////////////////////////////////////////////////////////////////////
This project simulates the working of a virtual memory .
it is executed in an Linux environment.

data1.txt and data2.txt file contain memory location and the write and read operations.

main.py file creates the structure of a virtual memory i.e pages and performs the read and write operations
Page replacement techniques such as random , least recently used(lru), First in first out (FIFO0) and Periodic reference reset(per).
The page faults, memory swapped and other details are observedd.

							HOW TO RUN

Used Python(python3) in an linux based system(ubuntu)

To tun the scheduling algorithm

	The format:	>> python3 main.py [input_file] [algorithm]
	
	input_file are : 'data1' or 'data2'
	algorithm : 'rand' or 'fifo' or 'lru' or 'per'
	

-------------------------------------THE END-------------------------------------------
