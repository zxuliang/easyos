####common flags ######
CROSS_COMPILE:=arm-none-eabi-
CC 	:= $(CROSS_COMPILE)gcc
LD 	:= $(CROSS_COMPILE)ld
NM 	:= $(CROSS_COMPILE)nm
SIZE := $(CROSS_COMPILE)size
OBJDUMP := $(CROSS_COMPILE)objdump
OBJCOPY := $(CROSS_COMPILE)objcopy

ARCH_FLAGS	:= -march=armv5tej -marm -mlittle-endian
CMN_FLAGS	:= -fno-common -ffunction-sections -fdata-sections
CMN_FLAGS	+= --specs=nano.specs --specs=nosys.specs
CMN_FLAGS	+= -fomit-frame-pointer -fno-common -fno-builtin -Wall -O0 -g

AFLAGS	:= $(CMN_FLAGS) $(ARCH_FLAGS) -D__ASM__
CFLAGS	:= $(CMN_FLAGS) $(ARCH_FLAGS) -std=gnu99 -fno-builtin
LDFLAGS	:=  -Wl,--gc-sections -Wl,-lgcc -nostartfiles -nostdlib -Bstatic -Teasyos.lds

#
# srcfiles or includes for build
#
INCLUDES := -Iinc
SRCS := $(wildcard src/plat/*.S)
SRCS += $(wildcard src/plat/*.c)
SRCS += $(wildcard src/os/*.c)
SRCS += $(wildcard src/os/*.S)
SRCS += $(wildcard src/appx/*.c)

#
# objfiles
#
OBJS :=$(patsubst %.c, out/%.o, $(filter %.c,$(SRCS)))
OBJS +=$(patsubst %.S, out/%.o, $(filter %.S,$(SRCS)))
DEPS :=$(OBJS:.o=.d)

####
#### pass linker options -Wl,--gc-sections using gcc
#### pass linker options --gc-sections using ld
#### @$(LD) $(OBJS) $(LDFLAGS) -o $@ -Map $(basename $@).map
#### -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
#### qemu-system-arm -M versatilepb -m 128M -kernel $< -nographic -serial mon:stdio -S -s
####

TARGET:=out/easyos.elf

.PHONY: all clean run tags

all: $(TARGET)
	@echo "--------------------------------------------------"
	@$(SIZE) $<

run: $(TARGET)
	qemu-system-arm -M versatilepb -m 128M -kernel $< -nographic -serial mon:stdio
debug: $(TARGET)
	qemu-system-arm -M versatilepb -m 128M -kernel $< -nographic -serial mon:stdio -S -s

$(TARGET): $(OBJS)
	@echo ""
	@echo "CC -Wl,--start-group $(OBJS) -Wl,--end-group $(LDFLAGS) -o $@"
	@$(CC) -Wl,--start-group $(OBJS) -Wl,--end-group $(LDFLAGS) -o $@
	@$(OBJCOPY) --gap-fill=0xff -O binary -j.text -j.data -S $@ $(subst .elf,.bin,$@)
	@$(OBJCOPY) --gap-fill=0xff -O ihex -j.text -j.data -S $@ $(subst .elf,.hex,$@)
	@$(NM) -n $@ | grep -v '\( [aUw] \)|\(__crc__\)' > $(subst .elf,.map,$@)

out/%.o : %.c
	@mkdir -p $(dir $@)
	@echo "CC  $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

out/%.o : %.S
	@mkdir -p $(dir $@)
	@echo "CC  $<"
	@$(CC) $(AFLAGS) $(INCLUDES) -c $< -o $@

#
#Auto depends for c or asm
#
out/%.d : %.c
	@mkdir -p ${dir $@}
	@set -e; rm -f $@; $(CC) -MM $< $(INCLUDES) $(CFLAGS) > $@.$$$$;\
	sed 's,\(.*\)\.o[ :]*,$(addprefix $(dir $@),\1.o):,g' < $@.$$$$ > $@;\
	rm -f $@.$$$$

out/%.d : %.S
	@mkdir -p ${dir $@}
	@set -e; rm -f $@; $(CC) -MM $< $(INCLUDES) $(AFLAGS) > $@.$$$$;\
	sed 's,\(.*\)\.o[ :]*,$(addprefix $(dir $@),\1.o):,g' < $@.$$$$ > $@;\
	rm -f $@.$$$$

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

#Tell makefile dont't autoremove anything
.SECONDARY:

tags:
	@ctags -R .
clean:
	@-rm -rf out tags
	@echo "Cleaning...done"
