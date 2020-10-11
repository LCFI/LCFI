import struct
import sys
import os
import subprocess
from ruamel import yaml


LLVM_FILE = "llfi/demo-llfi_index.ll"
in_main = False
varible_init, varible_name, function_name, in_main, varible_location


def config(i):
    global varible_init, varible_name, function_name, in_main, varible_location

    file = open('set.yaml', 'r', encoding="utf-8")
    file_data = file.read()
    file.close()

    data = yaml.safe_load(file_data)
    varible_num = data[varible_num]

    if data[option][i][function_name] == "main":
        in_main = True
    else:
        in_main = False

    function_name = data[option][i][function_name]
    varible_name = data[option][i][varible_name]
    varible_init = bool(data[option][i][varible_init])
    varible_location = int(data[option][i][varible_location])


def detectFormain(varible_name):
    file = open(LLVM_FILE, 'r')


def detectForinit(varible_name, function_name):
    file = open(LLVM_FILE, 'r')
    for line in file:
        if "}" in line:
            con = False
        if line[0] != ';' and line[0] != '\n' and con == True:
            print(line[:-1])

        if "define" in line and varible_name in line:
            con = True
    file.close()


def detectForNotinit(varible_name, function_name):
    file = open(LLVM_FILE, 'r')
    file.close()


def main(args):
    global varible_init, varible_name, function_name, in_main, varible_location
    # parseArgs(args)
    varible_num = args[0]

    for i in range(int(varible_num)):
        config(i)
        print()
        if in_main == True:
            detectFormain(varible_name)
        else:
            if varible_init == True:
                detectForinit(varible_name, function_name)
            else:
                detectForNotinit(varible_name, function_name)


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
