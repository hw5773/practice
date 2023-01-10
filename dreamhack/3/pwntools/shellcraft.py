#!/usr/bin/python3
# Name: shellcraft.py

from pwn import *
context.arch = 'aarch64'
code = shellcraft.sh()
print (code)
