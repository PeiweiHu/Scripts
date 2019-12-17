import requests
import re
import threading
from queue import Queue
from time import sleep
import urllib.parse

"""
if u can not access Wikipedia directly
"""
proxies = {'https':'https://127.0.0.1:1080',
		   'http':'http://127.0.0.1:1080',
			}

url = 'https://en.wikipedia.org/wiki/Category:{0}'
ptn = re.compile('"/wiki/Category:(.*?)"')

cnt = 0              #count requests
threads_cnt= 30      #max thread number
q = Queue()          #save sub categories temporarily


"""
access single page of wikipedia, invoked by sub thread in "extract"
@para:
	url: (str)exact url of single page
	q: (Queue)queue used to save sub categories
"""
def extract_page(url, q):
	global cnt, proxies

	try:
		txt = requests.get(url, proxies=proxies).text
	except Exception as e:
		print(e)
		return
	cnt += 1
	q.put(set(ptn.findall(txt)))


"""
generate sub categories of start_category
@para:
	start_category: (list)key word used to pull sub categories
	level: (int)define the depth of sub categories
"""
def extract(start_category, level):
	global q, url, cnt, threads_cnt
	res = dict()
	#transform native list to set, which does a favor on removing duplicate categories
	res[0] = set(start_category)
	for _ in range(level):
		print('[+]start level ' + str(_ + 1))
		cnt = 0
		thread_lst = []
		res[_ + 1] = set()

		#start creating sub threads for current level
		for _cat in res[_]:
			while threading.active_count() >= threads_cnt:
				sleep(1)
			t = threading.Thread(target=extract_page, args=(url.format(_cat), q))
			thread_lst.append(t)
			t.start()
		#wait until all sub threads come to an end
		for t in thread_lst:
			t.join()

		print(str(cnt) + ' valid requets for level ' + str(_ + 1))

		#construct the category set of current level
		while not q.empty():
			_set = q.get()
			for _s in _set:
				res[_ + 1].add(_s)
	return res


"""
save result
@para:
	res: (dict)contains sub categories in different level
	output: (str)output file path
	is_split: (int)indicate the output format(0->url format 1->dump file format)
"""
def output(res, output , is_split=0):
	cat_s = set()
	for key in res.keys():
		cats = res[key]
		for _ in cats:
			if is_split:
				_ = _.replace('_', '')
			cat_s.add(urllib.parse.unquote(_))
	with open(output, 'w') as f:
		for _ in cat_s:
			f.write(_ + '\n')


if __name__ == '__main__':

	START = ['Computers', 'Computer_science']
	LEVEL = 1
	OUTPUT = '/path/to/output'

	res = extract(START, LEVEL)
	output(res, OUTPUT)