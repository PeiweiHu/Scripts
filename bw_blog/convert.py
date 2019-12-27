import os

import markdown

"""
typical post format:

[bw_blog_start]

title = {your title}

date = {your date}

tag = {your tag}

author = {author name}

[bw_blog_end]

{your content in markdown format}

"""

"""
extract words between { and }
"""
def extract(line):
	left_pos = line.find('{')
	right_pos = line.rfind('}')
	if left_pos < 0 or right_pos < 0:
		return False
	return line[left_pos+1:right_pos]


"""
extract meta info of md file
@para:
	md_path: (str)markdown file path
"""
def meta_info(md_path):
	meta = dict()
	meta_tag = ['title', 'date', 'tag', 'author']
	if not os.path.exists(md_path):
		print('[-]Error: path ' + md_path + ' not exists. (convert.py/meta_info)')
		return False
	flag = 0
	with open(md_path, 'r', encoding='utf-8') as f:
		for line in f.readlines():
			if '[bw_blog_start]' in line:
				flag = 1
			elif '[bw_blog_end]' in line:
				return meta
			if flag == 1:
				for _tag in meta_tag:
					if line.strip().startswith(_tag):
						content = extract(line)
						if not content:
							print('[-]Error: can not extract words between { and } while solving ' + _tag + ' (convert.py/meta_info)')
							return False
						meta[_tag] = content
	return meta


"""
convert content of md file to html format
@para:
	md_path: (str)markdown file path
"""
def convert(md_path):
	if not os.path.exists(md_path):
		print('[-]Error: path ' + md_path + ' not exists. (convert.py/convert)')
		return False
	content = ''
	flag = 0
	with open(md_path, 'r', encoding='utf-8') as f:
		for line in f.readlines():
			if '[bw_blog_end]' in line:
				flag = 1
				continue
			if flag == 1:
				content += line
	return markdown.markdown(content)
