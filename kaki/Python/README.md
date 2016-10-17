start.py finds the operating system then calls either WinDrive.py or LinDrive to start communicating with HID device.
Driver.py is an old version. Follow following steps in windows to install all required libraries, installation on linux is similar so I won't write how to do it.

    python -m pip install –upgrade pip
    python -m pip install libusb1
    python -m pip install pyusb
    python -m pip install pywinusb
    exit

