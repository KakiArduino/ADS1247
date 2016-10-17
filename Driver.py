import os
import sys
import numpy
import platform
from time import gmtime, strftime ##,clock   clock() tells the last time clock() was called
import binascii
import pywinusb.hid as hid
import usb.core
import usb.util


string = '0x'
os = platform.system()
V = 0x0001
P = 0x0001
loop = True
device = 0
endpointIN =None
endpointOUT = None
intface = None
dataIN = None

found = 'kaki found'
nfound = 'kaki not found'

def setupPlatform():
    if os is 'Windows':
        print(platform.system())
        if __name__ == '__main__':
            vid = V
            pid = hid.get_full_usage_id(0xffab, 0x02)
            findHIDDevice(P, V)
            
    elif os is 'Linux':
        import usb.core
        import usb.util
        setupLinux(V,P)
    else:
        print ('Operating System not found')

def time():
    strftime("%Y:%m:%d:%H:%s", gmtime())

def rx_handler(dataIN):
    data = dataIN
    print ('recv: ', data)
    for ndx, x in enumerate(data):
        DataIn = chr(data[ndx])
        print(DataIn)

      
def findHIDDevice(pid, vid):
    filter = hid.HidDeviceFilter(vender_id = vid, product_id = pid)
    hid_device = filter.get_devices()
    all_devices = filter.get_devices()
    print ('Found %d matching hid devices' % len(all_devices))
    if not all_devices:
        print (nfound)
    else:
        #search for device
        for hid_device in all_devices:
            print(hid_device)
            try:
                hid_device.open()
                print(found)
                while True:
                    hid_device.set_raw_data_handler(rx_handler)

                sleep(0.2)
            except:
                print('close')
                hid_device.close()

def setupLinux(V,P):
    if os is 'linux':
        ##  device's IDs    ##
        Vender_ID = V
        Product_ID = P
        ##    Find USB device    ##
        try:
            global device
            device = usb.core.find(idVendor = Vender_ID,
                                       idProduct = Product_ID)
            print(hid_device)
        except:
            print(nfound)
            return
        if not hid_device:
            print(nfound)
        else:
            sys.stdout.write(found)
            ##     check os and configure apropriatly     ##
            ##    Check if OS is linux and see if device is clamed     ##

            if device.is_kernel_drive_active(interface) is True:
                try:#tell kernel to detach
                    device.detach_kernel_driver(interface)
                    #clame the device
                    usb.util.clam_interface(device, interface)
                except usb.core.USBError as e:
                    sys.exit('could not detach or clame kerel driver: %s', str(e))
                    
            try:       
                ##   use default configuration    ##
                hid_device.set_configuration()
                hid_device.reset()
            except usb.core.USBError as e:
                sys.exit('could not detach or clame kerel driver: %s', str(e))

            cfg = device.get_active_configuration()
            global intface
            intface = cfg[(0,0)]

            ##    Get endpoint instance    ##
            global endpointOUT
            endpointOUT = usb.util.find_descriptor(
                intface,
                # match the first OUT endpoint
                custom_match = \
                lambda e: \
                    usb.util.endpoint_direction(e.bEndpointAddress) == \
                    usb.util.ENDPOINT_OUT)
            global endpointIN
            endpointIN = usb.util.find_descriptor(
                intface,
                # match the first OUT endpoint
                custom_match = \
                lambda e: \
                    usb.util.endpoint_direction(e.bEndpointAddress) == \
                    usb.util.ENDPOINT_IN)
        
            assert endpointIN is not None

            while True:
                data =[0x00] * 64

                #read data
                bytes = hid_device.read(endpointOUT.bEndpointAddress, 64)
                rx_handler(bytes)

                for i in range(64)
                    data[i] = bytes[i]
                    data[i+8] = random.radint(0,50)
                hid_write(endpoinIN.bEndpointAddress, data)
                
            
setupPlatform()
