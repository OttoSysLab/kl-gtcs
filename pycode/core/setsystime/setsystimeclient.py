import os 
import socket

# 設定連線server的IP與PORT
IP =  "192.168.0.150"                                    # 設定連線到Otto的桌機
PORT = 9000                                              # 設定Port=9000
_cmd_str = "Get me fucking time!!!"                      # 設定傳送字串

# socket發送流程
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # 宣告socket物件
sock.connect((IP, PORT))                                 # 連線server
sock.send(_cmd_str.encode('utf-8'))                      # 傳送字串 
_sock_recv = sock.recv(1024).decode('utf-8')
sock.close()
# 設定系統時間
_time_str =_sock_recv.replace("_"," ")                   # 使用空白鍵取代下底線
print("Get fuck time = '%s'"%(_time_str))                # 印出汰換後的系統時間
os.system("sudo timedatectl set-ntp no")                 # 設定系統時間時不需要pass word
os.system("sudo timedatectl set-time '%s'"%(_time_str))  # 設定目前的系統時間
os.system("sudo timedatectl ")                           # 顯示目前的系統時間
