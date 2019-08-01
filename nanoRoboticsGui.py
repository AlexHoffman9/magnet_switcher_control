import tkinter as tk
import serial
COMMAND_LENGTH = 12


ser = serial.Serial('COM3', 115200, timeout = 1)
try:

    
    start_command = 'START_______'
    stop_command =  'STOP________'
    freq = 'FREQ '
    dir_plus     =  'DIR +_______'
    dir_minus    =  'DIR -_______' 
    window = tk.Tk()
    window.title('nano robotics control')
    #geometry: width x height
    window.geometry('400x400')

    l = tk.Label(window, text='Input a frequency below before start', bg = 'grey', 
            font =('Arial', 10), width = 30, height = 3)
    l.pack()
    #l2 is used to show the messages sent back from the board.	
    textVar = tk.StringVar()
    l2 = tk.Label(window, textvariable=textVar, bg = 'purple', 
            font =('Arial', 10), width = 30, height = 3)
    l2.pack()
	
    var3 = tk.StringVar()
    l3 = tk.Label(window, bg = 'yellow', font =('Arial', 10), width = 30, height = 3, text = 'You have not specify the direction')
    
	
    e = tk.Entry(window, show=None)
    e.pack()

    def start_Method():
        var = e.get()
        if len(var) == 0:
            command = bytes(start_command, 'utf-8')
            
        else:
            command_str = freq + var
            commandLength = len(command_str)
            # add padding so length is correct
            command_str = command_str + (COMMAND_LENGTH-commandLength) * '_' 
            command = bytes(command_str, 'utf-8')
        ser.write(command)
        ret = ser.readline()
        while len(ret) > 1:
            print(ret)
            textVar.set(ret)			
            ret = ser.readline()
            
    def stop_Method():
        command = bytes(stop_command, 'utf-8')
        ser.write(command)
        ret = ser.readline()
        while len(ret) > 1:
            print(ret)
            textVar.set(ret)			
            ret = ser.readline()
    def freq_Method():
        start_Method()
		
    def direction_Selection():
        l3.config(text = 'you have selected "' + var3.get() + '"' )
        if var3.get() == '+ direction':
            command = bytes(dir_plus, 'utf-8')	
        else:
            command = bytes(dir_minus, 'utf-8')		
        ser.write(command)
        ret = ser.readline()
        while len(ret) > 1:
            print(ret)
            textVar.set(ret)			
            ret = ser.readline()
			
    r1 = tk.Radiobutton(window, text='+ direction', variable = var3, value = '+ direction', command = direction_Selection)
    
    r2 = tk.Radiobutton(window, text='- direction', variable = var3, value = '- direction',command = direction_Selection)

	
    b1 = tk.Button(window, text = 'START', width = 20, height = 2, command = start_Method)

    b1.pack()

    b2 = tk.Button(window, text = 'STOP', width = 20, height = 2, command = stop_Method)

    b2.pack()

    b3 = tk.Button(window, text = 'CHANGE FREQ', width = 20, height = 2, command = freq_Method)

    b3.pack()
	
    l3.pack()
    r1.pack()
    r2.pack()
	
    window.mainloop()
    
finally:
    ser.close()