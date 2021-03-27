import os

def alter(file,old_str,new_str):
    """
    :param file:filename
    :param old_str:old string
    :param new_str:new string
    :return:
    """
    file_data =""
    with open(file, "r", encoding="utf-8") as f:
        for line in f:
            if old_str in line:
                line = line.replace(old_str,new_str)
            file_data += line
    with open(file,"w",encoding="utf-8") as f:
        f.write(file_data)

#os.system("sudo -u")
alter("/etc/X11/xorg.conf", "UD", "CCW")
os.system("reboot")