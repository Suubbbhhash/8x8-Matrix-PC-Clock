import serial
import time
from datetime import datetime
import serial.tools.list_ports

BAUD = 9600

def find_arduino():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if "Arduino" in port.description or "CH340" in port.description:
            return port.device
    return None

print("Auto Sync Service Started...")

while True:
    port = find_arduino()

    if port:
        try:
            print("Arduino found on", port)
            arduino = serial.Serial(port, BAUD, timeout=2)
            time.sleep(2)

            now = datetime.now()
            hours = now.hour
            minutes = now.minute
            seconds = now.second

            if hours > 12:
                hours -= 12
            if hours == 0:
                hours = 12

            time_string = f"TIME {hours} {minutes} {seconds}\n"
            arduino.write(time_string.encode())

            print("Clock Synced ")

            arduino.close()

            # Wait until Arduino unplugged before scanning again
            while find_arduino():
                time.sleep(1)

        except:
            pass

    time.sleep(1)
