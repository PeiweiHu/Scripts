import os

import convert

TEMPLATE_PATH = 'C:/Users/11910/Desktop/bw_blog/template/'
ROOT_PATH = 'C:/Users/11910/Desktop/bw_blog/root/'
POST_PATH = 'C:/Users/11910/Desktop/bw_blog/root/post/'
MD_PATH = 'C:/Users/11910/Desktop/bw_blog/root/md/'


def read_template(template):
	content = ''
	if not os.path.exists(template):
		print('[-]Error: template ' + template + ' not exists. (construct_page.py/read_template)')
		return False
	f = open(template, 'r', encoding='utf-8')
	for line in f.readlines():
		content += line
	f.close()
	return content


"""
fill in

	title, author, date, tag, content avaliable
@para
	t_name: (str) tag name
	template: (str) template content
	content: (str) content wanna fill in tag
"""
def fill_in(t_name, template, content):
	embed_tag = ['title']
	span_tag = ['date', 'tag', 'author']
	div_tag = ['content']

	if t_name in span_tag:
		start = "<span id='bw_blog_{0}'>".format(t_name)
		end = "</span>"

	if t_name in embed_tag:
		start = "<{0}>".format(t_name)
		end = "</{0}>".format(t_name)

	if t_name in div_tag:
		start = "<div id='bw_blog_{0}'>".format(t_name)
		end = "</div>"

	#fill in content to target tag
	start_pos = template.find(start)
	if start_pos < 0:
		print('[-]Error: can not fill in template because of tag error. (construct_page.py/fill_in)')
		return False
	end_pos = template.find(end, start_pos)
	if end_pos < 0:
		print('[-]Error: can not fill in template because of tag error. (construct_page.py/fill_in)')
		return False
	start_pos = start_pos + len(start)

	template = template[:start_pos] + content + template[end_pos:]
	return template 


"""
construct post page
@para:
	md: (str)markdown file path
	output: (str)output path
"""
def construct_postpage(md):
	#note that possible errors are handled in called functions, so simply return False here
	template = read_template(TEMPLATE_PATH + 'post_page.html')
	if not template:
		return False
	meta_info = convert.meta_info(md)
	if not meta_info:
		return False
	for _key in meta_info.keys():
		template = fill_in(_key, template, meta_info[_key])
		if not template:
			return False
	content = convert.convert(md)
	if not content:
		return False
	template = fill_in('content', template, content)
	return template


"""
output the constructed page of md into category
@para:
	md: (str)markdown file path
	path: (str)fixed path to store posts
"""
def category_manage(md, path=POST_PATH):
	meta_info = convert.meta_info(md)
	if not meta_info:
		return False
	title = meta_info['title'].replace(' ', '_')
	if not os.path.exists(path):
		os.mkdir(path)
	filename = path.rstrip('/') + '/' + title + '.html'
	page = construct_postpage(md)
	if not page:
		return False
	with open(filename, 'w') as f:
		f.write(page)
	return True


"""
update index according to post path
@para:
	path: (str)path to store posts
"""
def index_update(path=POST_PATH):
	if not os.path.exists(path):
		print('[-]Error: post path ' + POST_PATH + ' not exists. (construct_page.py/index_update)')
		return False
	li_lst = ''
	for file in os.listdir(path):
		title = file.rstrip('.html').replace('_', ' ')
		li_lst += '<li><a href="{0}">{1}</a></li>'.format('.' + path[path.rstrip('/').rfind('/'):] + file, title)
	template = read_template(TEMPLATE_PATH + 'index.html')
	if not template:
		return False
	start = '<ul id="bw_list">'
	end = '</ul>'
	start_pos = template.find(start)
	if start_pos < 0:
		print('[-]Error: wrong format in template index.html, can not find <ul id="bw_list"> (construct_page.py/index_update)')
		return False
	end_pos = template.find(end, start_pos)
	if end_pos < 0:
		print('[-]Error: wrong format in template index.html, can not find </ul> (construct_page.py/index_update)')
		return False
	start_pos = start_pos + len(start)
	template = template[:start_pos] + li_lst + template[end_pos:]
	with open(ROOT_PATH + 'index.html', 'w') as f:
		f.write(template)


"""
scan the md directory and construct pages, update index
@para:
	md: (str)markdown file directory
"""
def update(md=MD_PATH):
	if not os.path.exists(md):
		print('[-]Error: md path ' + md + ' not exists. (construct_page.py/update)')
		return False
	for file in os.listdir(md):
		category_manage(md + file)
	index_update()


if __name__ == '__main__':
	update()
	
