from tkinter import *
from tkinter import ttk
 
 
def main():
    lasty, lastx = 0, 0
 
    def xy(event):
        nonlocal lasty, lastx
        lastx = event.x
        lasty = event.y
 
    def addLine(event):
        nonlocal lasty, lastx
        canvas.create_line((lastx, lasty, event.x, event.y))
        lasty = event.y
        lastx = event.x
 
 
    root = Tk()
    root.columnconfigure(0, weight=1)
    root.rowconfigure(0, weight=1)
 
    canvas = Canvas(root)
    canvas.grid(column=0, row=0, sticky='nwes')
    canvas.bind('<Button-1>', xy)
    canvas.bind('<B1-Motion>', addLine)
    root.mainloop()
 
 
if __name__ == '__main__':
    main()