#!/usr/bin/python

DEBUG = False

# Import the required libraries
import sys, serial, glob, array, threading
import json, urllib
import xml.etree.ElementTree as ET
from time import sleep

sys.path.insert(0, '/usr/lib/python2.7/bridge/')
from bridgeclient import BridgeClient as bridgeclient
value = bridgeclient()

# This function is used to writeToStdOut to the main board (needs to flush to see content!)
def writeToStdOut(text):
    sys.stdout.write("OLF," + text + "\n")
    sys.stdout.flush()

# -- SERIAL PORT DETECTION AND HANDLING (connection to olfactometer's Arduino micro)

def scan():
    # scan for available ports. return a list of device names.
    return glob.glob('/dev/ttyACM*')

def choosePort():
    # list ports if more than one to choose; if not, we're done; returns port name
    global port_name
    total_ports = 0
    list_ports=scan()
    total_ports=len(list_ports)
    if total_ports == 0:
        writeToStdOut("E,FATAL PORT: COULD NOT FIND OLFACTOMETER.");
    else:
        port_name=list_ports[0]

def initializeOlfacto():
    # initializes serial communication with Arduino micro
    global ser, port_name
    try:
        choosePort()
        ser = serial.Serial(port_name,9600)
        ser.close()
        ser.open()
        ser.flushInput()
        ser.flushOutput()
    except serial.SerialException:
        writeToStdOut("E,FATAL ERROR: COULD NOT CONNECT TO OLFACTOMETER.")
    else:
        writeToStdOut("CONOK,1")
    sys.stdout.flush()

# Closes Serial connection properly
def closeOlfacto():
    global ser
    try:
        ser.flushInput()
        ser.flushOutput()
        ser.close()
    except serial.SerialException:
        writeToStdOut("E,COULD NOT CLOSE OLFACTOMETER.")

def resetMicro():
    global ser, port_name
    endListening()
    closeOlfacto()
    sleep(1)
    ser2=serial.Serial(port_name,1200)
    ser2.close()
    ser2.open()
    ser2.close()
    sleep(20)

def startListening():
    global threadRead, threadWrite, threadCommands, running
    initializeOlfacto() # Serial initialization
    initializeBuffers()
    running = True
    # Threads definitions
    threadRead = threading.Thread(target=readOlfacto)
    threadWrite = threading.Thread(target=writeToOlfacto)
    threadCommands = threading.Thread(target=respondToCommands)
    threadRead.daemon = True
    threadWrite.daemon = True
    threadCommands.daemon = True
    try:
        threadRead.start()
        threadWrite.start()
        threadCommands.start()
    except:
        writeToStdOut("E,FATAL ERROR: UNABLE TO START THREAD ON OLFACTOMETER BOARD")

def endListening():
    global running
    running = False

# This thread looks continiously for serial input and puts it in the reading buffer
def readOlfacto():
    global ser,running,bufferRead
    while running:
        while ser.inWaiting() > 0:
            if ser.readable():
                character = ser.read(1)
                if character == b'\r':
                    pass
                elif character == b'\n':
                    writeToStdOut(bufferRead.decode("utf-8"))
                    bufferRead = bytearray('')
                else:
                    bufferRead.extend(character)

def initializeBuffers():
    global bufferRead, bufferWrite
    bufferRead = bytearray('') # input buffer
    bufferWrite = bytearray(' ') # output buffer

# This thread looks continiously for serial output to send from the output buffer and does it at the first occasion
def writeToOlfacto():
    global ser,bufferWrite
    while running:
        if(len(bufferWrite) > 1):
            if ser.writable():
                try:
                    ser.write(str(unicode(chr(bufferWrite[1]))))
                except serial.SerialException:
                    writeToStdOut("E,COULD NOT SEND COMMAND TO OLFACTOMETER: " + bufferWrite[1])
                else:
                    del bufferWrite[1]

# This function adds a command to the output buffer
def sendCommandToOlfacto(cmd):
    global ser, bufferWrite
    bufferWrite.extend(cmd.encode(encoding = 'UTF-8'))
    bufferWrite.extend('\r')
    bufferWrite.extend('\n')

# This function handles commands
def handleCommand(command):
    global DEBUG,running,running_program
    split_command=command.rstrip().split(',')
    cmd_code=split_command[0]
    if(len(split_command)>1):
        param1=split_command[1]
    else:
        param1="0"
    if(len(split_command)>2):
        param2=split_command[2]
    else:
        param2="0"
    if cmd_code == 'D':
        sendCommandToOlfacto('D,' + param1)
        if DEBUG:
            writeToStdOut("S,Duration changed to " + param1)
    elif cmd_code == 'W':
        sendCommandToOlfacto('W,0')
        if DEBUG:
            writeToStdOut("S,Flushing")
    elif cmd_code == 'OV':
        sendCommandToOlfacto('O,' + param1)
        if DEBUG:
            writeToStdOut("S,Opening valve " + param1)
    elif cmd_code == 'CV':
        sendCommandToOlfacto('C,' + param1)
        if DEBUG:
            writeToStdOut("S,Closing valve " + param1)
    elif cmd_code == 'FLD':
        sendCommandToOlfacto('R,0')
    elif cmd_code == 'FCS':
        sendCommandToOlfacto('M,' + param1)
        if DEBUG:
            writeToStdOut("S,Carrier flow set to " + param1)
    elif cmd_code == 'FOS':
        sendCommandToOlfacto('F,' + param1)
        if DEBUG:
            writeToStdOut("S,Odor flow set to " + param1)
    elif cmd_code == 'I':
        sendCommandToOlfacto('I,0')
        if DEBUG:
            writeToStdOut("S,Idling")
    elif cmd_code == 'T':
        sendCommandToOlfacto('T,' + param1)
        if DEBUG:
            writeToStdOut("S,Trial valve " + param1)
    elif cmd_code == 'RESET':
        if DEBUG:
            writeToStdOut("S,Resetting Arduino Micro...")
        resetMicro()
        startListening()
    elif cmd_code == 'INIT':
        if DEBUG:
            writeToStdOut("S,Reinitializing")
        running = False
        closeOlfacto()
        startListening()
    elif cmd_code == 'EXIT':
        running = False
        running_program = False
    else:
        writeToStdOut("S,UNKNOWN COMMAND")

# This thread listens to input on the console and sends them to the handling function
def respondToCommands():
    global running
    command=None
    while running:
        command=sys.stdin.readline()
        if command:
            handleCommand(command)
            command=None

# Initialization of the buffers and of the state variable (running must be true, otherwise the sketch exits)
running = True
running_program = True

startListening()

# runs continuously until running is set to false
while running_program:
    pass

closeOlfacto()
exit()
