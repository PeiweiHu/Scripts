import os
import random

CASE_LOC = ''
CASE_NUM = 20000

TRAIN_LOC = ''
TEST_LOC = ''
VALI_LOC = ''

def split_case():
    total_index = [_r for _r in range(CASE_NUM)]
    random.shuffle(total_index)

    train_num = int(CASE_NUM * 10 / 12)
    test_num = int((CASE_NUM - train_num) / 2)
    vali_num = CASE_NUM - train_num - test_num

    train_index = total_index[:train_num]
    test_index = [train_num:train_num+test_num]
    vali_index = [train_num+test_num:train_num+test_num+vali_num]

    for _t in train_index:
        cmd = 'cp -r ' + CASE_LOC.format(_t) + ' ' + TRAIN_LOC
        print(cmd)
        #os.system(cmd)
    for _t in test_index:
        cmd = 'cp -r ' + CASE_LOC.format(_t) + ' ' + TEST_LOC
        print(cmd)
        #os.system(cmd)
    for _t in vali_index:
        cmd = 'cp -r ' + CASE_LOC.format(_t) + ' ' + VALI_LOC
        print(cmd)
        # os.system(cmd)


if __name__ == '__main__':
    split_case()



