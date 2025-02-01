import json
import subprocess

with open('secret.json') as f:
    secret_json = json.load(f)

quotes = secret_json.get("quotes")
new_data = bytearray()

BLANK = b"\x00" * 0x300
new_data.extend(BLANK)

quotes = secret_json.get("quotes")
new_data.extend(len(quotes).to_bytes(4, "little"))
for q in quotes:
    new_data.extend(len(q).to_bytes(4, "little"))

for q in quotes:
    new_data.extend(q.encode("ascii"))


while True:
    s = input("Enter message here: ")
    new_data[:]

    s = s[:0x300 - 4].upper()
    new_data[:0x300] = BLANK
    new_data[:4] = len(s).to_bytes(4, "little")
    new_data[4:len(s)+4] = s.encode(encoding="ascii")
    with open("data.bin", "wb") as f:
        f.write(new_data)

    subprocess.run(["picotool", "load", "data.bin", "-o", "10040100", "-fx"])
