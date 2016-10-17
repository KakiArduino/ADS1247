import platform
import os.path
from time import strftime

OS = platform.system()

def IdentifyOS():
    if OS is 'Windows':
        print('OS detected: ', OS)
        try:
            import WinDrive as driver
        except ImportError:
            print ("unable to start windows driver")
        

    if OS is 'Linux':
        print('OS detected: ', OS)
        try:
            import LinDrive as driver
        except ImportError:
            print ("unable to start windows driver")

##   Create datalog     ##        
def DataLog():
    subdirectory = strftime("%a, %d %b %Y %H:%M:%S")
    try:
        os.mkdir(subdirectory)
    except Exception:
        print('fail')
        pass

DataLog()
IdentifyOS()
