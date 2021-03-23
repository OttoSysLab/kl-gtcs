import signal
import os
import threading
from time import sleep
def f(a,b):
    print("kill me")
    os.kill(os.getpid(),signal.SIGKILL)

def tf():
    sleep(20)

signal.signal(signal.SIGINT,f)
p=threading.Thread(target=tf)
p.start()
p.join()

print ("done")