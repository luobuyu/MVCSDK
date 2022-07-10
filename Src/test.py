import os
import platform
import random
from socket import timeout
import sys

############### 一、需要设置相关的文件夹路径 ###############
exe_path = "../x64/Release/SDK.exe"
instance_path = "../Deploy/Instance/"
solution_path = "../Deploy/Solution/"
log_path = "../Deploy/Solution/log.csv"
############### 二、需要设置程序的运行时限(s) ################
timeout = 5
#########################################################
#########################################################

help = """
1. 所有样例测1遍         python test.py all
2. 源程序跑10遍          python test.py 10
3. 单个样例测1遍         python test.py instance
4. 单个样例测10遍        python test.py instance 10
5. 所有样例每个测10遍    python test.py all 10
"""
# 加载所有的样例
def load_all_instance():
    instances = os.listdir(instance_path)
    instances.sort()
    return instances

# 运行单个样例  exe instance_name randseed  timeout    
def run_single_instance_with_times(file_name, times = 1):
    # 注意设置一下程序运行时间
    for i in range(times):
        r = random.randint(0, 65536)
        order = exe_path + ' ' + file_name[:file_name.rfind('.')] + ' ' + str(r) + ' ' + str(timeout) \
        + ' < ' + instance_path + file_name + ' > ' + solution_path + file_name
        print(file_name, "\t第" + str(i + 1) + "次运行")
        os.system(order)

# 运行所有的样例  exe timeout seed  instance_name
def run_all_instance_with_times(times = 1):
    for instance in load_all_instance():
        run_single_instance_with_times(instance, times)


# 只运行可执行程序 exe timeout seed
def run_exe_times(times = 1):
    for i in range(times):
        r = random.randint(0, 65536)
        print("第" + str(i + 1) + "次")
        order = exe_path
        os.system(order)


def check_platform():
    global exe_path, instance_path, solution_path, log_path
    plat = platform.system()
    if plat == 'Windows':
        exe_path = exe_path.replace('/', '\\')
        instance_path = instance_path.replace('/', '\\')
        solution_path = solution_path.replace('/', '\\')
        log_path = log_path.replace('/', '\\')


if __name__ == '__main__':
    check_platform() # 注意linux和windows差异
    if len(sys.argv) == 2:  # 测试一遍
        if sys.argv[1] == 'all':
            run_all_instance_with_times()       # ./test.py all
        elif sys.argv[1].isdigit():
            run_exe_times(int(sys.argv[1]))     # ./test.py 10
        else:
            run_single_instance_with_times(sys.argv[1])     # ./test.py instance_name
    elif len(sys.argv) == 3: # 测试 times 遍
        if sys.argv[1] == 'all':
            run_all_instance_with_times(int(sys.argv[2]))   # ./test.py all 10
        else:
            run_single_instance_with_times(sys.argv[1], int(sys.argv[2]))   # ./test.py instance_name 10
    else:
        print(help)
