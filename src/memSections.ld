/******************************************************************************\
    Split Keyboard
    Copyright (C) 2019-2025 - Max Maisel

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
\******************************************************************************/

MEMORY
{
    FLASH (RX) : ORIGIN = 0x08000000, LENGTH = 0x00080000
    SRAM  (WX) : ORIGIN = 0x20000000, LENGTH = 0x00018000
}

ENTRY(reset_vector)

SECTIONS
{
    .inttable 0x08000000 :
    {
        KEEP (*(.inttable))
    } > FLASH

    .vtable :
    {
        KEEP(*(.vtable))
    } > FLASH

    .text :
    {
        *(.text)
        /**(.text.*)*/
        . = ALIGN(4);
        __init_array_start__ = .;
        KEEP (*(.init_array*))
        __init_array_end__ = .;
        KEEP (*(.init*));
        KEEP (*(.fini*));
    } > FLASH

    .rodata :
    {
        *(.rodata)
        *(.rodata*)
    } > FLASH

    .stack 0x20000000 : ALIGN(8)
    {
        __stack_bot__ = .;
        . = . + 0x1000;
        __stack_top__ = .;
    } > SRAM

    .data : ALIGN(4)
    {
        __data_load__ = LOADADDR(.data);
        __data_start__ = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } > SRAM AT> FLASH

    .ARM.exid :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    .bss : ALIGN(4)
    {
        __bss_start__ = .;
        *(.shbss)
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > SRAM

    .heap_start : ALIGN(4)
    {
        __heap_start__ = .;
    } > SRAM

    .heap_end 0x20018000 :
    {
        __heap_end__ = .;
    } > SRAM
}
