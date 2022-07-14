import threading
import os
import platform
import random
import queue
import threading
import subprocess

############### 一、需要设置相关的文件夹路径 ###############
exe_path = "../x64/Release/SDK.exe"
instance_path = "../Deploy/Instance/"
solution_path = "../Deploy/Solution/"
log_path = "../Deploy/Solution/log.csv"
############### 二、需要设置程序的运行时限(s) ################
timeout = 10
#########################################################
#########################################################

help = """
1. 所有样例测1遍         python test.py all
2. 源程序跑10遍          python test.py 10
3. 单个样例测1遍         python test.py instance
4. 单个样例测10遍        python test.py instance 10
5. 所有样例每个测10遍    python test.py all 10
"""
# instance_name  time  delta
all_instances2 = [
("inf-roadNet-CA.txt",1500,1),
("inf-roadNet-PA.txt",1500,1),
("sc-msdoor.txt",1000,1),
("sc-nasasrb.txt",1000,0),
("sc-pkustk11.txt",1000,0),
("sc-pkustk13.txt",1000,0),
("sc-pwtk.txt",1000,1),
("sc-shipsec1.txt",1000,1),
("sc-shipsec5.txt",1000,1),
("soc-FourSquare.txt",200,1),
("soc-LiveMocha.txt",1000,1),
("soc-buzznet.txt",1000,4),
("soc-delicious.txt",1000,4),
("soc-digg.txt",500,1),
("soc-flickr.txt",1000,1),
("soc-flixster.txt",1000,1),
("soc-livejournal.txt",1000,1),
("soc-pokec.txt",1000,1),
("soc-youtube-snap.txt",1000,1),
("socfb-A-anon.txt",1000,1),
("socfb-B-anon.txt",1000,1),
("socfb-Berkeley13.txt",1000,1),
("socfb-Duke14.txt",500,1),
("socfb-Indiana.txt",1000,1),
("socfb-OR.txt",1000,1),
("socfb-Penn94.txt",1000,1),
("socfb-Stanford3.txt",200,1),
("socfb-Texas84.txt",1000,1),
("socfb-UCLA.txt",1000,1),
]

all_instances3 = [
("socfb-UCSB37.txt",1000,1),
("socfb-UConn.txt",1000,2),
("socfb-UF.txt",1000,1),
("socfb-UIllinois.txt",1000,3),
("socfb-Wisconsin87.txt",1000,1),
("tech-RL-caida.txt",1000,10),
("tech-as-skitter.txt",1000,1),
("web-BerkStan.txt",500,1),
("web-arabic-2005.txt",1000,4),
("web-it-2004.txt",1000,1),
("web-sk-2005.txt",500,1),
("web-wikipedia2009.txt",1000,1),
]

all_instances1 = [
    ("tech-as-skitter.txt",1000,1),
    ("web-BerkStan.txt", 1000, 1),
    ("web-arabic-2005.txt", 1000, 4),
    ("web-wikipedia2009.txt", 1000, 1),
]

# 加载所有的样例
def load_all_instance():
    # print(len(all_instances))
    return all_instances1



# # exename instance_name, delta, randseed, timeout
# # 运行单个样例  exe instance_name randseed  timeout    
# def run_single_instance_with_times(file_name, time, delta, times = 1):
#     # 注意设置一下程序运行时间
#     for i in range(times):
#         r = random.randint(0, 65536)
#         order = exe_path + ' ' + file_name[:file_name.rfind('.')] + ' ' + str(r) + ' ' + str(load_all_instance()[file_name]) \
#         + ' < ' + instance_path + file_name + ' > ' + solution_path + file_name
#         print(file_name, "\t第" + str(i + 1) + "次运行")
#         os.system(order)

# # # 运行所有的样例  exe timeout seed  instance_name
# # def run_all_instance_with_times(times = 1):
# #     for instance in load_all_instance():
# #         run_single_instance_with_times(instance, times)

# # 运行所有的样例  exe timeout seed  instance_name
# def run_all_instance_with_times(times = 1):
#     for instance, time, delta in load_all_instance():
#         run_single_instance_with_times(instance, time, delta, times)


# # 只运行可执行程序 exe timeout seed
# def run_exe_times(times = 1):
#     for i in range(times):
#         r = random.randint(0, 65536)
#         print("第" + str(i + 1) + "次")
#         order = exe_path
#         os.system(order)


def check_platform():
    global exe_path, instance_path, solution_path, log_path
    plat = platform.system()
    if plat == 'Windows':
        exe_path = exe_path.replace('/', '\\')
        instance_path = instance_path.replace('/', '\\')
        solution_path = solution_path.replace('/', '\\')
        log_path = log_path.replace('/', '\\')

# exename instance_name, delta, randseed, timeout
def make_jobs():
    for file_name, time, delta in load_all_instance():
        for i in range(10):
            r = random.randint(0, 65536)
            order = exe_path + ' ' + file_name[:file_name.rfind('.')] + ' ' + str(delta) + ' ' + str(r) + ' ' + str(time) \
            + ' < ' + instance_path + file_name
            jobs.put((file_name, i + 1, order))


def run_job(name, jobs, logs):
    while True:
        if not jobs.empty():
            instance_name, times, order = jobs.get()
            print(name, instance_name, times)
            ret = subprocess.run(order, stderr=subprocess.PIPE, stdout=subprocess.PIPE, shell=True, encoding='gbk')
            logs.put(ret.stderr)
        else:
            break
    print(name, '运行结束')

if __name__ == '__main__':
    check_platform() # 注意linux和windows差异

    jobs = queue.Queue()
    logs = queue.Queue()
    

    pool = 10
    threads = []
    make_jobs()
    for i in range(pool):
        thread = threading.Thread(target=run_job, args=('线程' + str(i + 1), jobs, logs))
        thread.start()
        threads.append(thread)
    for thread in threads:
        thread.join()
    with open(log_path, 'a') as file:
        while logs.qsize():
            file.writelines(logs.get())
    # if len(sys.argv) == 2:  # 测试一遍
    #     if sys.argv[1] == 'all':
    #         run_all_instance_with_times()       # ./test.py all
    #     elif sys.argv[1].isdigit():
    #         run_exe_times(int(sys.argv[1]))     # ./test.py 10
    #     else:
    #         run_single_instance_with_times(sys.argv[1])     # ./test.py instance_name
    # elif len(sys.argv) == 3: # 测试 times 遍
    #     if sys.argv[1] == 'all':
    #         run_all_instance_with_times(int(sys.argv[2]))   # ./test.py all 10
    #     else:
    #         run_single_instance_with_times(sys.argv[1], int(sys.argv[2]))   # ./test.py instance_name 10
    # else:
    #     print(help)
