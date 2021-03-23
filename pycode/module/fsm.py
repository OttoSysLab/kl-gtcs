# Finite State Machine
# -*- coding: utf-8 -*-
# Event table struct.
class FSMTable():
    # Initial even table.
    def __init__(self):     
        self.event    = None   
        self.C_state  = None
        self.action   = None
        self.N_state  = None

# Moore FSM
class MooreFSM():
    # Moor FMS state struct. 
    class MooreFSMStuct():
        FSM_table = FSMTable()
        C_state = None
  
    # Initial Object.
    def __init__(self):
        self.FSM_state = self.MooreFSMStuct()
        self.__FSM_lst = []   

    # Regist FSM 
    def regist_FSM(self,Event,C_state,Action,N_state):
        _FSM = FSMTable()        
        _FSM.C_state  = C_state
        _FSM.event    = Event
        _FSM.action   = Action
        _FSM.N_state  = N_state
        self.__FSM_lst.append(_FSM)

    # Set start state
    def set_start_state(self,S_state):
        self.FSM_state.C_state = S_state     
    
    # Transfer FSM state.
    def __trasfer_FSM_state(self,state):
        self.FSM_state.C_state=state
    
    # Handle FSM evernt.
    def handle_FSM_event(self,event):
        _switch_ok =False
        _act_FSM = None
        _event_trigger = False
        for idx,val in enumerate(self.__FSM_lst):
            if ((event==val.event)&(self.FSM_state.C_state==val.C_state)):
                _act_FSM = val
                _event_trigger = True
                break
        if _event_trigger == True:
            if _act_FSM.action!=None:
                _act_FSM.action()
                self.__trasfer_FSM_state(state=_act_FSM.N_state)
                _switch_ok==True
        else:
            pass         

# Mealy FSM
class MealyFSM(): 
    # Moor FMS state struct. 
    class MealyFSMStruct():
        FSM_table = FSMTable()
        MearlyValue = 0
        C_state = None

    # Initial Object.
    def __init__(self):
        self.FSM_state = self.MealyFSMStruct()
        self.__FSM_lst = []    

    # Regist FSM 
    def regist_FSM(self,Event,C_state,Action,N_state):
        _FSM = FSMTable()        
        _FSM.C_state  = C_state
        _FSM.event    = Event
        _FSM.action   = Action
        _FSM.N_state  = N_state
        self.__FSM_lst.append(_FSM)

    # Set start state
    def set_start_state(self,S_state):self.FSM_state.C_state = S_state     
    
    # Transfer FSM state.
    def __trasfer_FSM_state(self,state):self.FSM_state.C_state=state
    
    # Handle FSM evernt.
    def handle_FSM_event(self,event):
        # 
        _act_FSM = self.FSM_state
        _N_state = None
        _event_trigger = False
        # Use currentGet current state.
        for idx,val in enumerate(self.__FSM_lst):
            if ((event== val.event)&(_act_FSM.C_state==self.__FSM_lst[idx].C_state)):
                _event_trigger = True
                _act_FSM.FSM_table=self.__FSM_lst[idx]
                _N_state = self.__FSM_lst[idx].N_state
                break
        # 
        if(_event_trigger==True):
            if(_act_FSM.FSM_table.action!=None):
                _act_FSM.FSM_table.action()                       
            self.__trasfer_FSM_state(state=_N_state)
        else:pass
#endregion
    