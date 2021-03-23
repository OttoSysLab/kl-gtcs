# Finite State Machine
# -*- coding: utf-8 -*-
# Event table struct.
class GtcsFSMStatus():
    class FSM():
        cmd  = ""
        data = dict()

    # Initial
    M_FSM = ""
    MCB = FSM() 
    IOM = FSM()

    # Initial signleTon object.
    _instance = None 
    def __new__(cls, *args, **kwargs): 
        if cls._instance is None: 
            cls._instance = super().__new__(cls) 
        return cls._instance  
    # Initial object.
    def __init__(self): pass

#endregion
    