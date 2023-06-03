# Chip-8 Emulator
This is a Chip-8 Emulator program coded in C programming language.



### Overview of Chip-8
- Chip-8 is an interpreter from mid 1970s
- It was designed to make the devolopments of the games easier
- Chip-8 is essentially a virtual machine like JVM but something far more basic
- Chip-8 interpreters can play many classical games such as pong, tetris and space invaders

### The Memory Layout of Chip-8

- The Chip-8 language is capable of accessing up to 4KB (4,096 bytes) of RAM, from location 0x000 (0) to 0xFFF (4095).
- The first 512 bytes, from 0x000 to 0x1FF, are where the original interpreter was located, and should not be used by programs.

*Most Chip-8 programs start at location 0x200 (512), but some begin at 0x600 (1536). Programs beginning at 0x600 are intended for the ETI 660 computer.*


```
+---------------+= 0xFFF (4095) End of Chip-8 RAM
|               |
|               |
|               |
|               |
|               |
| 0x200 to 0xFFF|
|     Chip-8    |
| Program / Data|
|     Space     |
|               |
|               |
|               |
+- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
|               |
|               |
|               |
+---------------+= 0x200 (512) Start of most Chip-8 programs
| 0x000 to 0x1FF|
| Reserved for  |
|  interpreter  |
+---------------+= 0x000 (0) Start of Chip-8 RAM
```
### Stack of Chip-8

Chip-8 has a stack (LIFO) that is an array of 16, 16-bit values which is used to store the address that the Chip-8 should return to when returning from a subroutine. Chip-8 allows for up to 16 levels of nested subroutines.

*Note: The stack is not part of main memory; it is seperate from it.*

### The Registers

- Chip-8 has 16 general purpose 8-bit registers, usually referred to as Vx, where x is a hexadecimal digit (0 through F).
[ V0, V1, V2, V3 V4, V5, V6, V7, V8, V9, VA, VB, VC, VD, VE, VF ]
- As these are 8-bit registers it can hold 1 byte of information per register.
- Chip-8 data registers should be implemented as a character array with size of 16.
- There is also a 16-bit register called I. This register is generally used to store memory addresses.
- Chip-8 also has a 16-bit PC (Program Counter) registers which points to the memory address of current instruction being executed.
In Chip-8 instructions are 2-bytes of size and that the reason why we increment by 2.
- Chip-8 also has an 8-bit SP (Stack Pointer) that points the location of stack.
- Chip-8 also has two special purpose 8-bit registers, for the delay and sound timers. When these registers are non-zero, they are automatically decremented at a rate of 60Hz. 


### The Sound Timer
- The sound timer is active whenever the sound timer register (ST) is non-zero.
- It plays a bleep. (This is implementation specific; you pick the frequency of the bleep)
- Decrements at a rate of 60Hz.
- When it reaches 0 the sound no longer plays.

### The Delay Timer
- Chip-8 stops executing instructions when the delay timer is above 0.
- Decrements at a rate of 60Hz.
- When the delay timer is 0 again the program execution resumes.

### Display
- Chip-8 display size is 64 X 32 pixels.
- It is a monochrome display.
- The pixel at [0, 0] corresponds to the top-left corner, and the pixel [63, 31] corresponds to the bottom-right.

```
  [0,0]                      [63,0]     
  ┌───────────────────────────────┐
  │                               │
  │                               │
  │         64x32 DISPLAY         │
  │                               │
  │                               │
  └───────────────────────────────┘
  [0,31]                    [63,31]     
```

### Drawing
- Drawing to the Chip-8 display is done with the sprites not pixels.
- If a sprite overflows the screen wraps it back around to the other side.

### Sprites
- Sprites are many pixels grouped together that can be drawn to the screen as a whole.
- It is interpreted as a row of bits that encode the on/off state of each of the pixels.

```
  HEX          BINARY          IMAGE
  
  0XFF        11111111        ■■■■■■■■
  0X99        10011001        ■  ■■  ■
  0X99        10011001        ■  ■■  ■
  0XFF   =>   11111111   =>   ■■■■■■■■
  0X99        10011001        ■  ■■  ■
  0X99        10011001        ■  ■■  ■
  0XFF        11111111        ■■■■■■■■
```
- Notice each binary bit represents one pixel of sprite. 0 means pixel is not set, 1 means pixel is set.
- Sprites can be a maximum of 8 bits in width (X axis) and up to 15 bytes of length (Y axis).
- Sprites get drawn to the Chip-8 display if the pixels go out of bounds (x>=64 || y>=32) then the breaching pixels warp back around to the start of the display. x coordinte to 0 or y coordinate to 0.
- Sprites get XOR'ed onto the display if this causes any pixels to be erased then the VF register is set.

### The Keyboard
- The Chip-8 keyboard has 16 keys from 0 to F with the following layout:
```
1	2	3	C
4	5	6	D
7	8	9	E
A	0	B	F
```
Of course, nowadays nobody expects anyone to have one of these physical keypads, so emulators typically map it to the keyboard. Usually, the map is done like this:
```
1	2	3	4
Q	W	E	R
A	S	D	F
Z	X	C	V
```
- This can be represented in the emulator as a 16 byte array.
- With a true value in the array means the key is pressed and a false value meaning the key is not pressed.

### The Instruction Set
- Chip-8 instructions has 36 different instructions that need to be implemented for a successful implementation.
- The instruction set has instructions for mathematical operations, drawing and much more.
- All instructions are 2 bytes long and are stored most-significant-byte first. In memory, the first byte of each instruction should be located at an even addresses. If a program includes sprite data, it should be padded so any instructions following it will be properly situated in RAM.
```
In these listings, the following variables are used:

nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
n or nibble - A 4-bit value, the lowest 4 bits of the instruction
x - A 4-bit value, the lower 4 bits of the high byte of the instruction
y - A 4-bit value, the upper 4 bits of the low byte of the instruction
kk or byte - An 8-bit value, the lowest 8 bits of the instruction
```
```
0nnn - SYS addr
Jump to a machine code routine at nnn.

This instruction is only used on the old computers on which Chip-8 was originally implemented. It is ignored by modern interpreters.


00E0 - CLS
Clear the display.


00EE - RET
Return from a subroutine.

The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.


1nnn - JP addr
Jump to location nnn.

The interpreter sets the program counter to nnn.


2nnn - CALL addr
Call subroutine at nnn.

The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.


3xkk - SE Vx, byte
Skip next instruction if Vx = kk.

The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.


4xkk - SNE Vx, byte
Skip next instruction if Vx != kk.

The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.


5xy0 - SE Vx, Vy
Skip next instruction if Vx = Vy.

The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.


6xkk - LD Vx, byte
Set Vx = kk.

The interpreter puts the value kk into register Vx.


7xkk - ADD Vx, byte
Set Vx = Vx + kk.

Adds the value kk to the value of register Vx, then stores the result in Vx.

8xy0 - LD Vx, Vy
Set Vx = Vy.

Stores the value of register Vy in register Vx.


8xy1 - OR Vx, Vy
Set Vx = Vx OR Vy.

Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0.


8xy2 - AND Vx, Vy
Set Vx = Vx AND Vy.

Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1. Otherwise, it is 0.


8xy3 - XOR Vx, Vy
Set Vx = Vx XOR Vy.

Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0.


8xy4 - ADD Vx, Vy
Set Vx = Vx + Vy, set VF = carry.

The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.


8xy5 - SUB Vx, Vy
Set Vx = Vx - Vy, set VF = NOT borrow.

If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.


8xy6 - SHR Vx {, Vy}
Set Vx = Vx SHR 1.

If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.


8xy7 - SUBN Vx, Vy
Set Vx = Vy - Vx, set VF = NOT borrow.

If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.


8xyE - SHL Vx {, Vy}
Set Vx = Vx SHL 1.

If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.


9xy0 - SNE Vx, Vy
Skip next instruction if Vx != Vy.

The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.


Annn - LD I, addr
Set I = nnn.

The value of register I is set to nnn.


Bnnn - JP V0, addr
Jump to location nnn + V0.

The program counter is set to nnn plus the value of V0.


Cxkk - RND Vx, byte
Set Vx = random byte AND kk.

The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx. See instruction 8xy2 for more information on AND.


Dxyn - DRW Vx, Vy, nibble
Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.


Ex9E - SKP Vx
Skip next instruction if key with the value of Vx is pressed.

Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.


ExA1 - SKNP Vx
Skip next instruction if key with the value of Vx is not pressed.

Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.


Fx07 - LD Vx, DT
Set Vx = delay timer value.

The value of DT is placed into Vx.


Fx0A - LD Vx, K
Wait for a key press, store the value of the key in Vx.

All execution stops until a key is pressed, then the value of that key is stored in Vx.


Fx15 - LD DT, Vx
Set delay timer = Vx.

DT is set equal to the value of Vx.


Fx18 - LD ST, Vx
Set sound timer = Vx.

ST is set equal to the value of Vx.


Fx1E - ADD I, Vx
Set I = I + Vx.

The values of I and Vx are added, and the results are stored in I.


Fx29 - LD F, Vx
Set I = location of sprite for digit Vx.

The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx. See section 2.4, Display, for more information on the Chip-8 hexadecimal font.


Fx33 - LD B, Vx
Store BCD representation of Vx in memory locations I, I+1, and I+2.

The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.


Fx55 - LD [I], Vx
Store registers V0 through Vx in memory starting at location I.

The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.


Fx65 - LD Vx, [I]
Read registers V0 through Vx from memory starting at location I.

The interpreter reads values from memory starting at location I into registers V0 through Vx.

```
### References
```
http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
https://tonisagrista.com/blog/2021/chip8-spec/
https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
```
