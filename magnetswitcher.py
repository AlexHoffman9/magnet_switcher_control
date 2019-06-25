import serial
try:
    ser = serial.Serial('COM3', 115200, timeout = 1)   
    while 1:
        command = bytes(input('Available commands: START, STOP, FREQ # '), 'utf-8')
        if command == bytes('QUIT', 'utf-8'): # end python program
            break
        ser.write(command)
        ret = ser.readline()
        while len(ret) > 1:
            print(ret)
            ret = ser.readline()
finally:
        ser.close()

