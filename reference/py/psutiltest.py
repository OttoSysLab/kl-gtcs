import psutil

def get_pid(name):
    '''
     作用：根据进程名获取进程pid
    '''
    # pids = psutil.process_iter()
    # print("[" + name + "]'s pid is:",end="")
    # for pid in pids:
    #     if(pid.name() == name):
    #         print(pid.pid)
    pids = psutil.process_iter()
    for pid in pids:
        print("[%s] = %d" %(pid.name(),pid.pid))

get_pid("gtcsdriver")
