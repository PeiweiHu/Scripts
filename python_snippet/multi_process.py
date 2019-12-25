from time import sleep, time
from multiprocessing import Process, Manager
"""
more data structures are avaliable in Manager

invoke the function containing multiple processes in the main function
"""
def sub_func(lst):
	sleep(0.5)
	lst.append('goods')

def func(process_num=5):
	lst = Manager().list()
	p_lst = []
	for _ in range(50):
		p_lst += [Process(target=sub_func, args=(lst,))]
		if len(p_lst) == process_num:
			for p in p_lst:
				p.start()
			for p in p_lst:
				p.join()
			p_lst = []
	return lst

if __name__ == '__main__':
	t = time()
	lst = func()
	t = time() - t
	print('time: ' + str(t) + ' list length: ' + str(len(lst)))