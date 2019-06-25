import serial
COMMAND_LENGTH = 12
print('Commands: ')
print('START: starts switching')
print('STOP: stops switching')
print('FREQ #: changes switching frequency. # is float frequency in Hz')
print('QUIT: stops switching and exits this code')
try:
    ser = serial.Serial('COM3', 115200, timeout = 1)  
    running = True 
    while running:
        command_str = input('Enter command: START, STOP, FREQ #, QUIT ')
        if command_str == 'QUIT': # end python program
            command_str = 'STOP'
            running = False # breaks out of loop afer sending stop command
        # find length of command
        commandLength = len(command_str)
        # add padding so length is correct
        command_str = command_str + (COMMAND_LENGTH-commandLength) * '_'
        # print(command_str)
        command = bytes(command_str, 'utf-8')
        ser.write(command)
        ret = ser.readline()
        while len(ret) > 1:
            print(ret)
            ret = ser.readline()
finally:
        ser.close()

