import sys
import random

random.seed(10)



class pg_list():
	def __init__(self,name):
		self.pte = [None]*128
		self.dirty = [None]*128
		self.visit = [None]*128  # assigning class struct for vpn and other details for processes
		self.p_id = name
		self.refernce =[None]*128

	

def update_visit(table):
	for i in range(len(table)):
		x  = table[list(table)[i]]
		for k in range(len(x.visit)):  # updating the number of time it has elapsed since the memory was accessed
			if x.visit[k] != None :
				x.visit[k] += 1


def update_ref_bit(table):
	for i in range(len(table)):
		x  = table[list(table)[i]]
		for k in range(len(x.visit)):  # Reference bit set to 0 after 200 references
			if x.refernce[k] == 1 :
				x.refernce[k] = 0

def update_to_disk(disk_update,disk_access):
	disk_update += 1
	disk_access += 1

def update_mm(pg_fault,disk_access):
	pg_fault += 1
	disk_access += 1

def rand_algo(size):
	pos = random.randint(0,size-1)  # Rand algorithm

	return pos

def lru_algo(table):
	ru = []
	max_v = []
	for i in range(len(table)):
		val = []
		x  = table[list(table)[i]] ## LRU algorithm implemetation
		y = x.visit
		for l in y:
			if l != None:
				val.append(l)
			else:
				val.append(0)

		m = [max(val),x.p_id]
		max_v.append(m)

	max_v.sort(key=lambda x:x[0],reverse=True)


	rp = 0
	for i in range(len(max_v)-1):

		if max_v[i][0] == max_v[0][0] and i != 0:
			rp +=1
		elif max_v[i][0] <= max_v[i+1][0]:
			break

	dir_v = []
	if rp > 0:

		for i in range(rp):
			val = []
			x  = table[max_v[i][1]]
			
			idx = x.visit.index(max_v[i][0])
			if x.dirty[idx] == 0:
				dir_v.append(max_v[i])
	if len(dir_v) == rp:
		max_v.sort(key=lambda x:x[1])

	max_v.sort(key=lambda x:x[0],reverse=True)

	x  = table[max_v[0][1]]
	idx = x.visit.index(max_v[0][0])

	pos = x.pte[idx]

	return pos



def per_algo(table,mm):
	nr_nd=[]
	nr_d=[]
	r_nd=[]
	r_d=[]
	for i in range(len(mm)):

		val = []
		x  = table[mm[i]]

		idx =x.pte.index(i)
		if x.refernce[idx] == 0:
			if x.dirty[idx] == 0:  # per algortihm implemetation
				nr_nd.append(i)
				
			else:
				nr_d.append(i)
				
		elif x.refernce[idx] == 1:
			if x.dirty[idx] == 0:				
				r_nd.append(i)
				
			else:

				r_d.append(i)
			

	if len(nr_nd) == 0:
		if len(nr_d) == 0:
			if len(r_nd) == 0:
				pos = r_d[0]
			else:
				pos = r_nd[0]
		else:
			pos = nr_d[0]
	else:
		pos = nr_nd[0]

	return pos



def test():

	if len(sys.argv) <= 2:
		print('ERROR: Not enough Parameters')  ##Checking arguments passed length
		prnt('EXITING....')
		sys.exit()
	algo = sys.argv[2]

	count =0
	pg_fault = 0
	disk_update = 0
	disk_access = 0

	mm_size = 32
	page_size_bits = 16	#Initi variables
	offset_bits  = 9
	VPN_bits = 7
	table = {}
	task =[]
	pr=[]
	process =[]
	fifo_i = 0
	mm_ref = 0

	mm = [None]*mm_size
	mk = []
	for i in range(mm_size):
		mk.append(i)
	
	
	my_file = open(sys.argv[1]+'.txt','r')
	testsite_array = my_file.readlines()
	my_file.close() 

	for line in testsite_array:
		line =line.split()
		pr.append(line[0])  ## reading processes
		task.append(line)

	


	for p in task:
		if p[0] not in table:
			table[str(p[0])] = pg_list(str(p[0]))

		b = int(p[1]) >> 9

		a = table[str(p[0])]
		if algo =='per':
			if mm_ref >= 200:
				mm_ref = 0
				update_ref_bit(table)

		if a.pte[b] == None and len(mk) > 0: #un-avoidable page fault
			idx = mk.pop(0)

			a.pte[b] = idx
			a.refernce[b] = 1
			mm_ref +=1
			update_visit(table)
			a.visit[b] = 0
			if p[2] =='W':
				a.dirty[b] = 1
			elif p[2] == 'R' and a.dirty[b] != 1 and a.dirty[b] == None:
				a.dirty[b] = 0

			mm[idx] = a.p_id
			
			
			pg_fault += 1
			disk_access += 1
			
		
		elif a.pte[b] == None and len(mk)==0: #page fault occurs
			count += 1
			if algo == 'rand':
				pos = rand_algo(mm_size)
			elif algo == 'fifo':
				pos = fifo_i
				fifo_i +=1
				if fifo_i == mm_size:	#choosing algorithm
					fifo_i = 0
			elif algo == 'lru':
				pos  = lru_algo(table)
			elif algo == 'per':
				pos = per_algo(table,mm)

			temp = mm[pos]
			v = table[temp]
			idx = v.pte.index(int(pos))
			if v.dirty[idx] == 1:
				
				disk_update += 1
				disk_access += 1

			v.pte[idx] = None
			v.dirty[idx] = None
			v.visit[idx] = None
			v.refernce[idx] = None
			
			mm[pos] = a.p_id
										swapping the maim memory with other memory adn writing back if necessary

			pg_fault += 1
			disk_access += 1

			a.pte[b] = pos
			a.refernce[b] = 1
			mm_ref +=1
			update_visit(table)
			a.visit[b] = a.visit[b] = 0

			if p[2] =='W':

				a.dirty[b] = 1
			elif p[2] == 'R' and a.dirty[b] != 1 and a.dirty[b] == None:
				a.dirty[b] = 0
			


		elif a.pte[b] != None:			#when memory is referenced without page fault
			update_visit(table)
			a.visit[b] = 0
			a.refernce[b] =1
			mm_ref +=1
			if p[2] =='W':
				a.dirty[b] = 1
			elif p[2] == 'R' and a.dirty[b] != 1 and a.dirty[b] == None:
				a.dirty[b] = 0

 			

	print('page_fault-',pg_fault)
	print('disk updated(written back)-',disk_update)
	print('disk accessed(during page fault and written back)-',disk_access)	
		

test()