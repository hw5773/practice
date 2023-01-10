from pwn import *

e = ELF("./test")
#puts_plt = e.plt['puts']
read_got = e.got['read']
