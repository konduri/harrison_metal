import serial
import time
import select

port = '/dev/ttyACM0'

ser = serial.Serial(port,9600, timeout=0)
time.sleep(2) # wait for Arduino

response=[]
byte_read=''

while True:
    #print "starting over"
    ser.flushInput()
    ser.flushOutput()
    read_data = ""
    while True:
        reading,_,_ = select.select([ser], [], [], 0.1) 
        if reading !=[]:                                   
            byte_read = ser.read(1)
            if byte_read == ">":
                print "Found end of line"
                read_data = "".join(response)
                print read_data
                break
            else:
                response.append(byte_read)
        else:
            break
#    print "read data is " +  read_data
