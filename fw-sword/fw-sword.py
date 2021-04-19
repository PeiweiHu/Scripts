import os
import random
import sys
import hashlib
import json
import pdfkit


BIN_MD5_SAVE = './md5/'
EXT_SAVE = './ext/'


def analyse_php(path, out_file):
    php_lst = []
    for root, dirs, files in os.walk(path):
        for _f in files:
            if _f.endswith('.php'):
                php_lst.append(os.path.join(root, _f))
    dir_set = set()
    for _p in php_lst:
        dir_set.add(os.path.dirname(_p))
    for _d in dir_set:
        print('[+]start analysing ' + _d)
        os.system('echo "start analysing {0}" >> {1}'.format(_d, out_file))
        os.system('./vendor/bin/phpstan analyse {0} >> {1}'.format(_d, out_file))


# util function for finding web directory
def find_web_sub(path):
    php_lst = []
    for root, dirs, files in os.walk(path):
        for _f in files:
            if _f.endswith('.php'):
                php_lst.append(os.path.join(root, _f).strip('/'))
                #print('add', os.path.join(root, _f))
    php_lst = [_p.split('/') for _p in php_lst]
    php_ele = random.choice(php_lst)
    #print(php_ele)
    dir_cnt = 0
    vote_cnt = 0

    while True:
        ele = php_ele[dir_cnt]
        for _p in php_lst:
            if len(_p) > dir_cnt and _p[dir_cnt] == ele:
                vote_cnt += 1
        #print('vote to ', php_ele[dir_cnt], str(vote_cnt))
        if vote_cnt/len(php_lst) < 0.2:
            break
        vote_cnt = 0
        dir_cnt += 1

    php_ele = php_ele[:dir_cnt]
    return '/'+'/'.join(php_ele)


# find web directory within specified path
def find_web_dir(path):
    web_path = set()
    for _r in range(10):
        web_path.add(find_web_sub(path))
    # remoce recursive direcroty
    rep_path = []
    for _w in web_path:
        for _ww in web_path:
            if _w in _ww and _w != _ww:
                rep_path.append(_ww)
    for _r in rep_path:
        web_path.remove(_r)
    return web_path


# this function returns a dict contains valid file name and corresponding md5
def get_md5(path):
    bin_dict = dict()
    for _dir in os.listdir(path):
        _dir_full = os.path.join(path, _dir)
        if os.path.isfile(_dir_full):
            with open(_dir_full, 'rb') as r:
                data = r.read()
            bin_dict[_dir] = hashlib.md5(data).hexdigest()
    return bin_dict


def check_md5(extracted_path, bin_id):
    work_dir = os.getcwd()

    # find existing dict file
    bin_dict_path = bin_id + '_bin'
    bin_dict_path = os.path.join(BIN_MD5_SAVE, bin_dict_path)
    sbin_dict_path = bin_id + '_sbin'
    sbin_dict_path = os.path.join(BIN_MD5_SAVE, sbin_dict_path)

    # load existing dict file
    if not os.path.exists(bin_dict_path):
        print('[-]' + bin_dict_path + ' does not exist')
        bin_dict = None
        return
    else:
        with open(bin_dict_path, 'rb') as r:
            bin_dict = json.loads(r.read())
    if not os.path.exists(sbin_dict_path):
        print('[-]' + sbin_dict_path + ' does not exist')
        sbin_dict = None
        return
    else:
        with open(sbin_dict_path, 'rb') as r:
            sbin_dict = json.loads(r.read())

    # extract firmware
    dir_content = os.listdir(work_dir)
    """
    os_rtn = os.system('binwalk -e ' + bin_path)
    if os_rtn != 0:
        print('[-]Error while run binwalk')
        return
    extracted_dir = [_d for _d in os.listdir(work_dir) if not _d in dir_content][0]
    """
    bin_path = os.path.join(extracted_path, 'squashfs-root', 'bin')
    sbin_path = os.path.join(extracted_path, 'squashfs-root', 'sbin')

    if not os.path.exists(bin_path):
        print('[-]Bin directory does not exist: ' + bin_path)
        return
    else:
        test_bin_dict = get_md5(bin_path)
    if not os.path.exists(sbin_path):
        print('[-]Sbin directory does not exist: ' + sbin_path)
        return
    else:
        test_sbin_dict = get_md5(sbin_path)
    
    # start comparision
    # test bin
    bin_in_old = dict()
    bin_in_new = dict()
    bin_diff_md5 = dict()
    bin_same = dict()
    for _k in bin_dict.keys():
        if not _k in test_bin_dict.keys():
            bin_in_old[_k] = bin_dict[_k]
        else:
            if bin_dict[_k] != test_bin_dict[_k]:
                bin_diff_md5[_k] = [bin_dict[_k], test_bin_dict[_k]]
            else:
                bin_same[_k] = bin_dict[_k]

    for _k in test_bin_dict.keys():
        if not _k in bin_dict.keys():
            bin_in_new[_k] = test_bin_dict[_k]
    
    # test sbin
    sbin_in_old = dict()
    sbin_in_new = dict()
    sbin_diff_md5 = dict()
    sbin_same = dict()
    for _k in sbin_dict.keys():
        if not _k in test_sbin_dict.keys():
            sbin_in_old[_k] = sbin_dict[_k]
        else:
            if sbin_dict[_k] != test_sbin_dict[_k]:
                sbin_diff_md5[_k] = [sbin_dict[_k], test_sbin_dict[_k]]
            else:
                sbin_same[_k] = sbin_dict[_k]

    for _k in test_sbin_dict.keys():
        if not _k in sbin_dict.keys():
            sbin_in_new[_k] = test_sbin_dict[_k]
    
    return [bin_in_new, bin_in_old, bin_diff_md5, sbin_in_new, sbin_in_old, sbin_diff_md5, bin_same, sbin_same]


# read a bin file and record corresponding md5 value to conf 
def record_md5(img_path, bin_id):
    work_dir = os.getcwd()
    dir_content = os.listdir(work_dir)
    
    os_rtn = os.system('binwalk -e ' + img_path)
    if os_rtn != 0:
        print('[-]Error while run binwalk')
        return
    extracted_dir = [_d for _d in os.listdir(work_dir) if not _d in dir_content][0]

    bin_path = os.path.join(extracted_dir, 'squashfs-root', 'bin')
    sbin_path = os.path.join(extracted_dir, 'squashfs-root', 'sbin')
    bin_dict = dict()
    sbin_dict = dict()
    if os.path.exists(bin_path):
        for _dir in os.listdir(bin_path):
            if os.path.isfile(os.path.join(bin_path, _dir)):
                print('file: ' + os.path.join(bin_path, _dir))
                with open(os.path.join(bin_path, _dir), 'rb') as r:
                    data = r.read()
                bin_dict[_dir] = hashlib.md5(data).hexdigest()
                print('bin - ' + _dir + ' hash - ' + bin_dict[_dir])
        print('[+]Success to extract md5 from bin directory')
    else:
        print('[-]Bin directory does not exist')
    if os.path.exists(sbin_path):
        for _dir in os.listdir(sbin_path):
            if os.path.isfile(os.path.join(sbin_path, _dir)):
                print('file: ' + os.path.join(sbin_path, _dir))
                with open(os.path.join(sbin_path, _dir), 'rb') as r:
                    data = r.read()
                sbin_dict[_dir] = hashlib.md5(data).hexdigest()
                print('bin - ' + _dir + ' hash - ' + sbin_dict[_dir])
        print('[+]Success to extract md5 from sbin directory')
    else:
        print('[-]Sbin directory does not exist')
    
    # save
    if len(bin_dict.keys()) != 0:
        with open(os.path.join(BIN_MD5_SAVE, bin_id + '_bin'), 'w') as w:
            w.write(json.dumps(bin_dict))
    if len(sbin_dict.keys()) != 0:
        with open(os.path.join(BIN_MD5_SAVE, bin_id + '_sbin'), 'w') as w:
            w.write(json.dumps(sbin_dict))
    # remove extraced fs
    os.sysetem('rm -r ' + extracted_dir)


def controller(bin_path, bin_id, out_file='tmp_out.html'):
    work_dir = os.getcwd()
    dir_content = os.listdir(work_dir)

    os.system('echo  "<!DOCTYPE html><html><head><style>body {{background-color:white;}}</style></head><body><pre>" >> {0}'.format(out_file))

    # brief info
    os.system('echo "\n\n===================================BRIEF INFO==========================================\n\n" >> {0}'.format(out_file))
    os.system('binwalk {0} >> {1}'.format(bin_path, out_file))
    # extract
    os_rtn = os.system('binwalk -e ' + bin_path)
    if os_rtn != 0:
        print('[-]Error while run binwalk')
        return
    extracted_dir = [_d for _d in os.listdir(work_dir) if not _d in dir_content][0].rstrip('/')
    os.system('mv ./{0} {1}/'.format(extracted_dir, EXT_SAVE))
    extracted_dir = os.path.join(EXT_SAVE, extracted_dir)
    # detect web directory
    web_path = find_web_dir(extracted_dir)
    os.system('echo "\n\n===================================WEB DIR==========================================\n\n" >> {0}'.format(out_file))
    for _w in web_path:
        os.system('echo "{0}\n" >> {1}'.format(_w,out_file))
    # md5 result
    os.system('echo "\n\n===================================MD5 RESULT==========================================\n\n" >> {0}'.format(out_file))
    bin_dict_path = bin_id + '_bin'
    bin_dict_path = os.path.join(BIN_MD5_SAVE, bin_dict_path)
    sbin_dict_path = bin_id + '_sbin'
    sbin_dict_path = os.path.join(BIN_MD5_SAVE, sbin_dict_path)
    if os.path.exists(bin_dict_path) and os.path.exists(sbin_dict_path):
        lst = check_md5(extracted_dir, bin_id)
        bin_in_new = lst[0]
        bin_in_old = lst[1]
        bin_diff_md5 = lst[2]
        sbin_in_new = lst[3]
        sbin_in_old = lst[4]
        sbin_diff_md5 = lst[5]
        bin_same = lst[6]
        sbin_same = lst[7]

        os.system('echo "\n\nBIN DIRECTORY - ONLY IN NEW\n\n" >> {0}'.format(out_file))
        for _k in bin_in_new.keys():
            os.system('echo "\n\n{0} - {1}\n\n" >> {2}'.format(_k, bin_in_new[_k], out_file))
        
        os.system('echo "\n\nBIN DIRECTORY - ONLY IN OLD\n\n" >> {0}'.format(out_file))
        for _k in bin_in_old.keys():
            os.system('echo "\n\n{0} - {1}\n\n" >> {2}'.format(_k, bin_in_old[_k], out_file))
        
        os.system('echo "\n\nBIN DIRECTORY - DIFF\n\n" >> {0}'.format(out_file))
        for _k in bin_diff_md5.keys():
            os.system('echo "\n\n{0} - {1},{2}\n\n" >> {3}'.format(_k, bin_diff_md5[_k][0], bin_diff_md5[_k][1], out_file))
        
        os.system('echo "\n\nBIN DIRECTORY - SAME\n\n" >> {0}'.format(out_file))
        for _k in bin_same.keys():
            os.system('echo "{0} - {1}" >> {2}'.format(_k, bin_same[_k], out_file))


        
        os.system('echo "\n\nSBIN DIRECTORY - ONLY IN NEW\n\n" >> {0}'.format(out_file))
        for _k in sbin_in_new.keys():
            os.system('echo "\n\n{0} - {1}\n\n" >> {2}'.format(_k, sbin_in_new[_k], out_file))
        
        os.system('echo "\n\nSBIN DIRECTORY - ONLY IN OLD\n\n" >> {0}'.format(out_file))
        for _k in sbin_in_old.keys():
            os.system('echo "\n\n{0} - {1}\n\n" >> {2}'.format(_k, sbin_in_old[_k], out_file))
        
        os.system('echo "\n\nSBIN DIRECTORY - DIFF\n\n" >> {0}'.format(out_file))
        for _k in sbin_diff_md5.keys():
            os.system('echo "\n\n{0} - {1},{2}\n\n" >> {3}'.format(_k, sbin_diff_md5[_k][0], sbin_diff_md5[_k][1], out_file))
        
        os.system('echo "\n\nSBIN DIRECTORY - SAME\n\n" >> {0}'.format(out_file))
        for _k in sbin_same.keys():
            os.system('echo "{0} - {1}" >> {2}'.format(_k, sbin_same[_k], out_file))

    else:
        os.system('echo "\n\nNO EXISTING MD5 DATABASE\n\n" >> {0}'.format(out_file))
    
    # phpstan
    os.system('echo "\n\n===================================PHPSTAN==========================================\n\n" >> {0}'.format(out_file))
    analyse_php(extracted_dir, out_file)

    # firmwalker
    os.system('echo "\n\n===================================FIRMWALKER==========================================\n\n" >> {0}'.format(out_file))
    os.chdir('./firmwalker')
    os.system('./firmwalker.sh {0} {1}'.format('../'+extracted_dir, '../out.txt '))
    os.chdir(work_dir)
    os.system('cat out.txt >> {0}'.format(out_file))

    os.system('echo  "</pre></body></html>" >> {0}'.format(out_file))


if __name__ == '__main__':
    if sys.argv[1] = 'analyse':
        bin_file = sys.argv[2]
        bin_id = sys.argv[3]
        out = sys.argv[4]
        if not os.path.exists(bin_file):
            print('File {0} does not exist. exit.'.format(bin_file))
            exit()
        # clean existing tmp file
        os.system('rm tmp_out.html')
        # work
        controller(bin_file, bin_id, out_file='tmp_out.html')
        # change to pdf
        pdfkit.from_file('tmp_out.html', out.rstrip('.pdf') + '.pdf')
    elif sys.argv[1] == 'record'
        bin_file = sys.argv[2]
        bin_id = sys.argv[3]
        if os.path.exists(os.path.join(BIN_MD5_SAVE, bin_id+'_bin')) or os.path.exists(os.path.join(BIN_MD5_SAVE, bin_id+'_sbin')):
            print('Recording existing binary id. exit.')
            exit()
        record_md5(bin_file, bin_id)
    else:
        print("Help:")
        print("Scan target firmware: python3 fw-sword.py analyse [bin_file] [bin_id] [out_pdf]")
        print("e.g. python3 fw-sword.py analyse ./testcase/DWP2360b-firmware-v210-rc020.bin dwp2360b-v210-rc020 ./result.pdf")
        


