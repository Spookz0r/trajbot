# -*- coding: utf-8 -*-
from serial import Serial
from time import sleep, time


import logging
log = logging.getLogger(__name__)
log.setLevel(logging.DEBUG)
logging.basicConfig(level=logging.DEBUG)

###########################
# serial communication to nexus
def mk_expectation_exception(wanted, got):
    return Exception(
        "Expected {0} but got {1}".format(
            str(wanted),
            str(got)))

class Wlink(object):
    def __init__(self, port,logbrowser): #Initialize port
        self.serial = Serial(port, 9600, timeout=5) #Initialize port with Baud Rate 9600 and timeout of 5s        
        self.logbrowser=logbrowser
        self.logbrowser.clear()
        log.info("Giving serial port 2 seconds to wake up")
        sleep(2)
        log.info("Serial ready")
        log.info("Reading motor set points")
        connection_established = self.ping()
        if connection_established:
            log.info("Robot ready")            
        else:
            raise mk_expectation_exception("True", "False")
        
    def __del__(self): #Close port
        self.serial.close()
        print 'Serial port closed'

    def _writemsg(self, msg): #write message to Arduino
        b = msg.encode("ascii") #Format it correctly and...
        log.info("sending {0}".format(str(b)))
        self.serial.write(b) #... send it after logging it

    def _readmsg(self,timeout):# reads a message byte by byte
        st = time()        
        while time()-st < timeout: # read until the timeout is reached
            b=b""
            msg = b""
            while b!=b";" and time()-st < timeout: # read the message until you receive a ";"
                b = self.serial.read()
                msg += b                
            bytes_waiting=self.serial.inWaiting()
            if bytes_waiting!=0: #in case the buffer is not empty, read also the rest if the timeout is not reached
                while bytes_waiting!=0 and time()-st < timeout:
                    b = self.serial.read(bytes_waiting)
                    msg += b
                    bytes_waiting=self.serial.inWaiting()
                break
            else:
                break
            if time()-st >= timeout:
                log.info('Message receive timeout')
            return msg
                    
                    
#        if flag_serial==0:
#            timeout = 10.
#            st = time()
#            msgFromSerial=b""
#            while time()-st < timeout:
#                b = b""
#                while b != b":" and time()-st < timeout: #read until : is reached
#                    b = self.serial.read()
#                    msgFromSerial += b
#                b = self.serial.read() 
#                msgFromSerial += b
#                msg = b""
#                while b != b";" and time()-st < timeout: #read until ; is reached
#                    msg += b #and add the read byte/letter? to the message
#                    b = self.serial.read()
#                    msgFromSerial += b
#                log.info("read {0}".format(str(msg)))
#                if msg.startswith(b"!"):
#                    log.debug("robot says: {0!s}".format(msg))
#                else:
#                    break
#            if time()-st >= timeout:
#              log.info('Message receive timeout')
#            self.logbrowser.append(msgFromSerial)
#        else:
#            bytes_waiting=self.serial.inWaiting()
#            msg=self.serial.read(bytes_waiting)
        return msg

    def ping(self):
        self._readmsg(0.1) # read out possible old buffer values
        self._writemsg(":state?;") # Ask for state of the serial connection
        #sleep(1) ##########################
        resp = self._readmsg(1) # Record response
        self.logbrowser.append(resp) 
        if resp!= "":
            return True
        else:
            return False

#    def buffer_input(self):
#        self.serial.read

#    def close_connection(self): #Closes connection
#        self.serial.close()


    #=================================================
    #manual control mode
    
    def manual_forward(self, power):
        assert power >= -999 and power <= 999
        serialprint_msg = self.control(power, power)
        return serialprint_msg

    def manual_backward(self, power):
        assert power >= -999 and power <= 999
        serialprint_msg = self.control(-power, -power)
        return serialprint_msg

    def manual_rotateR(self, power):
        assert power >= -999 and power <= 999
        serialprint_msg = self.control(power/2, -power/2)
        return serialprint_msg

    def manual_rotateL(self, power):
        assert power >= -999 and power <= 999
        serialprint_msg = self.control(-power/2, power/2)
        return serialprint_msg

    def control(self, uleft, uright):
        assert uleft >= -999 and uleft <= 999
        assert uright >= -999 and uright <= 999
        # NOTE: a constant base voltage may be needed. 
        uleft = int(uleft)
        uright = int(uright)
        msg = ":manual {0:=+04d} {1:=+04d};".format(uleft, uright)
        #print msg
        self._writemsg(msg)
        #for printing serial.print into the GUI
        serialprint_msg=self._readmsg(1)
        self.logbrowser.append(serialprint_msg)
        return serialprint_msg

    #===============================================
    #send state
    
    def transmit_state(self, x, y, theta):
        # transform x, y, theta to int
        # assume x,y are in meters, send them in cm
        x = int(x*100)
        y = int(y*100)
        # assume theta is given in degree
        theta = int(theta)
        msg = ":pose {0:=+04d} {1:=+04d} {2:=+04d};".format(x, y, theta)
        print msg
        self._writemsg(msg)
        #for printing serial.print into the GUI
        serialprint_msg=self._readmsg(1)
        self.logbrowser.append(serialprint_msg)
        return serialprint_msg

    #===============================================
    #send start and goal point
    
    def transmit_startgoal(self, x0, y0, xg, yg):
        # transform x0, y0, xg, yg, to int
        # assume x,y are in meters, send them in cm
        x0 = int(x0*100)
        y0 = int(y0*100)
        xg = int(xg*100)
        yg = int(yg*100)
        msg = ":startgoal {0:=+04d} {1:=+04d} {2:=+04d} {3:=+04d};".format(x0, y0, xg, yg)
        print msg
        self._writemsg(msg)
        #for printing serial.print into the GUI
        serialprint_msg=self._readmsg(1)
        self.logbrowser.append(serialprint_msg)
        return serialprint_msg
