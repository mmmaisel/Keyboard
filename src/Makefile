########################################################################
# USB-Mux
#
# Makefile for ARM-GCC none-eabi toolchain
########################################################################
#
# Instructions
#
# On command line:
#
# make all = Make software.
#
# make clean = Clean out built project files.
#
# To rebuild project do "make clean" then "make all".
#
########################################################################
# Build Options   -  true / false

## TODO

########################################################################
# Target file name; name of the hex file to be built

TARGET = keyboard

########################################################################
# List any extra directories to look for include files here.
# Each directory must be seperated by a space.
# Use forward slashes for directory separators.
# For a directory that has spaces, enclose it in quotes.

EXTRAINCDIRS = dev

########################################################################
# If there is more than one source file, append them here separated by
# spaces. Usually you have to add the Library files here!
# (ONLY add c files "*.c" here, NO header files "*.h"!)
# Don't forget to add relative paths!

CPPSRC = main.cpp \
    led_matrix.cpp \
    key_matrix.cpp \
    key_layout.cpp \
    hid_keyboard_endpoint.cpp \
    modular_keyboard.cpp \
    module_detection.cpp \
    startup.cpp \
    system.cpp \
    uart.cpp \
    usb_control_endpoint.cpp \
    usb_descriptor.cpp \
    usb_endpoint.cpp \
    usb_phy.cpp \

CSRC = inttable.c

# assembler sources
ASRC =

# binary files
BIN =

# Global Defines
#
DEFINES = M_PI=3.1415926f STM32F401xC

# Global Undefines
UNDEFINES =

# precompiled header
PCHSRC =

# Additionally linked symbols
LINKSYM =

########################################################################
# Libraries to link.
LIBS = -L./ -lgcc

# Try to comment/uncomment this if you encounter strange problems
# with math lib, link linker errors or wrong compass/pitch/roll values.
#LIBS += -lm

########################################################################
# Optimization level, can be [0, 1, 2, 3, s]. 0 turns off optimization.
# (Note: 3 is not always the best optimization level.)

OPT = s

# Enable link-time optimization (This may break the code!)
LTO = false

########################################################################
# MCU definitions - STM32F401xE
MCPU   = cortex-m4
MARCH  = armv7e-m
MFPU   = fpv4-sp-d16
MFLOAT = hard

########################################################################
# Memory Layout - Linker script
LINKER_SCRIPT = memSections.ld

########################################################################
# Debugging format.
#     Native formats for AVR-GCC's -g are dwarf-2 [default] or stabs.
#     AVR Studio 4.10 requires dwarf-2.
#     AVR [Extended] COFF format requires stabs, plus an avr-objcopy run.

ENABLE_DEBUG = true
DEBUG_FORMAT = dwarf-3

# Programmer configuration
OPENOCD_CFG="st_nucleo_f401re.cfg"

########################################################################
########################################################################
#
# DO NOT EDIT BELOW
#
########################################################################
########################################################################

########################################################################
# ARM compiler definitions
########################################################################

# Define programs and commands.
CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar
CCPP = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size
NM = arm-none-eabi-nm
SHELL = sh
REMOVE = rm -f
REMOVEDIR = rm -R
COPY = cp
WINSHELL = cmd

# Define special build commands
CPU = -mcpu=$(MCPU) -march=$(MARCH) -mfpu=$(MFPU) -mfloat-abi=$(MFLOAT)
CPU += -mthumb

# Define Messages
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before:
MSG_SIZE_AFTER = Size after:
MSG_FLASH = Creating load file for Flash:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:

# Create a Variable containing a single space ' '
_NOTHING=
_SPACE=$(_NOTHING) $(_NOTHING)

# Output format. (can be srec, ihex, binary)
FORMAT = ihex

# Debugging format.
#     Native formats for ARM-GCC's -g are dwarf-3 [default] or stabs.
ifeq ($(ENABLE_DEBUG), true)
DEBUG = -g$(DEBUG_FORMAT)
DEBUG += -gstrict-dwarf
endif

# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99

CPPSTANDARD = -std=c++11

# Place -D or -U options here
# Create -D options from defines in $(DEFINES)
ifneq ($(DEFINES), $(_NOTHING))
CDEFS =  -D$(subst $(_SPACE),$(_SPACE)-D,$(DEFINES))
endif
# Create -U options from undefines in $(UNDEFINES)
ifneq ($(UNDEFINES), $(_NOTHING))
CDEFS += -U$(subst $(_SPACE),$(_SPACE)-U,$(UNDEFINES))
endif

# Create -u options for linker
ifneq ($(LINKSYM), $(_NOTHING))
UNDEF_SYMBOLS += $(subst $(_SPACE),$(_SPACE)-u,$(LINKSYM))
endif

# Optimization
OPTFLAGS = -O$(OPT)

ifeq ($(LTO), true)
OPTFLAGS += -flto
endif

# Place -I options here
CINCS =

########################################################################
#  Compiler Options
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS = $(DEBUG)
CFLAGS += $(CDEFS) $(CINCS)
CFLAGS += $(OPTFLAGS)
CFLAGS += -funsigned-char -funsigned-bitfields
CFLAGS += -fshort-enums
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wall -nostartfiles -ffreestanding
CFLAGS += -Wa,-adhlns=$(<:.c=.lst)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)
ifneq ($(COLORS), false)
CFLAGS += -fdiagnostics-color=always
endif

CPPFLAGS = $(DEBUG)
CPPFLAGS += $(CDEFS) $(CINCS)
CPPFLAGS += $(OPTFLAGS)
CPPFLAGS += -funsigned-char -funsigned-bitfields
CPPFLAGS +=	-fshort-enums
CPPFLAGS += -fno-exceptions -fno-rtti #-fno-implicit-templates
CPPFLAGS += -ffunction-sections -fdata-sections
CPPFLAGS += -Wall -Wextra -nostartfiles -ffreestanding
CPPFLAGS += -Wa,-adhlns=$(<:.cpp=.lst)
CPPFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CPPFLAGS += $(CPPSTANDARD)
ifneq ($(COLORS), false)
CPPFLAGS += -fdiagnostics-color=always
endif

########################################################################
#  Assembler Options
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -ahlms:    create listing
#  -gstabs:   have the assembler create line number information; note
#             that for use in COFF files, additional information about
#             filenames and function names needs to be present in the
#             assembler source files
#  -listing-cont-lines: Sets the maximum number of continuation lines
#       of hex dump that will be displayed for a given single line of
#       source input.
ASFLAGS = -Wa,-adhlns=$(<:.asm=.lst),$(DEBUG)
ifneq ($(COLORS), false)
ASFLAGS += -fdiagnostics-color=always
endif

########################################################################
#  Linker Options
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
ifneq ($(COLORS), false)
LDFLAGS += -fdiagnostics-color=always
endif
LDFLAGS += -Wl,-T$(LINKER_SCRIPT)

########################################################################
# build definitions

# Define all object files.
OBJ = $(CSRC:.c=.o) $(CPPSRC:.cpp=.o) $(ASRC:.asm=.o) $(BIN:.bin=.o)

# Define precompiled header file.
GCH = $(PCHSRC:.h=.h.gch)

# Define all listing files.
LST = $(CSRC:.c=.lst) $(CPPSRC:.cpp=.lst) $(ASRC:.asm=.lst)

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.

ALL_CFLAGS = $(CPU) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_CPPFLAGS = $(CPU) -I. $(CPPFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = $(CPU) -I. -x assembler-with-cpp $(ASFLAGS)

# buggy
# Target: rebuild all.
#new: begin clean_files gccversion sizebefore build sizeafter end

# Default target.
all: begin gccversion sizebefore build sizeafter end

# Target: clean project.
clean: begin clean_list end

build: gch elf hex lss sym

elf: $(TARGET).elf
hex: $(TARGET).hex
lss: $(TARGET).lss
sym: $(TARGET).sym
gch: $(GCH)

# Eye candy.
begin:
	@echo
	@echo $(MSG_BEGIN)

end:
	@echo $(MSG_END)
	@echo

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) $(TARGET).elf

sizebefore:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); \
	$(ELFSIZE); 2>/dev/null; echo; fi

sizeafter:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); \
	$(ELFSIZE); 2>/dev/null; echo; fi

# Display compiler version information.
gccversion :
	@$(CC) --version

########################################################################
#  main build script

# Create final output files (.hex) from ELF output file.
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

# Create extended listing file from ELF output file.
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@
	
# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
.PHONY : $(TARGET).elf
$(TARGET).elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CPPFLAGS) $^ $(LIBS) $(LDFLAGS) $(FIXME_MATH_LIB) $(UNDEF_SYMBOLS) --output $@

# Compile: create object files from C source files.
%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) $< -o $@
	
# Compile: create object files from C++ source files.
%.o : %.cpp
	@echo
	@echo $(MSG_COMPILING) $<
	$(CCPP) -c $(ALL_CPPFLAGS) $< -o $@

# Compile: precompiled header files.
%.h.gch : %.h
	@echo
	@echo Precompiling Header $<
	$(CCPP) -c $(ALL_CPPFLAGS) $< -o $@

# Compile: create assembler files from C source files.
%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@
	
# Compile: create assembler files from C++ source files.
%.s : %.cpp
	$(CCPP) -S $(ALL_CPPFLAGS) $< -o $@

# Assemble: create object files from assembler source files.
%.o : %.asm
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

# Create preprocessed source for use in sending a bug report.
%.i : %.c
	$(CC) -E -mmcu=$(MCU) -I. $(CFLAGS) $< -o $@
	
# Create preprocessed source for use in sending a bug report.
%.i : %.cpp
	$(CCPP) -E -mmcu=$(MCU) -I. $(CPPFLAGS) $< -o $@

clean_list :
	@echo
	@echo $(MSG_CLEANING)

	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).cof
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(OBJ)
	$(REMOVE) $(LST)
	$(REMOVE) $(CSRC:.c=.s)
	$(REMOVE) $(CSRC:.c=.d)
	$(REMOVE) $(CSRC:.c=.i)
	$(REMOVE) $(CSRC:.c=.o)
	$(REMOVE) $(CSRC:.c=.lst)
	$(REMOVE) $(CPPSRC:.cpp=.s)
	$(REMOVE) $(CPPSRC:.cpp=.d)
	$(REMOVE) $(CPPSRC:.cpp=.i)
	$(REMOVE) $(CPPSRC:.cpp=.o)
	$(REMOVE) $(CPPSRC:.cpp=.lst)
	$(REMOVE) $(PCHSRC:.h=.h.gch)
	$(REMOVE) $(ASRC:.asm=.d)
	$(REMOVE) $(ASRC:.asm=.i)
	$(REMOVE) $(ASRC:.asm=.o)
	$(REMOVE) $(ASRC:.asm=.lst)
	$(REMOVEDIR) .dep

debug:
	if ! pgrep -f "openocd -f ${OPENOCD_CFG} -c init"; then \
		openocd -f "${OPENOCD_CFG}" -c "init; reset init"& \
	fi
	arm-none-eabi-gdb -ex "target remote localhost:3333" "${TARGET}.elf"
	pkill -f "openocd -f ${OPENOCD_CFG} -c init" ||:

flash:
	pkill -f "openocd -f ${OPENOCD_CFG} -c init" ||:
	openocd -f "${OPENOCD_CFG}" -c "program \"${TARGET}.elf\" verify reset exit"

# Include the dependency files.
include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
