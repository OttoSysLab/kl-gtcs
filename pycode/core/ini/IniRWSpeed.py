# Impot lib.
from configparser import ConfigParser 
from json import JSONEncoder,JSONDecoder
from time import sleep,perf_counter_ns 
from platform import system
import os 

_check_sys = system()
if _check_sys == "Windows":
    READ_PATH  = "testread.ini"                       # 請自行修改序列埠名稱
    WRITE_PATH = "testwrite.ini"                      # 請自行修改序列埠名稱
elif _check_sys == 'Linux':
    READ_PATH   = "/mnt/ramdisk/testread.ini"         # 請自行修改序列埠名稱
    WRITE_PATH = "/mnt/ramdisk/testwrite.ini"         # 請自行修改序列埠名稱
    os.system("cp /home/kls/Desktop/tdd/ini/testread.ini /mnt/ramdisk/testread.ini")
    os.system("chmod 777 -R /mnt/ramdisk/testread.ini")
    os.system("cp /home/kls/Desktop/tdd/ini/testwrite.ini /mnt/ramdisk/testwrite.ini")
    os.system("chmod 777 -R /mnt/ramdisk/testwrite.ini")    

# INI
class MyConfigParser(ConfigParser):
    # Initial obj
    def __init__(self,defaults=None):
        ConfigParser.__init__(self,defaults=None)
    def optionxform(self, optionstr):
        return optionstr

class IniDictConverter():    
    #region INI defalut function.
    # Inital obj value.
    def __init__(self):
        pass
    
    # Read data from INI and return list formate.
    def read_INI_list(self,iniPath):
        _ini_lst = []
        cfg = MyConfigParser()
        cfg.read(iniPath)
        _section_lst = cfg.sections()
        # for key in cfg[_section_lst[0]]:_ini_lst.append([key,cfg.get(_section_lst[0],key)])
        for sec in _section_lst:
            for key in cfg[sec]:
               _ini_lst.append([sec,key,cfg.get(sec,key)])
        return _ini_lst
        
    # Read INI to json.
    def read_INI_to_json(self,INIPath):
        _json_str = ''                          # retuur jsion string.
        cfg = MyConfigParser()                  # Initial csv obj.
        cfg.read(INIPath)                       # Read all data from INI.
        _sec_lst = cfg.sections()               # Get section list from INI.
        _cfg_dict = dict.fromkeys(_sec_lst)     # Create a dictionary include all secssion in INI.     
        _JE = JSONEncoder()
        try:
            for sec in _sec_lst:                              #                 
                _key_lst  = cfg[sec].keys()
                _buf_dict = dict.fromkeys(_key_lst)
                for key in _key_lst:
                    try:
                        _value = int(cfg.get(sec,key).replace('"',""))
                    except Exception as e:
                        _value = cfg.get(sec,key).replace('"',"")
                    _buf_dict.update({key:_value})
                # _cfg_dict.update({sec:json.dumps(_buf_dict)})
                _cfg_dict.update({sec:_buf_dict})
            # _json_str = json.dumps(_cfg_dict)
            _json_str = _JE.encode(_cfg_dict)
        except Exception as e:
            print(e)

        return _json_str

    # Convert json to dictionary.
    def convert_json_to_dict(self,jStr):
        _JD = JSONDecoder()
        _dict = _JD.decode(jStr)
        return _dict

    # Convert dictionary to json
    def convert_dict_to_json(self,_dict):
        _JE = JSONEncoder()
        _json = _JE.encode(_dict)
        return _json
    
    #region Write data list to INI.
    def write_json_to_INI(self,INIPath,jStr):
        _write_ok = False                       # retuur jsion string.
        _Jobj = JSONDecoder()              # 
        _ini_dict = _Jobj.decode(jStr)          # 
        _cfg = MyConfigParser()                 # Initial csv obj.
        _cfg.read_dict(_ini_dict)           
        # print(type(_ini_dict)) 
        # Write configparser data to ini file.
        with open(INIPath,'w',encoding='cp950') as configfile:_cfg.write(configfile)           
        return _write_ok
    #endregion
    #endregion

def main_01():
    t1 = perf_counter_ns()
    _ini = IniDictConverter()
    for idx in range(1000):
        _a = _ini.read_INI_to_json(INIPath=READ_PATH)
        _b = _ini.convert_json_to_dict(jStr=_a)
        _c = _ini.convert_dict_to_json(_dict=_b)
        _d = _ini.write_json_to_INI(INIPath=WRITE_PATH,jStr=_c)  
    t2 = perf_counter_ns()
    #sleep(100)
    print("Dict elapsed  = %f (ms)"%((t2-t1)/1000000))

if __name__ == "__main__":
    main_01()
