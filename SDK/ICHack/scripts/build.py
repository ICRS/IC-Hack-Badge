import json


with open('secret.json') as f:
    secret_json = json.load(f)

insert_position = 0x00040000
with open("build/ICHack.bin", 'rb') as f:
    original_data = f.read()

# Create a new binary content
new_data = bytearray()

# Copy the data before the insert position
new_data.extend(original_data[:insert_position])

# Add padding with zeros if necessary
if len(original_data) < insert_position:
    new_data.extend(b'\x00' * (insert_position - len(original_data)))

# secret message here morse
s = secret_json.get("secret_msg", "")
b = s.replace(" ", "0"*3).replace("-", "1110").replace(".", "10")
for i in range(0, len(b), 8):
    new_data.extend(int(b[i:i+8][::-1], 2).to_bytes(1, "little"))


if len(new_data) < 0x00040100:
    new_data.extend(b'\x00' * (0x00040100 - len(new_data)))

s = secret_json.get("custom_msg", "")

new_data.extend(len(s).to_bytes(4, "little"))
new_data.extend(s.encode(encoding="ascii"))

quotes = secret_json.get("quotes")
if len(new_data) < 0x00040400:
    new_data.extend(b'\x00' * (0x00040400 - len(new_data)))

new_data.extend(len(quotes).to_bytes(4, "little"))
for q in quotes:
    new_data.extend(len(q).to_bytes(4, "little"))

for q in quotes:
    new_data.extend(q.encode("ascii"))

with open("build/temp.bin", 'wb') as f:
    f.write(new_data)
