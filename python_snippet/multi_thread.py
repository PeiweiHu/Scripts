from threading import Thread
from queue import Queue
from time import time, sleep

def sub_func(q):
	sleep(0.5)
	q.put('goods')

def func(threads_num=5):
	t_lst = []
	q = Queue()
	for _ in range(50):
		t_lst += [Thread(target=sub_func, args=(q,))]
		if len(t_lst) == threads_num:
			for t in t_lst:
				t.start()
			for t in t_lst:
				t.join()
			t_lst = []
	return q

if __name__ == '__main__':
	t = time()
	q = func()
	t = time() - t
	print('time: ' + str(t) + ' queue length: ' + str(q.qsize()))