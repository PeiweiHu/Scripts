import sys
import re
import os

from gensim.corpora import WikiCorpus
from gensim.corpora import wikicorpus

rm_pattern = ',|\'|\"|\(|\)|\{|\}|\[|\]|\n|=|\+|-|!|@|#|\$|%|&|\*|\^|`|~|<|>|:|_|\/|;|\|'

"""
Define categories u wanna extract.

Now that we already got categories with 
interesting themes from extract_category.py, 
ADD read file function to import them.
"""
related_flag = ['Category:Computers',
                'Category:Computing by computer model',
                'Category:Computer architecture',
                'Category:Classes of computers',
                'Category:Data centers',
                'Category:Computers and the environment',
                'Category:Evaluation of computers',
                'Category:Fictional computers',
                'Category:Computer hardware',
                'Category:History of computing',
                'Category:Information appliances',
                'Category:Computer-related introductions by year',
                'Category:Computer programming',
                'Category:Computer systems',
                'Category:Artificial intelligence',
                'Category:Artificial intelligence applications',
                'Category:Human–computer interaction',
                'Category:Virtual reality',
                'Category:World Wide Web',
                'Category:Computer vision',
                'Category:Machine learning',
                'Category:Artificial neural networks',
                'Category:Data mining and machine learning software',
                'Category:Computer engineering',
                'Category:Software engineering',
                'Category:Software architecture',
                'Category:Distributed computing architecture',
                'Category:Cloud computing',
                'Category:Programming languages',
                'Category:Software design patterns',
                ]

"""
Define all categories with special keywords.

U don't need it in most of time except u really
can not get corpus big enough. Delete comment below
(line 78)to open it (with high risk of low accuracy).
"""
reg_flag = ['Category:.*?[A|a]ndroid',
            'Category:.*?[C|c]omputer',
            'Category:.*?[H|h]ardware',
            'Category:.*?[S|s]oftware',
            ]


def analyse(target, output, output1):
    cnt = 0
    wanna_1 = 0
    wanna_2 = 0
    f_in = open(target, 'r')
    f_out = open(output, 'w')
    f_out1 = open(output1, 'w')
    try:
        pages = wikicorpus.extract_pages(f_in)
        for _ in pages:
            cnt += 1
            is_wanna  = 0

            for flag in related_flag:
                if flag in _[1]:
                    is_wanna = 1
            """
            if not is_wanna:
                for flag in reg_flag:
                    if re.search(flag, _[1]):
                        is_wanna = 2 
            """
            if is_wanna:
                txt = re.sub(rm_pattern, '', wikicorpus.remove_markup(_[1]))
                if is_wanna == 1:
                    wanna_1 += 1
                    f_out.write(txt + '\n\n')
                elif is_wanna == 2:
                    wanna_2 += 1
                    f_out1.write(txt + '\n\n')

            if cnt % 1000 == 0:
                print('solved: ' + str(cnt) + ' wanna(accurate): ' + str(wanna_1) + ' wanna:' + str(wanna_2))

    except Exception as e:
        print(str(e))
        print('solved: ' + str(cnt) + ' wanna(accurate): ' + str(wanna_1) + ' wanna:' + str(wanna_2))
        
    finally:            
        f_in.close()
        f_out.close()
        f_out1.close()
        os._exit(0)

if __name__ == '__main__':
    if len(sys.argv) != 4:
        print('usage: python3 wiki.py dump_file(xml) output_file(accurate) output_file\n')
        os._exit(0)    

    in_f = sys.argv[1]
    out_f = sys.argv[2]
    out_f1 = sys.argv[3]

    if not os.path.exists(in_f):
        print('dump_file not exists \n')
        os._exit(0)

    analyse(in_f, out_f, out_f1)






