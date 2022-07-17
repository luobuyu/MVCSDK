import threading
import multiprocessing
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
############### 二、需要设置多线程还是多进程 ################
multi_threading = True
multi_processing = False
pool = 5        # 线程个数或进程个数
times = 20      # 每个算例执行的次数
############### 三、构造算例 ##############################
# 调用 construct_all_instance(n) 函数可以在控制台打印出所有的算例
# 参数 n 表示除了 timeout,randomseed 之外还需要 n 个参数个数
# 构造出的算例默认 timeout=1000,randomseed在实际执行时生成,
# 其余的 n 个参数默认为 1,需要的话可以进行调整
# 最后直接将控制台生成的算例放在 all_instances 后就行
# 修改 load_all_instance函数，返回需要测试的算例列表
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
all_instances1 = [
("bio-dmela.txt",20,1),
("bio-yeast.txt",10,1),
("ca-AstroPh.txt",10,1),
("ca-citeseer.txt",20,1),
("ca-coauthors-dblp.txt",500,1),
("ca-CondMat.txt",10,1),
("ca-CSphd.txt",10,1),
("ca-dblp-2010.txt",20,1),
("ca-dblp-2012.txt",20,1),
("ca-Erdos992.txt",10,1),
("ca-GrQc.txt",10,1),
("ca-HepPh.txt",10,1),
("ca-hollywood-2009.txt",1000,1),
("ca-MathSciNet.txt",25,1),
("socfb-A-anon.txt",1000,1),
("socfb-B-anon.txt",1000,1),
("socfb-Berkeley13.txt",1000,1),
("socfb-CMU.txt",500,1),
("socfb-Duke14.txt",500,1),
("socfb-Indiana.txt",1000,1),
("socfb-MIT.txt",300,1),
("socfb-OR.txt",1000,1),
("socfb-Penn94.txt",1000,1),
("socfb-Stanford3.txt",200,1),
("socfb-Texas84.txt",1000,1),
("socfb-UCLA.txt",1000,1),
("socfb-UCSB37.txt",1000,1),
("socfb-UConn.txt",1000,2),
("socfb-UF.txt",1000,1),
("socfb-UIllinois.txt",1000,3),
("socfb-Wisconsin87.txt",1000,1),
("ia-email-EU.txt",10,1),
("ia-email-univ.txt",10,1),
("ia-enron-large.txt",10,1),
("ia-fb-messages.txt",10,1),
("ia-reality.txt",10,1),
("ia-wiki-Talk.txt",10,1),
("inf-power.txt",10,1),
("inf-roadNet-CA.txt",1000,1),
("inf-roadNet-PA.txt",1000,1),
("rec-amazon.txt",50,1),
("sc-ldoor.txt",1000,8),
("sc-msdoor.txt",1000,1),
("sc-nasasrb.txt",1000,0),]

all_instances2 = [
("sc-pkustk11.txt",1000,0),
("sc-pkustk13.txt",1000,0),
("sc-pwtk.txt",1000,1),
("sc-shipsec1.txt",1000,1),
("sc-shipsec5.txt",1000,1),
("soc-BlogCatalog.txt",20,1),
("soc-brightkite.txt",50,1),
("soc-buzznet.txt",1000,4),
("soc-delicious.txt",1000,4),
("soc-digg.txt",500,1),
("soc-douban.txt",10,1),
("soc-epinions.txt",10,1),
("soc-flickr.txt",1000,1),
("soc-flixster.txt",1000,1),
("soc-FourSquare.txt",200,1),
("soc-gowalla.txt",500,1),
("soc-lastfm.txt",20,1),
("soc-livejournal.txt",1000,1),
("soc-LiveMocha.txt",1000,1),
("soc-pokec.txt",1000,1),
("soc-slashdot.txt",10,1),
("soc-twitter-follows.txt",10,1),
("soc-youtube.txt",500,1),
("soc-youtube-snap.txt",1000,1),
("tech-as-caida2007.txt",10,1),
("tech-as-skitter.txt",1000,1),
("tech-internet-as.txt",10,1),
("tech-p2p-gnutella.txt",10,1),
("tech-RL-caida.txt",1000,10),
("tech-routers-rf.txt",10,1),
("tech-WHOIS.txt",10,1),
("web-arabic-2005.txt",1000,4),
("web-BerkStan.txt",500,1),
("web-edu.txt",10,1),
("web-google.txt",10,1),
("web-indochina-2004.txt",10,1),
("web-it-2004.txt",1000,1),
("web-sk-2005.txt",1000,1),
("web-spam.txt",10,1),
("web-uk-2005.txt",10,1),
("web-webbase-2001.txt",10,1),
("web-wikipedia2009.txt",1000,1),
]

all_instances3 = [
    ("sc-shipsec5.txt",10,1),
]

# 调用该函数会得到所有的算例
# 参数 n 表示需要的参数个数，默认值全是 1
# [(instance_name, time, 参数1, 参数2, …, 参数n)]
def construct_all_instance(n):
    instances = os.listdir(instance_path)
    instances.sort()
    print('[')
    for instance in instances:
        print('("' + instance + '", 1000, ' + ('1, ' * n) + ')')
    print(']')


# 加载所有的样例
def load_all_instance():
    # print(len(all_instances))
    return all_instances3


def check_platform():
    global exe_path, instance_path, solution_path, log_path
    plat = platform.system()
    if plat == 'Windows':
        exe_path = exe_path.replace('/', '\\')
        instance_path = instance_path.replace('/', '\\')
        solution_path = solution_path.replace('/', '\\')
        log_path = log_path.replace('/', '\\')

# exename instance_name, timeout, randseed, 参数1, 参数2, 参数n
def make_jobs():
    for item in load_all_instance():
        file_name = item[0]
        # 构造命令，中间留出随机数种子的位置
        # order1: exename instance_name, timeout,
        # order2: randseed
        # order3: 参数1, 参数2, 参数n
        # order4: < instance_path + file_name
        order1 = exe_path + ' ' + file_name[:file_name.rfind('.')] + ' ' + str(item[1])
        order3 = ''
        for i in range(2, len(item)):
            order3 += ' ' + str(item[i])
        for i in range(times):
            r = random.randint(0, 65536)
            order2 = ' ' + str(r)
            order4 = ' < ' + instance_path + file_name
            order = order1 + order2 + order3 + order4
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


def write_log():
    with open(log_path, 'a') as file:
        l = []
        while logs.qsize():
            l.append(logs.get())
        l.sort()
        for log in l:
            file.writelines(log)


if __name__ == '__main__':
    check_platform() # 注意linux和windows差异
    if multi_threading:
        jobs = queue.Queue()
        logs = queue.Queue()
    else:
        jobs = multiprocessing.Queue()
        logs = multiprocessing.Queue()
    
    all_exec = []
    make_jobs()
    for i in range(pool):
        if multi_threading:
            exec = threading.Thread(target=run_job, args=('线程' + str(i + 1), jobs, logs))
        else:
            exec = multiprocessing.Process(target=run_job, args=('进程' + str(i + 1), jobs, logs))
        exec.start()
        all_exec.append(exec)
    for exec in all_exec:
        exec.join()

    write_log()













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
