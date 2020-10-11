import struct
import sys
import os
import subprocess
from ruamel import yaml


LLVM_FILE = "ir.ll"
in_main = False
#variable_init, variable_name, function_name, in_main, variable_location


def init_Config():
    global variable_num, loop_num, loop_start, fi_type
    file = open('set.yaml', 'r', encoding="utf-8")
    file_data = file.read()
    file.close()
    
    data = yaml.safe_load(file_data)
    fi_type = data['fi_type']
    variable_num=data['variable_num']
    if 'loop_num' in data:
        loop_num = data['loop_num']





def config(i):
    global variable_init, variable_name, function_name, in_main, variable_location
    global in_arr, in_loop, loop_num, variable_num
    
    file = open('set.yaml', 'r', encoding="utf-8")
    file_data = file.read()
    file.close()
    
    data = yaml.safe_load(file_data)


    if data['option'][i]['function_name'] == "main":
        in_main = True
    else:
        in_main = False
    variable_num = int(data['variable_num'])
    function_name = data['option'][i]['function_name']
    variable_name = data['option'][i]['variable_name']
    variable_init = bool(data['option'][i]['variable_init'])
    variable_location = int(data['option'][i]['variable_location'])
    in_arr = bool(data['option'][i]['in_arr'])
    in_loop = bool(data['option'][i]['in_loop'])
    if in_loop and 'loop_num' in data:
        loop_num = int(data['loop_num'])
    
def inLine(variable_name, line):
    tmp1 = '%' + variable_name + ','
    tmp2 = '@' + variable_name + ','
    if tmp1 in line.split() or tmp2 in line.split():
        return True
    else:
        return False
    

def config_loop(index):
    global loop_start
    f = open(LLVM_FILE, 'r')
    file = f.readlines()
    file = reversed(file)
    con = False
    for line in file:
        if "!llfi_index !" + str(index) in line:
            con = True
        if con and "icmp slt" in line:
            loop_start = int(line[:-1].split('!')[-1])
            return
            



    
# init是全局变量或者局部变量，非init是传递的变量

def detectFormain(variable_name):
    file = open(LLVM_FILE, 'r')


def detectarrForinit(variable_name, function_name, variable_location):
    file = open(LLVM_FILE, 'r')
    con = False
    location = 1
    for line in file:
        if "}" in line:
            con = False
        if line[0] != ';' and line[0] != '\n' and con == True:
            if inLine(variable_name, line):
                if variable_location == location:
                    print(int(line[:-1].split('!')[-1])+2)
                    file.close()
                    return int(line[:-1].split('!')[-1])+2
                location += 1
        if function_name in line and "define" in line:
            con = True
    file.close()


def detectarrForNotinit(variable_name, function_name, variable_location):
    file = open(LLVM_FILE, 'r')
    con = False
    location = 1
    mem_address = "###"
    for line in file:
        if "}" in line:
            con = False
        if line[0] != ';' and line[0] != '\n' and con == True:
            if "store" in line[:-1].split() and '%'+variable_name+',' in line[:-1].split():
                # print(line[:-1])
                mem_address = line.split()[4]
            if mem_address != "###" and mem_address in line[:-1].split():
                # print(line[:-1])
                if variable_location == location:
                    print(int(line[:-1].split('!')[-1])+2)
                    file.close()
                    return int(line[:-1].split('!')[-1])+2
                location += 1
        if variable_name in line and variable_name in line:
            con = True
    file.close()




def detectForinit(variable_name, function_name, variable_location):
    file = open(LLVM_FILE, 'r')
    con = False
    location = 1
    for line in file:
        if "}" in line:
            con = False
        if line[0] != ';' and line[0] != '\n' and con == True:
            if inLine(variable_name, line):
                if variable_location == location:
                    print(int(line[:-1].split('!')[-1]))
                    file.close()
                    return int(line[:-1].split('!')[-1])
                location += 1

        if function_name in line and "define" in line:
            con = True
    file.close()


def detectForNotinit(variable_name, function_name, variable_location):
    file = open(LLVM_FILE, 'r')
    con = False
    location = 1
    mem_address = "###"
    
    for line in file:
        if "}" in line:
            con = False
        if line[0] != ';' and line[0] != '\n' and con == True:
            if "store" in line[:-1].split() and '%'+variable_name+','==line.split()[2]:
                mem_address = line.split()[4]
            elif mem_address != "###" and mem_address in line.split():
                if variable_location == location:
                    print(int(line[:-1].split('!')[-1]))
                    file.close()
                    return int(line[:-1].split('!')[-1])
                location += 1
        if function_name in line and "define" in line:
            con = True
    file.close()


def main(args):
    global variable_init, variable_name, function_name, in_main, variable_location
    global in_arr, in_loop, loop_num, variable_num, loop_start, fi_type
    init_Config()

    
    file = open('sample.yaml', 'r', encoding="utf-8")
    file_data = file.read()
    file.close()
    data = yaml.safe_load(file_data)
    data['runOption'][0]['run']['fi_index'] = []
    data['compileOption']['instSelMethod'][0]['funcname']['include'] = []
    for i in range(int(variable_num)):
        config(i)
        print(variable_name)
        if function_name not in data['compileOption']['instSelMethod'][0]['funcname']['include']:
            data['compileOption']['instSelMethod'][0]['funcname']['include'].append(function_name)
        if in_main == True:
            print('main')
            detectFormain(variable_name)
        elif in_arr == False:
            if variable_init == True:
                index = detectForinit(
                    variable_name, function_name, variable_location)
                data['runOption'][0]['run']['fi_index'].append(index)
            else:
                index = detectForNotinit(
                    variable_name, function_name, variable_location)
                data['runOption'][0]['run']['fi_index'].append(index)

        else:
            if variable_init == True:
                index = detectarrForinit(
                    variable_name, function_name, variable_location)
                data['runOption'][0]['run']['fi_index'].append(index)
            else:
                index = detectForNotinit(
                    variable_name, function_name, variable_location) + 2
                data['runOption'][0]['run']['fi_index'].append(index)
        
        if in_loop == True:
            config_loop(index)
            data['runOption'][0]['run']['loop_number'] = loop_num
            data['runOption'][0]['run']['loop_start'] = loop_start
                
    
    
    
    data['runOption'][0]['run']['fi_type'] = fi_type
    nums=len(data['runOption'][0]['run']['fi_index'])
    data['runOption'][0]['run']['num_of_data'] = nums
    file1 = open('input.yaml', 'w', encoding='utf-8')
    yaml.dump(data, file1, Dumper=yaml.RoundTripDumper)
    file1.close()


if __name__ == "__main__":
    main(sys.argv[1:])

'''
def tohex(decVal):
    return str(hex(struct.unpack("<Q", struct.pack("d", decVal))[0]))[2:]


file = open('sample.yaml', 'r', encoding="utf-8")
file_data = file.read()
file.close()
data = yaml.safe_load(file_data)
data['runOption'][0]['run']['fi_index'] = []


f = open("llfi/baseline/llfi.stat.trace.prof.txt")
inputf = open("in.txt")

nums = []
for i in inputf:
    for num in i.split():
        nums.append(float(num))

line = f.readline()
for num in nums:
    num = tohex(num)
    while line:
        thisline = line[:-1]
        thisline = thisline.split()
        if thisline[3] == 'load' and thisline[5] == str(num):
            print(thisline[1])
            data['runOption'][0]['run']['fi_index'].append(int(thisline[1]))
            line = f.readline()
            break
        else:
            line = f.readline()
data['runOption'][0]['run']['num_of_data'] = len(nums)
file1 = open('input.yaml', 'w', encoding='utf-8')
yaml.dump(data, file1, Dumper=yaml.RoundTripDumper)
file1.close()



'''

