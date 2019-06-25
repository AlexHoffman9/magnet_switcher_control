port = 'COM4'; % change this to match the device you are using
% find out in device manager

com = serial(port);
set(com,'BaudRate',115200);

fopen(com);
fprintf(s, 'FREQ 10');

output = fscanf(com)

fclose(com)
delete(com)
clear com