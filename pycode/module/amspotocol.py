#!/usr/bin/python
# -*- coding: UTF-8 -*-    
try:
    import sqlite3 
    from enum import Enum 
    from sqlite3 import Error
    from datetime import datetime
    from json import JSONDecodeError,JSONDecoder,JSONEncoder 
    from module import common as comm
    from module import gtcsdata as gdt
    # from functools import singledispatch
except Exception as e :
    print(e)
    
#region Sqlite data base manager.
class AmsGtcsProtocol():    
    #region CMD Protocol.
    # CMD300,
    __CMD_300 = {                                                       
        "str1"             :"CMD300",                                   # Header+DATA
        "str2"             :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"             :"0",                                        # check sum ,4 chars
        "str4"             :"0",                                        # Command_sn
    }
    
    # CMD301, 條碼切換工序
    __CMD_301 = {          # {CMDstr1,str2,str3,str4}				                                     
        "str1"             :"CMD301",                                   # Header+DATA
        "str2"             :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"             :"0",                                        # check sum ,4 chars
        "str4"             :"0",                                        # Command_sn
        "str5"             :"0",                                        # Job ID
    }
    
    # CMD302, 工具控制命令
    __CMD_302 = {          # {CMDstr1,str2,str3,str4}                                             
        "str1"             :"CMD302",                                   # Header+DATA
        "str2"             :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"             :"0",                                        # check sum ,4 chars
        "str4"             :"0",                                        # Command_sn
        "str5"             :"0",                                        # Tool control 
    }

    # CMD303, 跳工序   
    __CMD_303 = {          # {CMDstr1,str2,str3}				      
        "str1"             :"CMD303",                                   # Header+DATA
        "str2"             :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"             :"0",                                        # check sum ,4 chars
        "str4"             :"0",                                        # Command_sn
    }

    # CMD310, 設定Normal Torque
    # {CMDstr1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14}	
    __CMD_310 = {    			                                       
        "str1"             :"CMD310",                                    # Header+DATA
        "str2"             :datetime.now().strftime("%Y%m%d %H:%M:%S"),  # yyyyMMdd HH:mm:ss
        "str3"             :"0",                                         # check sum ,4 chars
        "str4"             :"0",                                         # Command_sn
        "str5"             :"0",                                         # Direction
        "str6"             :"0",                                         # RPM
        "str7"             :"0",                                         # Target Torque
        "str8"             :"0",                                         # Torque threshold
        "str9"             :"0",                                         # Joint offset
        "str10"            :"0",                                         # Monitoring mode
        "str11"            :"0",                                         # Torque window
        "str12"            :"0",                                         # High torque
        "str13"            :"0",                                         # Low torque
        "str14"            :"0",                                         # High angle
        "str15"            :"0",                                         # Low angle
    }

    # CMD311, 設定Normal Angle
    # {CMDstr1,str2,str3,str4,str5,str6}
    __CMD_311 = {                                                        
        "str1"             :"CMD311",                                    # Header+DATA
        "str2"             :datetime.now().strftime("%Y%m%d %H:%M:%S"),  # yyyyMMdd HH:mm:ss
        "str3"             :"0",                                         # check sum ,4 chars
        "str4"             :"0",                                         # Command_sn
        "str5"             :"0",                                         # Direction 
        "str6"             :"0",                                         # RPM
        "str7"             :"0",                                         # Target angle
        "str8"             :"0",                                         # Angle threshold
        "str9"             :"0",                                         # Monitoring mode
        "str10"            :"0",                                         # Angle window
        "str11"            :"0",                                         # High torque
        "str12"            :"0",                                         # Low torque
        "str13"            :"0",                                         # High angle
        "str14"            :"0",                                         # Low angle
    }

    # CMD312, 設定Normal Reverse
    # {CMDstr1,str2,str3,str4,str5,str6}
    __CMD_312 = {   				                                                     
        "str1"             :"CMD312",                                   # Header+DATA
        "str2"             :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"             :"0",                                        # check sum ,4 chars
        "str4"             :"0",                                        # Command_sn
        "str5"             :"0",                                        # RPM 
        "str6"             :"0",                                        # Force
        "str7"             :"0",                                        # Direction
    }

    # CMD320, 設定Advanced Torque
    # {CMDstr1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15,str16,str17}				                                                        
    __CMD_320 = { 
        "str1"              :"CMD320",                                   # Header+DATA
        "str2"              :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"              :"0",                                        # check sum ,4 chars
        "str4"              :"0",                                        # Command_sn
        "str5"              :"0",                                        # Step id 
        "str6"              :"0",                                        # Step name
        "str7"              :"0",                                        # Direction
        "str8"              :"0",                                        # RPM
        "str9"              :"0",                                        # Delay time
        "str10"             :"0",                                        # Target torque
        "str11"             :"0",                                        # Torque threshold
        "str12"             :"0",                                        # Joint offset
        "str13"             :"0",                                        # Monitoring mode
        "str14"             :"0",                                        # Torque window
        "str15"             :"0",                                        # High torque
        "str16"             :"0",                                        # Low torque
        "str17"             :"0",                                        # High angle
        "str18"             :"0",                                        # Low angle
    }

    # CMD321, 設定Advanced Angle
    # {CMDstr1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15,str16,str17}				
    __CMD_321 = {                                                        
        "str1"              :"CMD321",                                   # Header+DATA
        "str2"              :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"              :"0",                                        # check sum ,4 chars
        "str4"              :"0",                                        # Command_sn
        "str5"              :"0",                                        # Step id 
        "str6"              :"0",                                        # Step name
        "str7"              :"0",                                        # Direction
        "str8"              :"0",                                        # RPM
        "str9"              :"0",                                        # Delay time
        "str10"             :"0",                                        # Target torque
        "str11"             :"0",                                        # Angle threshold
        "str12"             :"0",                                        # Monitoring mode
        "str13"             :"0",                                        # Angle window
        "str14"             :"0",                                        # High torque
        "str15"             :"0",                                        # Low torque
        "str16"             :"0",                                        # High angle
        "str17"             :"0",                                        # Low angle
    }

    # CMD322, 設定Advanced Revolutions
    # {CMDstr1,str2,str3,str4,str5,str6,str7,str8,str9}				
    __CMD_322 = {                                                        
        "str1"              :"CMD322",                                   # Header+DATA
        "str2"              :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"              :"0",                                        # check sum ,4 chars
        "str4"              :"0",                                        # Command_sn
        "str5"              :"0",                                        # Step id 
        "str6"              :"0",                                        # Step name
        "str7"              :"0",                                        # Direction
        "str8"              :"0",                                        # RPM
        "str9"              :"0",                                        # Delay time
        "str10"             :"0",                                        # Revolutions
    }

    # CMD323, 設定Advanced Time
    # {CMDstr1,str2,str3,str4,str5,str6,str7,str8,str9}				
    __CMD_323 = {                                                        
        "str1"              :"CMD323",                                   # Header+DATA
        "str2"              :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"              :"0",                                        # check sum ,4 chars
        "str4"              :"0",                                        # Command_sn
        "str5"              :"0",                                        # Step id 
        "str6"              :"0",                                        # Step name
        "str7"              :"0",                                        # Direction
        "str8"              :"0",                                        # RPM
        "str9"              :"0",                                        # Delay time
        "str10"             :"0",                                        # Time
    }

    # CMD330, 設定Program
    # {CMDstr1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15}				
    __CMD_330 = {                                                        
        "str1"              :"CMD330",                                   # Header+DATA
        "str2"              :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"              :"0",                                        # check sum ,4 chars
        "str4"              :"0",                                        # Command_sn
        "str5"              :"0",                                        # Program id 
        "str6"              :"0",                                        # Program name
        "str7"              :"0",                                        # Step1
        "str8"              :"0",                                        # Step2
        "str9"              :"0",                                        # Step3
        "str10"             :"0",                                        # Step4
        "str11"             :"0",                                        # Step5
        "str12"             :"0",                                        # Step6 
        "str13"             :"0",                                        # Step7
        "str14"             :"0",                                        # Step8
        "str15"             :"0",                                        # Ok time
        "str16"             :"0",                                        # NG stop
    }

    # CMD340, 設定Initial
    # {CMDstr1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15,str16,str17}				
    __CMD_340 = {                                                        
        "str1"              :"CMD340",                                   # Header+DATA
        "str2"              :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"              :"0",                                        # check sum ,4 chars
        
        "str4"              :"0",                                        # Command_sn
        "str5"              :"0",                                        # Min temperature 
        "str6"              :"0",                                        # Max temperature
        "str7"              :"0",                                        # Max current
        "str8"              :"0",                                        # Max peak current
        "str9"              :"0",                                        # torque sensor type
        "str10"             :"0",                                        # Max duty cycle
        "str11"             :"0",                                        # Max torque
        "str12"             :"0",                                        # PWM frequency 
        "str13"             :"0",                                        # Max rpm
        "str14"             :"0",                                        # Max slope
        "str15"             :"0",                                        # Min bus voltage
        "str16"             :"0",                                        # Max bus voltage
        "str17"             :"0",                                        # Start duty cycle
        "str18"             :"0",                                        # Gear box ratio
        "str19"             :"0",                                        # Start input source
        "str20"             :"0",                                        # Reverse ipnut source
        "str21"             :"0",                                        # Reverse rpm
        "str22"             :"0",                                        # Reverse slope
        "str23"             :"0",                                        # Reverse max current
        "str24"             :"0",                                        # Reverse max torque
        "str25"             :"0",                                        # Error idle time
        "str26"             :"0",                                        # Bachlash 
        "str27"             :"0",                                        # Proportional gain
        "str28"             :"0",                                        # Integral gain
        "str29"             :"0",                                        # Encoder
    }
    #endregion

    #region ANS Protocol
    # ANS301, 回復條碼切換工序
    # {ANSstr1,str2,str3,str4}				
    __ANS_301 = {                                                       
        "str1"             :"ANS301",                                   # Header+DATA
        "str2"             :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"             :"0",                                        # check sum ,4 chars
        "str4"             :"0",                                        # Command_sn
        "str5"             :"0",                                        # Job ID
    }

    # ANS302, 回復工具控制命令
    # {ANSstr1,str2,str3,str4}
    __ANS_302 = {
        "str1"             :"ANS302",                                   # Header+DATA
        "str2"             :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"             :"0",                                        # check sum ,4 chars
        "str4"             :"0",                                        # Command_sn
        "str5"             :"0",                                        # Tool control 
    }

    # ANS310, 回復Normal Torque
    # {ANSstr1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14}
    __ANS_310 = {
        "str1"             :"ANS310",                                    # Header+DATA
        "str2"             :datetime.now().strftime("%Y%m%d %H:%M:%S"),  # yyyyMMdd HH:mm:ss
        "str3"             :"0",                                         # check sum ,4 chars
        "str4"             :"0",                                         # Command_sn
        "str5"             :"0",                                         # Direction
        "str6"             :"0",                                         # RPM
        "str7"             :"0",                                         # Target Torque
        "str8"             :"0",                                         # Torque threshold
        "str9"             :"0",                                         # Joint offset
        "str10"            :"0",                                         # Monitoring mode
        "str11"            :"0",                                         # Torque window
        "str12"            :"0",                                         # High torque
        "str13"            :"0",                                         # Low torque
        "str14"            :"0",                                         # High angle
        "str15"            :"0",                                          # Low angle
    }

    # ANS311, 回復Normal Angle
    # {CMDstr1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13}
    __ANS_311 = {                                                        
        "str1"             :"ANS311",                                    # Header+DATA
        "str2"             :datetime.now().strftime("%Y%m%d %H:%M:%S"),  # yyyyMMdd HH:mm:ss
        "str3"             :"0",                                         # check sum ,4 chars
        "str4"             :"0",                                         # Command_sn
        "str5"             :"0",                                         # Direction 
        "str6"             :"0",                                         # RPM
        "str7"             :"0",                                         # Target angle
        "str8"             :"0",                                         # Angle threshold
        "str9"             :"0",                                         # Monitoring mode
        "str10"            :"0",                                         # Angle window
        "str11"            :"0",                                         # High torque
        "str12"            :"0",                                         # Low torque
        "str13"            :"0",                                         # High angle
        "str14"            :"0",                                         # Low angle
    }

    # ANS312, 回復Normal Reverse
    # {ANSstr1,str2,str3,str4,str5,str6}
    __ANS_312 = {   				                                                     
        "str1"             :"ANS312",                                   # Header+DATA
        "str2"             :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"             :"0",                                        # check sum ,4 chars
        "str4"             :"0",                                        # Command_sn
        "str5"             :"0",                                        # RPM 
        "str6"             :"0",                                        # Force
        "str7"             :"0",                                        # Direction
    }

    # ANS320, 回復Advanced Torque
    # {ANSstr1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15,str16,str17}			                                                        
    __ANS_320 = { 
        "str1"              :"ANS320",                                   # Header+DATA
        "str2"              :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"              :"0",                                        # check sum ,4 chars
        "str4"              :"0",                                        # Command_sn
        "str5"              :"0",                                        # Step id 
        "str6"              :"0",                                        # Step name
        "str7"              :"0",                                        # Direction
        "str8"              :"0",                                        # RPM
        "str9"              :"0",                                        # Delay time
        "str10"             :"0",                                        # Target torque
        "str11"             :"0",                                        # Torque threshold
        "str12"             :"0",                                        # Joint offset
        "str13"             :"0",                                        # Monitoring mode
        "str14"             :"0",                                        # Torque window
        "str15"             :"0",                                        # High torque
        "str16"             :"0",                                        # Low torque
        "str17"             :"0",                                        # High angle
        "str18"             :"0",                                        # Low angle
    }

    # ANS321, 回復Advanced Angle
    # {ANSstr1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15,str16}			
    __ANS_321 = {                                                        
        "str1"              :"ANS321",                                   # Header+DATA
        "str2"              :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"              :"0",                                        # check sum ,4 chars
        "str4"              :"0",                                        # Command_sn
        "str5"              :"0",                                        # Step id 
        "str6"              :"0",                                        # Step name
        "str7"              :"0",                                        # Direction
        "str8"              :"0",                                        # RPM
        "str9"              :"0",                                        # Delay time
        "str10"             :"0",                                        # Target torque
        "str11"             :"0",                                        # Angle threshold
        "str12"             :"0",                                        # Monitoring mode
        "str13"             :"0",                                        # Angle window
        "str14"             :"0",                                        # High torque
        "str15"             :"0",                                        # Low torque
        "str16"             :"0",                                        # High angle
        "str17"             :"0",                                        # Low angle
    }

    # ANS322, 回復Advanced Revolutions
    # {ANSstr1,str2,str3,str4,str5,str6,str7,str8,str9}			
    __ANS_322 = {                                                        
        "str1"              :"ANS322",                                   # Header+DATA
        "str2"              :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"              :"0",                                        # check sum ,4 chars
        "str4"              :"0",                                        # Command_sn
        "str5"              :"0",                                        # Step id 
        "str6"              :"0",                                        # Step name
        "str7"              :"0",                                        # Direction
        "str8"              :"0",                                        # RPM
        "str9"              :"0",                                        # Delay time
        "str10"             :"0",                                        # Revolutions
    }

    # ANS323, 回復Advanced Time
    # {ANSstr1,str2,str3,str4,str5,str6,str7,str8,str9}			
    __ANS_323 = {                                                        
        "str1"              :"ANS323",                                   # Header+DATA
        "str2"              :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"              :"0",                                        # check sum ,4 chars
        "str4"              :"0",                                        # Command_sn
        "str5"              :"0",                                        # Step id 
        "str6"              :"0",                                        # Step name
        "str7"              :"0",                                        # Direction
        "str8"              :"0",                                        # RPM
        "str9"              :"0",                                        # Delay time
        "str10"             :"0",                                        # Time
    }

    # ANS330, 回復Program
    # {ANSstr1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15}			
    __ANS_330 = {                                                        
        "str1"              :"ANS330",                                   # Header+DATA
        "str2"              :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"              :"0",                                        # check sum ,4 chars
        "str4"              :"0",                                        # Command_sn
        "str5"              :"0",                                        # Program id 
        "str6"              :"0",                                        # Program name
        "str7"              :"0",                                        # Step1
        "str8"              :"0",                                        # Step2
        "str9"              :"0",                                        # Step3
        "str10"             :"0",                                        # Step4
        "str11"             :"0",                                        # Step5
        "str12"             :"0",                                        # Step6 
        "str13"             :"0",                                        # Step7
        "str14"             :"0",                                        # Step8
        "str15"             :"0",                                        # Ok time
        "str16"             :"0",                                        # NG stop
    }

    # ANS340, 回復Initial
    # {ANSstr1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15,str16,str17}				
    __ANS_340 = {                                                        
        "str1"              :"ANS340",                                   # Header+DATA
        "str2"              :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"              :"0",                                        # check sum ,4 chars
        "str4"              :"0",                                        # Command_sn
        "str5"              :"0",                                        # Min temperature 
        "str6"              :"0",                                        # Max temperature
        "str7"              :"0",                                        # Max current
        "str8"              :"0",                                        # Max peak current
        "str9"              :"0",                                        # torque sensor type
        "str10"             :"0",                                        # Max duty cycle
        "str11"             :"0",                                        # Max torque
        "str12"             :"0",                                        # PWM frequency 
        "str13"             :"0",                                        # Max rpm
        "str14"             :"0",                                        # Max slope
        "str15"             :"0",                                        # Min bus voltage
        "str16"             :"0",                                        # Max bus voltage
        "str17"             :"0",                                        # Start duty cycle
        "str18"             :"0",                                        # Gear box ratio
        "str19"             :"0",                                        # Start input source
        "str20"             :"0",                                        # Reverse ipnut source
        "str21"             :"0",                                        # Reverse rpm
        "str22"             :"0",                                        # Reverse slope
        "str23"             :"0",                                        # Reverse max current
        "str24"             :"0",                                        # Reverse max torque
        "str25"             :"0",                                        # Error idle time
        "str26"             :"0",                                        # Bachlash 
        "str27"             :"0",                                        # Proportional gain
        "str28"             :"0",                                        # Integral gain
        "str29"             :"0",                                        # Encoder
    }
    #endregion

    #region REQ Protocol
    # REQ300, 保持連線
    # {REQstr1,str2,str3}
    __REQ_300 = {                                                       
        "str1"             :"REQ300",                                   # Header+DATA
        "str2"             :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"             :"0",                                        # check sum ,4 chars
        "str4"             :"0",                                        # Command_sn
    }
    # REQ301,開機讀取MCB參數狀態
    __REQ_301 = {                                                       
        "str1"              :"REQ301",                                   # Header+DATA
        "str2"              :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        "str3"              :"0",                                        # check sum ,4 chars        
        "str4"              :"0",                                        # Command_sn
        "str5"              :"0",                                        # Min temperature 
        "str6"              :"0",                                        # Max temperature
        "str7"              :"0",                                        # Max current
        "str8"              :"0",                                        # Max peak current
        "str9"              :"0",                                        # torque sensor type
        "str10"             :"0",                                        # Max duty cycle
        "str11"             :"0",                                        # Max torque
        "str12"             :"0",                                        # PWM frequency 
        "str13"             :"0",                                        # Max rpm
        "str14"             :"0",                                        # Max slope
        "str15"             :"0",                                        # Min bus voltage
        "str16"             :"0",                                        # Max bus voltage
        "str17"             :"0",                                        # Start duty cycle
        "str18"             :"0",                                        # Gear box ratio
        "str19"             :"0",                                        # Start input source
        "str20"             :"0",                                        # Reverse ipnut source
        "str21"             :"0",                                        # Reverse rpm
        "str22"             :"0",                                        # Reverse slope
        "str23"             :"0",                                        # Reverse max current
        "str24"             :"0",                                        # Reverse max torque
        "str25"             :"0",                                        # Error idle time
        "str26"             :"0",                                        # Bachlash 
        "str27"             :"0",                                        # Proportional gain
        "str28"             :"0",                                        # Integral gain
        "str29"             :"0",                                        # Encoder
    }
    
    # REQ302, 自動學習連線 
    # {REQstr1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12}
    __REQ_302 = {                                                       
        gdt.REQ302.HEADER.value      :"REQ302",                                   # Header+DATA
        gdt.REQ302.TIME.value        :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        gdt.REQ302.CHECH_SUM.value   :"0",                                        # check sum ,4 chars        
        gdt.REQ302.CMD_SN.value      :"0",                                        # Command_sn
        gdt.REQ302.TRG_TRQ.value     :"0",                                        # Target Torque 
        gdt.REQ302.MAX_CURR.value    :"0",                                        # Max Current
        gdt.REQ302.ANGLE_SL.value    :"0",                                        # Angle Selflearning
        gdt.REQ302.SL_ANGLE_MON.value:"0",                                        # SI Angle Monitoring
        gdt.REQ302.SL_ANGLE_POS.value:"0",                                        # SI Angle Positioning
        gdt.REQ302.SL_WART_STR.value :"0",                                        # Sl Wait Start
        gdt.REQ302.SL_TURN_BACK.value:"0",                                        # SI Turn Back
        gdt.REQ302.SL_TURN_BACK.value:"0",                                        # Count
    }
    #endregion

    #region DATA Protocol
    # DATA300, 鎖附資料
    # {DATAstr1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15,str16,str17,str18,str19,str20,str21}
    __DATA_300 = {
        gdt.DATA300.HEADER.value    :"DATA300",                                  # str1:Header+DATA
        gdt.DATA300.TIME.value      :datetime.now().strftime("%Y%m%d %H:%M:%S"), # str2:yyyyMMdd HH:mm:ss
        gdt.DATA300.CHECH_SUM.value :"0",                                        # str3:check sum ,4 chars
        gdt.DATA300.CMD_SN.value    :"0",                                        # str4:Command_sn
        gdt.DATA300.DEV_TYPE.value  :"6",                                        # str5:Device type
        gdt.DATA300.TOOL_SN.value   :"0",                                        # str6:Tool SN
        gdt.DATA300.DEV_SN.value    :"0",                                        # str7:Device SN
        gdt.DATA300.JOB_ID.value    :"0",                                        # str8:Job ID
        gdt.DATA300.SEQ_ID.value    :"0",                                        # str9:Sequence ID
        gdt.DATA300.PRG_ID.value    :"0",                                        # str10:Program ID
        gdt.DATA300.STEP_ID.value   :"0",                                        # str11:Step ID
        gdt.DATA300.DIRECTION.value :"0",                                        # str12:Direction
        gdt.DATA300.UNIT.value      :"0",                                        # str13:Torque unit
        gdt.DATA300.INC_DEC.value   :"0",                                        # str14:INC/DEC
        gdt.DATA300.LAST_CNT.value  :"0",                                        # str15:Last_screw_count
        gdt.DATA300.MAX_CNT.value   :"0",                                        # str16:Max_screw_count
        gdt.DATA300.FASTEN_TR.value :"0",                                        # str17:Fastening time
        gdt.DATA300.TORQUE.value    :"0",                                        # str18:Torque
        gdt.DATA300.ANGLE.value     :"0",                                        # str19:Angle
        gdt.DATA300.MAX_TRQ.value   :"0",                                        # str20:Max Torque
        gdt.DATA300.REVOLUTION.value:"0",                                        # str21:Revolutions
        gdt.DATA300.STATUS.value    :"0",                                        # str22:Status
        gdt.DATA300.GPIO_IN.value   :"0",                                        # str23:Inputio
        gdt.DATA300.GPIO_OUT.value  :"0",                                        # str24:Outputio
        gdt.DATA300.ERR_MSG.value   :"0",                                        # str25:Error Masseage
        gdt.DATA300.TOOL_CNT.value  :"0",                                        # str26:Tool Count
        gdt.DATA300.TOOL_RPM.value  :"0",                                        # str27:RPM
    }
    # DATA302, 自動學習鎖附資料 
    # {DATAstr1,str2,str3,str4}
    __DATA_302 = {                                                       
        gdt.DATA302.HEADER.value    :"DATA302",                                   # Header+DATA
        gdt.DATA302.TIME.value      :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
        gdt.DATA302.CHECH_SUM.value :"0",                                        # check sum ,4 chars        
        gdt.DATA302.CMD_SN.value    :"0",                                        # Command_sn
        gdt.DATA302.STATUS.value    :"0",                                        #　status
    }
    #endregion

    #region dict 
    # AMS CMD Protocol.
    __dict={
        # AMS / DAS  to  Controller.
        "CMD300":__CMD_300,             # CMD300, 回復確認.
        "CMD301":__CMD_301,             # CMD301, 條碼切換工序.
        "CMD302":__CMD_302,             # CMD302, 工具控制命令.
        "CMD303":__CMD_303,             # CMD303, 跳工序.
        "CMD310":__CMD_310,             # CMD310, 設定Normal Torque.
        "CMD311":__CMD_311,             # CMD311, 設定Normal Angle.
        "CMD312":__CMD_312,             # CMD312, 設定Normal Reverse.
        "CMD320":__CMD_320,             # CMD320, 設定Advanced Torque.
        "CMD321":__CMD_321,             # CMD321, 設定Advanced Angle.
        "CMD322":__CMD_322,             # CMD322, 設定Advanced Revolutions.
        "CMD323":__CMD_323,             # CMD323, 設定Advanced Time.
        "CMD330":__CMD_330,             # CMD330, 設定Program.
        "CMD340":__CMD_340,             # CMD340, 設定Initial.

        # Controller  to  AMS / DAS.
        "ANS301":__ANS_301,             # ANS301, 回復條碼切換工序.
        "ANS302":__ANS_302,             # ANS302, 回復工具控制命令. 
        "ANS310":__ANS_310,             # ANS310, 回復Normal Torque.
        "ANS311":__ANS_311,             # ANS311, 回復Normal Angle.
        "ANS312":__ANS_312,             # ANS312, 回復Normal Reverse.
        "ANS320":__ANS_320,             # ANS320, 回復Advanced Torque.
        "ANS321":__ANS_321,             # ANS321, 回復Advanced Angle.
        "ANS322":__ANS_322,             # ANS322, 回復Advanced Revolutions.
        "ANS323":__ANS_323,             # ANS323, 回復Advanced Time.
        "ANS330":__ANS_330,             # ANS330, 回復Program.
        "ANS340":__ANS_340,             # ANS340, 回復Initial.
        
        # Controller  to  AMS / DAS.
        "REQ300" :__REQ_300,            # REQ300, 保持連線.
        "REQ301" :__REQ_301,            # REQ301, 開機啟動MCB狀態.
        "REQ302" :__REQ_302,            # REQ302, 自動學習連線

        # Controller  to  AMS / DAS
        "DATA300":__DATA_300,           # DATA300, 鎖附資料
        "DATA302":__DATA_302,           # DATA300, 鎖附資料
    }    
    #
    
    # Set dictionary json.        
    __dict_json = dict()
    # Conmmand serial number.
    __cmd_sn = 0  
    # Initial JSON obj.
    __JE = JSONEncoder()
    __JD = JSONDecoder()
    __value_list = list(__dict.values())
    __key_list   = list(__dict.keys())
    #endregion

    # Initial signleTon object.
    _instance = None 
    def __new__(cls, *args, **kwargs): 
        if cls._instance is None: 
            cls._instance = super().__new__(cls) 
        return cls._instance  
    # Initial object value.
    def __init__(self): pass

    # Calcualate check sum.
    def calculate_check_sum(self,pstr:str)->str: # 
        _value = 0
        for val in pstr:
            _value += ord(val)
        return str(hex(_value))[2:].zfill(4).upper() # 我懶得寫註解,請先讀完python再來看這行

    # Get command serial number.
    def get_command_sn(self)->int:
        if self.__cmd_sn<255:
            self.__cmd_sn +=1
        else:
            self.__cmd_sn = 0
        return self.__cmd_sn
    
    # Get sub dictionary keys.
    def get_sub_keys(self,key:str)->object:
        _keys = list(self.__dict.get(key).keys())    
        return _keys 

    # Get dictionary.
    def get_all_dict(self)->dict:
        return self.__dict      

    # Get dictionary obj key list.
    def get_all_dict_keys(self)->list:
        return self.__key_list
    
    # Get dictionary.
    def get_all_dict_values(self)->list:
        return self.__value_list      
    
    # Get dictionary.
    def get_sub_dict_keys(self,key:str)->list:
        _keys_list = list(self.__dict.get(key).keys())
        return _keys_list
    
    # Get sub dictionary.
    def get_sub_dict(self,key:str)->dict:
        return self.__dict.get(key)
    
    # Set sub dictionary.
    def set_sub_dict(self,key:str,pdict:dict):
        self.__dict.update({key:pdict})

    # Channg_sud_dict()
    def set_opposite_dict(self,pdict:dict,okey:str):
        _source_dict = pdict
        _source_dict["str1"] = okey
        self.set_sub_dict(key=okey,pdict=_source_dict)    # Read source dict.
        
#endregion
