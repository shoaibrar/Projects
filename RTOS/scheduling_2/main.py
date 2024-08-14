import sys

def test():
	if len(sys.argv) <= 2:
		print('ERROR: Not enough Parameters')  ##Checking arguments passed length
		prnt('EXITING....')
		sys.exit()

	if sys.argv[2] == 'EDF':
		ftp = 'EDF_out_'+sys.argv[1]
		if (len(sys.argv) >3)and(sys.argv[3] == 'EE') :
			ftp = 'EDF_EE_out_'+sys.argv[1]
	elif sys.argv[2] == 'RM':
		ftp = 'RM_out_'+sys.argv[1]
		if (len(sys.argv) >3)and(sys.argv[3] == 'EE') :
			ftp = 'RM_EE_out_'+sys.argv[1]

	sys.stdout = open(ftp,'wt')
	

	testsite_array = []

	Task_data=[]
	Sys_data=[]
	my_file = open(sys.argv[1],'r')
	testsite_array = my_file.readlines()
	my_file.close()

	for line in testsite_array:	## opening file and taking data
		line =line.split()
		if len(line)==7:
			Sys_data = line
		elif len(line)==6:
			Task_data.append(line)


	period=[]
	req_exec=[]
	name=[]
	energy=[]
	frequencies=[1188,918,648,384]

	f_1=[]
	f_2=[]
	f_3=[]
	f_4=[]
	f_5=[]
	idle_pwr=0
	for i in range(0,int(Sys_data[0])):
		req_exec.append(int(Task_data[i][2]))
		name.append(Task_data[i][0])
		period.append(int(Task_data[i][1]))
	for i in range(2,len(Task_data[1])):
		f_1.append(int(Task_data[0][i]))		#adding data to certain variables
		f_2.append(int(Task_data[1][i]))
		f_3.append(int(Task_data[2][i]))
		f_4.append(int(Task_data[3][i]))
		f_5.append(int(Task_data[4][i]))
	idle_pwr = float(Sys_data[-1])/1000


	for i in range(2,len(Sys_data)):
		energy.append(float(Sys_data[i])/1000)


	pos =0
	eq=0
	if sys.argv[2] == 'EDF':
		print('\t EDF ALGORITHM')
		eq= 1
		pos = 4
	elif sys.argv[2] == 'RM':
		print('\t RM ALGORITHM')			#getting inequality for RM and EDF 
		y=0
		y=float(1)/float(Sys_data[0])
	
		eq = float(Sys_data[0])*((2**y)-1)
		pos =5
	else:
		print('Arguments not passed correctly')
		print('exiting program')
		sys.exit()
	

	if len(sys.argv) == 3:
	
		req_frequency = []
		req_energy= []
		x=0
		for i in range(0,int(Sys_data[0])):
			x += float(Task_data[i][2]) / float(Task_data[i][1])	#Checking inequaltiy for Nomral
		if x < eq:													#And EE method respectively
			print("\tInequaltiy satisfied")	
			for i in range(0,len(req_exec)):
				req_frequency.append(frequencies[0])
				req_energy.append(energy[0])
		else:
			print("\tInequaltiy not satisfied\n")
			print('Exiting program')
			sys.exit()

	elif sys.argv[3] == 'EE':
		print("\t ENERGY-EFFICIENT MODE\n")

		possible_combination=[]
		possible_period=[]
		possible_energy=[]
		ind=[]
		ct=0
		for t1 in range(len(f_1)):
			for t2 in range(len(f_2)):
				for t3 in range(len(f_3)):
					for t4 in range(len(f_4)):
						for t5 in range(len(f_5)):			#EE method checking
							x=0 							#Considering all posssible values and checking inequaltiy
							eg=0
							pc=[]
							ec=[]
							i=[]
							t=[]
							
							x = (float(f_1[t1])/float(period[0]))+(float(f_2[t2])/float(period[1]))+(float(f_3[t3])/float(period[2]))+(float(f_4[t4])/float(period[3]))+(float(f_5[t5])/float(period[4]))
							
							if x < eq:
								t.extend([f_1[t1],f_2[t2],f_3[t3],f_4[t4],f_5[t5]])
								possible_combination.append(t)
								pc.append(x)
								pc.append(ct)
								possible_period.append(pc)
								eg = f_1[t1]*energy[t1]+f_2[t2]*energy[t2]+f_3[t3]*energy[t3]+f_4[t4]*energy[t4]+f_5[t5]*energy[t5]
								ec.append(eg)
								ec.append(ct)
								possible_energy.append(ec)
															#appending required data into certain variables
								i.extend([t1,t2,t3,t4,t5])
								ind.append(i)
								ct +=1
		if len(possible_combination) == 0:
			print('\tNo combination satified the Inequality')
			print('\texiting the program')
			sys.exit()


		possible_energy.sort(key=lambda x:x[0])
		possible_period.sort(key=lambda x:x[0],reverse=True)


		v=[]
		t=[]
		n = len(possible_combination)
		for i in range(0,n):
			for j in range(0,n):
				t=[]
				if possible_period[i][1] == possible_energy[j][1]:
					t.append(i+j)
					t.append(possible_period[i][1])		#finding the best possible combination for EE method
					v.append(t)	
		v.sort(key=lambda x:x[0])
		t=[]
		t.append(possible_period[0])
		overall_efficient_combination = int(v[0][1])
		

		seq=[]
		seq =ind[overall_efficient_combination]
		req_exec=[]
		
		req_exec = possible_combination[overall_efficient_combination]

		req_frequency=[]
		req_energy=[]
		for i in range(0,len(seq)):
			req_frequency.append(frequencies[seq[i]])
			req_energy.append(energy[seq[i]])


	
	

	t_time = int(Sys_data[1])

	scheduled_task=[]
	t=0
	for i in range(0,t_time):
		for n in range(0,len(name)):
			temp =[]
			if i % period[n] == 0:
				begin = i
				end = i + req_exec[n]
				cur_deadline = i + period[n]		#calculating all the tasks start time and necessary data
				temp.extend([name[n],begin,end,req_exec[n],cur_deadline,t])
				t+=1
				scheduled_task.append(temp)

	print('\n')
	history = []
	current_task = ""
	prev_task = ""
	lock = 0
	task_queue=[]
	p = []
	idle_id = 1


	for i in range(0,t_time):
		
		for n in scheduled_task:		#calculating all possible task that could arrive at this time period
			if n[1] == i:
				task_queue.append(n)
				task_queue.sort(key=lambda x:x[pos]) #sorting the task queue either based on Deadline or period
		
		if len(task_queue)>0:
			if idle_id == 0:
				idle_id = 1
				history[-1][2]=i


			current_task = task_queue[0][0]
			if current_task != prev_task:
				if lock == 0:
					lock =1
					temp =[]
					temp.append(i)
					temp.append(current_task)
				elif lock == 1:
					
					temp.append(i)
					history.append(temp)  # executing the task and recording its data
					lock == 0
					temp =[]
					temp.append(i)
					temp.append(current_task)
			task_queue[0][3] -=1
			if task_queue[0][3] == 0:
				lock = 0
				temp.append(i+1)

				p.append(task_queue[0][5])
				task_queue.pop(0)
				history.append(temp)		
			prev_task = current_task
		
		else:
			if idle_id == 1:
				idle_id = 0
				temp =[]
				temp.append(i)		#IDLE time calculation
				temp.append("IDLE")
				temp.append(i)
				history.append(temp)
			else:
				history[-1][2]=i
	if lock == 1:
		temp.append(i)
		history.append(temp)


	print('{:<7s}{:^7s}{:^9s}{:^11s}{:^22s}{:>5s}'.format('Start','Task','End','frequency','Energy unit(V)','Energy cosumed J'))
	ct1=0
	ct2=0
	ct3=0
	ct4=0
	ct5=0
	idle_t=0
	for i in history:			#Displaying the executin order along with the energy consumed
		x=0
		if i[1] == "w1":
			x=i[2]-i[0]
			ct1 += i[2]-i[0]
			print('{:<7d}{:^6s}{:^10d}{:^10d}{:^23f}{:^2f}{:>2s}'.format(i[0],'W1',i[2],req_frequency[0],req_energy[0],x*req_energy[0],'J'))
		elif i[1] == "w2":
			x=i[2]-i[0]
			ct2 += i[2]-i[0]
			print('{:<7d}{:^6s}{:^10d}{:^10d}{:^23f}{:^2f}{:>2s}'.format(i[0],'W2',i[2],req_frequency[1],req_energy[1],x*req_energy[1],'J'))
		elif i[1] == "w3":
			x=i[2]-i[0]
			ct3 += i[2]-i[0]
			print('{:<7d}{:^6s}{:^10d}{:^10d}{:^23f}{:^2f}{:>2s}'.format(i[0],'W3',i[2],req_frequency[2],req_energy[2],x*req_energy[2],'J'))
		elif i[1] == "w4":
			x=i[2]-i[0]
			ct4 += i[2]-i[0]
			print('{:<7d}{:^6s}{:^10d}{:^10d}{:^23f}{:^2f}{:>2s}'.format(i[0],'W4',i[2],req_frequency[3],req_energy[3],x*req_energy[3],'J'))
		elif i[1] == "w5":
			x=i[2]-i[0]
			ct5 += i[2]-i[0]
			print('{:<7d}{:^6s}{:^10d}{:^10d}{:^23f}{:^2f}{:>2s}'.format(i[0],'W5',i[2],req_frequency[4],req_energy[4],x*req_energy[4],'J'))
		elif i[1] == "IDLE":
			x=i[2]-i[0]
			idle_t += i[2]-i[0]
			print('{:<7d}{:^6s}{:^10d}{:^10s}{:^23f}{:^2f}{:>2s}'.format(i[0],'IDLE',i[2],'IDLE',idle_pwr,x*idle_pwr,'J'))
	Idle_time = float(idle_t) / float(t_time)
	Idle_power = Idle_time*100
	print('\n')
	print('\n')
	print('IDLE Time(percentage)',Idle_power)
	print('TOTAL - IDLE POWER',float(idle_t)*float(idle_pwr),'J')
	print('\n')

	print('TOTAL - W1 POWER',ct1*req_energy[0],'J')
	print('TOTAL - W2 POWER',ct2*req_energy[1],'J') #displaying necessary data
	print('TOTAL - W3 POWER',ct3*req_energy[2],'J')
	print('TOTAL - W4 POWER',ct4*req_energy[3],'J')
	print('TOTAL - W5 POWER',ct5*req_energy[4],'J')
	print('\n')
	print("Overall Time Taken by TASK in CPU",ct1+ct2+ct3+ct4+ct5)
	print('Overall POWER consumed',((ct1*req_energy[0])+(ct2*req_energy[1])+(ct3*req_energy[2])+(ct4*req_energy[3])+(ct5*req_energy[4])+(idle_t*idle_pwr)),'J')



test()