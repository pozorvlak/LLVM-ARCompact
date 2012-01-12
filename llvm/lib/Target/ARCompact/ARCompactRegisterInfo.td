//===--------- ARCompactRegisterInfo.td - ARCompact Register defs ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the ARCompact register file
//===----------------------------------------------------------------------===//

class ARCompactReg<bits<5> num, string n> : Register<n> {
  field bits<5> Num = num;
  let Namespace = "ARC";
}

//===----------------------------------------------------------------------===//
//  Register definitions
//===----------------------------------------------------------------------===//

// General Purpose Registers
def R0 : ARCompactReg<0, "R0">,    DwarfRegNum<[0]>;
def R1 : ARCompactReg<1, "R1">,    DwarfRegNum<[1]>;
def R2 : ARCompactReg<2, "R2">,    DwarfRegNum<[2]>;
def R3 : ARCompactReg<3, "R3">,    DwarfRegNum<[3]>;
def R4 : ARCompactReg<4, "R4">,    DwarfRegNum<[4]>;
def R5 : ARCompactReg<5, "R5">,    DwarfRegNum<[5]>;
def R6 : ARCompactReg<6, "R6">,    DwarfRegNum<[6]>;
def R7 : ARCompactReg<7, "R7">,    DwarfRegNum<[7]>;

def T0 : ARCompactReg<8, "R8">,    DwarfRegNum<[8]>;
def T1 : ARCompactReg<9, "R9">,    DwarfRegNum<[9]>;
def T2 : ARCompactReg<10, "R10">,  DwarfRegNum<[10]>;
def T3 : ARCompactReg<11, "R11">,  DwarfRegNum<[11]>;
def T4 : ARCompactReg<12, "R12">,  DwarfRegNum<[12]>;
def T5 : ARCompactReg<13, "R13">,  DwarfRegNum<[13]>;
def T6 : ARCompactReg<14, "R14">,  DwarfRegNum<[14]>;
def T7 : ARCompactReg<15, "R15">,  DwarfRegNum<[15]>;

def S0 : ARCompactReg<16, "R16">,  DwarfRegNum<[16]>;
def S1 : ARCompactReg<17, "R17">,  DwarfRegNum<[17]>;
def S2 : ARCompactReg<18, "R18">,  DwarfRegNum<[18]>;
def S3 : ARCompactReg<19, "R19">,  DwarfRegNum<[19]>;
def S4 : ARCompactReg<20, "R20">,  DwarfRegNum<[20]>;
def S5 : ARCompactReg<21, "R21">,  DwarfRegNum<[21]>;
def S6 : ARCompactReg<22, "R22">,  DwarfRegNum<[22]>;
def S7 : ARCompactReg<23, "R23">,  DwarfRegNum<[23]>;
def S8 : ARCompactReg<24, "R24">,  DwarfRegNum<[24]>;
def S9 : ARCompactReg<25, "R25">,  DwarfRegNum<[25]>;

// Global, frame, stack pointers
def GP : ARCompactReg<26, "GP">,  DwarfRegNum<[26]>;
def FP : ARCompactReg<27, "FP">,  DwarfRegNum<[27]>;
def SP : ARCompactReg<28, "SP">,  DwarfRegNum<[28]>;

// Interrupt link registers
def ILINK1 : ARCompactReg<29, "ILINK1">, DwarfRegNum<[29]>;
def ILINK2 : ARCompactReg<30, "ILINK2">, DwarfRegNum<[30]>;

// Branch link register
def BLINK : ARCompactReg<31, "BLINK">, DwarfRegNum<[31]>;

// Status register
def STATUS32 : ARCompactReg<0, "STATUS32">;

//===----------------------------------------------------------------------===//
//  Register classes
//===----------------------------------------------------------------------===//

// General purpose registers.
// Note: These are actually i32 and f32, but fp is currently not
// supported.
def CPURegs : RegisterClass<"ARC", [i32], 32, (add
  R0, R1, R2, R3, R4, R5, R6, R7,
  // Not preserved across procedure calls
  T0, T1, T2, T3, T4, T5, T6, T7,
  // Callee or caller saved (TODO: which is it?)
  S0, S1, S2, S3, S4, S5, S6, S7, S8, S9,
  // Reserved
  GP, FP, SP, ILINK1, ILINK2, BLINK)>;
