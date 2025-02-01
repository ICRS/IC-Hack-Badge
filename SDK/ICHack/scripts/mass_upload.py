import os
from time import sleep

import os

file_name = "Output.uf2"
source = f"{file_name}"
drive_name = "RPI-RP2"

def find_drive_by_name(target_name):
    for letter in "ABCDEFGHIJKLMNOPQRSTUVWXYZ":
        drive = f"{letter}:"
        result = os.popen(f"vol {drive}").read()
        if target_name in result:
            return drive
    return None


while True:
    found_drive = find_drive_by_name(drive_name)

    if found_drive is not None:
        print(f"Drive '{drive_name}' found at {found_drive}, copying")
        destination = f"{found_drive}\\{file_name}"
        output = os.popen(f'copy "{source}" "{destination}"').read()
        print(output)

    else:
        sleep(1)