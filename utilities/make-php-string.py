from sys import stdin


buffer = bytearray()
while True:
    input = stdin.buffer.readline()
    if len(input) == 0:
        break
    for c in input:
        buffer.append(c)

line = ""
lines = []
for c in buffer:
    if len(line) >= 15*3:
        lines.append(line)
        line = ""
    line += "\\x{:02x}".format(c)
if len(line) > 0:
    lines.append(line)

n = 0
for line in lines:
    if n == 0:
        output = " "
    else:
        output = "."
    n += 1
    output += "\"{}\"".format(line)
    print(output)
