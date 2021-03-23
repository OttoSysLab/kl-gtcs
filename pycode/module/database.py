#!/usr/bin/python
# -*- coding: UTF-8 -*-    
try:
    import sqlite3 
    from enum import Enum
    from datetime import datetime as dt 
    from sqlite3 import Error 
except Exception as e :
    print(e)
 
#region Sqlite data base manager.
class SqliteManager:
    # Private attribute.
    __timeout = 10
    __dict_data_type = None
    # private
    def __init__(self):pass    

    # Execute sql cmd.
    def execute_sql_cmd(self,db_name,cmd):
        _state = ""
        _conn = None                             # Initial sqlite conn. 
        try:
            _conn = sqlite3.connect(db_name,self.__timeout)
            _cursor=_conn.cursor()
            _cursor.execute(cmd)
        except sqlite3.OperationalError as e1:
            print(e1)
        except Error as e2:
            print(e2)
        finally:
            if _conn: _conn.close()
        return _state
    
    def operating_sql_cmd(self,db_name,cmd):
        _excute_ok = False
        _conn = None
        try:
            _conn = sqlite3.connect(db_name,self.__timeout)
            _cursor=_conn.cursor()
            _cursor.execute(cmd)            
            _conn.commit()
            _excute_ok = True
        except sqlite3.OperationalError as e :
            print(e)
            _excute_ok = False
        except Error:
            print(e)
            _excute_ok = False
        finally: 
            if _conn:_conn.close()
        return _excute_ok

    def operating_status_cmd(self,db_name,cmd):
        _err = ""
        _conn = None
        try:
            _conn = sqlite3.connect(db_name,self.__timeout)
            _cursor=_conn.cursor()
            _cursor.execute(cmd)            
            _conn.commit()
        except sqlite3.OperationalError as e1 :
            _err = e1
            print(e1)
        except Error as e2:
            _err = e2
            print(e2)
        except Exception as e3:
            _err = e3
            print(e3)
        finally: 
            if _conn:_conn.close()
        return str(_err)
    
    # read status
    def read_sql(self,db_name,cmd):
        _conn = None
        _list = []
        _state = ""
        try:
            _conn = sqlite3.connect(db_name,self.__timeout)
            _cursor=_conn.cursor()
            _cursor.execute(cmd)       
            for row in _cursor:_list.append(row)       
        except sqlite3.OperationalError as e1:
            print(e1)       
        except Error as e2:
            print(e2)
        except Exception as e3:
            print(e3)
        finally:
            if _conn:_conn.close()
        return _list

    # read status
    def read_status(self,db_name,cmd):
        _conn = None
        _list = []
        _state = ""
        try:
            _conn = sqlite3.connect(db_name,self.__timeout)
            _cursor=_conn.cursor()
            _cursor.execute(cmd)       
            for row in _cursor:_list.append(row)       
        except sqlite3.OperationalError as e1:
            _state = e1       
        except Error as e2:
            _state = e2
        except Exception as e3:
            _state = e3
        finally:
            if _conn:_conn.close()
        return str(_state)

    # Create database data & type.
    def create_table(self,db_name,table):
        _name_list = list(self.__dict_data_type.keys())
        _type_list = list(self.__dict_data_type.values())
        _list_len  = len(_name_list) 
        _creat_type = ""
        
        for idx,val in enumerate(_name_list):
            if idx < (_list_len-1): 
                _creat_type += val+" "+_type_list[idx]+"," 
            else:                                       
                _creat_type += val+" "+_type_list[idx]    # 最後一個逗號不能上
        
        _sql_cmd = (
            """
            create table if not exists %s (%s);
            """
            %(table,_creat_type)
        )
        # Exceute sql cmd.
        self.execute_sql_cmd(db_name=db_name,cmd=_sql_cmd)
        
    # Save fasetn data.
    def insert_data(self,db_name,table,pdict):  
        _pdict = pdict
        _type_list = list(pdict.keys())
        _type_len = len(_type_list)
        _value_list = list(pdict.values())
        _type_str  = ""
        _value_str = ""

        for idx,val in enumerate(_type_list):
            if idx < (_type_len-1): 
                _type_str  += val + ","
                _value_str += _value_list[idx] + "," 
            else:
                _type_str  += val
                _value_str += _value_list[idx]           

        _sql_cmd = (
            """
            insert into %s (%s) values (%s)
            """
            %(table,_type_str,_value_str)
        )
        # Exceute sql cmd.
        self.operating_sql_cmd(db_name= db_name,cmd=_sql_cmd)        
#endregion
