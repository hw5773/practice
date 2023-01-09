#!/usr/bin/python3
# Name: pup.py

from pwn import *

s32 = 0x41424344
s64 = 0x4142434445464748

print (p32(s32))
print (p64(s64))

s32 = "ABCD"
s64 = "ABCDEFGH"

print (hex(u32(s32)))
print (hex(u64(s64)))

