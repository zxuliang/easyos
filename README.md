# easyos
Just a hobby project and exercise of arm/gcc/makefile/linker-script/rtos; hardware is arm926

# make run
make the project and runing with qemu-system-arm

# make debug
make the project and can be debug with gdb. (not used very often)

# mini-context
r0-r1-r2-r3 will be used for function calling and return result, gcc will try to use only r0-r3 during function call if possible. caller need to save and retore r0-r3, callee need to save and restore r4-r11. r12(fp) also need to save and restore during context; {r0-r12,lr,CPSR,SPSR; sp is stack pointer}

