from xbee import ZigBee
import serial
import struct 
import signal
import sys

"""
    Pan Id brute force script
    Wooyoung Chung
    3/5/14
"""
def exceptionHandler(signum, frame):
    raise Exception("")

def callBackXbee(data):
    """ """
    print "CB Data \n", data

#set serial communication with xbee
ser = serial.Serial('/dev/cu.usbserial-AD025EOA', 9600)

#python xbee api doesn't implmenet timeout on read
#to work around this, set signal alarm to trigger timout

#set signal for timeout
signal.signal(signal.SIGALRM, exceptionHandler)

#create instance of zigbee
xbee = ZigBee(ser)

hb = 0x00
lb = 0x00

xbee.at(command='ID')
ret = xbee.wait_read_frame()
print "Current ID"
print ret

xbee.at(command='ND')
print "ACTIVE SCAN RESULT:"
print (xbee.wait_read_frame())

exit()
fd = open("scanlog.txt", "w+")

while 1:
    if hb > 0xFF:
        break
    rb = lb << 8 | hb
    fd.write("[*] pan id 0x%02x%02x\n" % (hb,lb))
    try:    
        #change pan id and query id command with argument 
        arg = struct.pack('h', rb)
        xbee.at(command='ID', parameter=arg)
        (xbee.wait_read_frame())
        
        #attempt to discover node 
        xbee.at(command='ND')
        signal.alarm(5)
        ret = (xbee.wait_read_frame())
        
        #unlike c api (matching with request), 
        #python xbee api read whatever data was received
        #that might cause to bypass current ND 
        #therefore to work around this if status is not ok, redo
        if ret['status'] != '\x00':
            continue

        if ret['status'] == '\x00':
            print "\n", ret
            break

    except Exception, exc:
        sys.stdout.write('.') 

    #increment pan id
    lb = lb + 0x1
    #if lower byte is FF reset it and increment high byte
    if lb > 0xFF:
        lb = 0x0
        hb = hb + 0x1

fd.close()
ser.close()
