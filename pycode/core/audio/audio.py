import os 
import time 

_path = "/home/kls/project/music/"
_headphone = 90

_display_list = [
    "Secret of my heart.wav",
    "Start in my life.wav",
    "YESTERDAY LOVE.wav",
    "always.wav",
    "SAWAGE.wav",
    "DYNAMITE.wav",
    "Winter Bells.wav",
    "bbb.wav",
    "Time after time.wav",
    "TRY AGAIN.wav",
    "wind.wav",
    "ccc.wav",
    "Growing of my heart.wav",
    "white.wav",
    "Your Best Friend.wav",
    "Love for you.wav",
    "Tomorrow is the last Time.wav",
    "Revive.wav",
    "SUMMER TIME GONE.wav"
]

def main():
    _headphone_str = str(_headphone)+"%"
    os.system("amixer set Headphone %s"%(_headphone_str))
    _idx = 0
    while True:
        if _idx<len(_display_list):
            _music = '"'+_display_list[_idx]+'"'
            try:
                print("file = %s"%(_music))
                os.system("aplay -i %s"%(_music))
            except Exception as e:
                print(e)
        else:
            _idx = 0
        _idx = _idx+1
        time.sleep(1)

if __name__ == "__main__":
    main()