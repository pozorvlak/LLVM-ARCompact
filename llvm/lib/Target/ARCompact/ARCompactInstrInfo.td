//===--- ARCompactInstrInfo.td - Target Description for ARCompact Target --===//
// 
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// 
//===----------------------------------------------------------------------===//
//
// This file describes the ARCompact instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//

include "ARCompactInstrFormats.td"

//===----------------------------------------------------------------------===//
// ARCompact-specific node definitions.
//===----------------------------------------------------------------------===//

// Models the return flag of a function.
def ARCretflag : SDNode<"ARCISD::RET_FLAG", SDTNone,
                        [SDNPHasChain, SDNPOptInGlue]>;

//===----------------------------------------------------------------------===//
// ARCompact Complex Pattern Definitions.
//===----------------------------------------------------------------------===//

// Patterns which match different immediate types.
// TODO: Are long-immediates signed or unsigned?
def uimm6 : PatLeaf<(imm), [{ return isUInt<6>(N->getZExtValue()); }]>;
def simm9 : PatLeaf<(imm), [{ return isInt<9>(N->getSExtValue()); }]>;
def simm12 : PatLeaf<(imm), [{ return isInt<12>(N->getSExtValue()); }]>;
def limm32 : PatLeaf<(imm), [{ return isInt<32>(N->getSExtValue()); }]>;

// Patterns which match addressing modes.
def ADDRri : ComplexPattern<i32, 2, "SelectADDRri", [frameindex], []>;
def ADDRli : ComplexPattern<i32, 1, "SelectADDRli", [frameindex], []>;
def ADDRrr : ComplexPattern<i32, 2, "SelectADDRrr", [frameindex], []>;
def ADDRrli : ComplexPattern<i32, 2, "SelectADDRrli", [frameindex], []>;
def ADDRlir : ComplexPattern<i32, 2, "SelectADDRlir", [frameindex], []>;

//===----------------------------------------------------------------------===//
// ARCompact Operand Definitions.
//===----------------------------------------------------------------------===//

// Memory addressing operands.
def MEMri : Operand<i32> {  // Register + signed immediate.
  let PrintMethod = "printMemOperand";
  let MIOperandInfo = (ops CPURegs, simm9);
}
def MEMli : Operand<i32> {  // Long immediate.
  let PrintMethod = "printMemOperand";
  let MIOperandInfo = (ops limm32);
}
def MEMrr : Operand<i32> {  // Register + register.
  let PrintMethod = "printMemOperand";
    let MIOperandInfo = (ops CPURegs, CPURegs);
}
def MEMrli : Operand<i32> {  // Register + long immediate.
  let PrintMethod = "printMemOperand";
    let MIOperandInfo = (ops CPURegs, limm32);
}
def MEMlir : Operand<i32> {  // Long immediate + register.
  let PrintMethod = "printMemOperand";
    let MIOperandInfo = (ops limm32, CPURegs);
}

//===----------------------------------------------------------------------===//
// ARCompact Instructions.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Misc Instructions.
//

// The manual defines the preferred NOP asm as mov 0,0.
def NOP : Pseudo<(outs), (ins), "mov 0,0", []>;

// A return is modelled as an explicit jump from BLINK.
let isReturn = 1, isTerminator = 1, hasDelaySlot = 1, isBarrier = 1 in {
    def RET : Pseudo<(outs), (ins), "j.d [blink]", [(ARCretflag)]>;
}

//===----------------------------------------------------------------------===//
// General Instructions - Alphabetical Order.
//

// ADD - Page 180.
//    Add two source operands together, and place the result in the destination
//    register.

def ADDrr : Pseudo<(outs CPURegs:$dst), (ins CPURegs:$src1, CPURegs:$src2),
                   "add $dst,$src1,$src2",
                   [(set CPURegs:$dst, (add CPURegs:$src1, CPURegs:$src2))]>;

// LD - Page 239.
//    Loads the value stored at a memory address into the destination register.
//    The memory address may be given as a single long immediate, or as two
//    operands whose values will be added together to get the address.
//
//    The suffixes given here refer to the memory address format - for example,
//    ri means an address given by a register + a signed 9-bit immediate.

def LDri : Pseudo<(outs CPURegs:$dst), (ins MEMri:$addr),
                  "ld $dst,$addr",
                  [(set CPURegs:$dst, (load ADDRri:$addr))]>;

def LDli : Pseudo<(outs CPURegs:$dst), (ins MEMli:$addr),
                   "ld $dst,$addr",
                   [(set CPURegs:$dst, (load ADDRli:$addr))]>;

def LDrr : Pseudo<(outs CPURegs:$dst), (ins MEMrr:$addr),
                    "ld $dst,$addr",
                    [(set CPURegs:$dst, (load ADDRrr:$addr))]>;

def LDrli : Pseudo<(outs CPURegs:$dst), (ins MEMrli:$addr),
                    "ld $dst,$addr",
                    [(set CPURegs:$dst, (load ADDRrli:$addr))]>;

def LDlir : Pseudo<(outs CPURegs:$dst), (ins MEMlir:$addr),
                    "ld $dst,$addr",
                    [(set CPURegs:$dst, (load ADDRlir:$addr))]>;

// MOV - Page 262.
//    The contents of the source are moved into the destination register.

// No pattern is defined for register-to-register moves, as LLVM is unable to 
// match them. Instead, copyPhysReg in ARCompactInstrInfo.cpp is responsible 
// for emitting this instruction when appropriate.
let neverHasSideEffects = 1 in {
  def MOVrr : Pseudo<(outs CPURegs:$dst), (ins CPURegs:$src),
                     "mov $dst,$src",
                     []>;
}

let isAsCheapAsAMove = 1 in {
  def MOVrui : Pseudo<(outs CPURegs:$dst), (ins i32imm:$imm),
                      "mov $dst,$imm",
                      [(set CPURegs:$dst, uimm6:$imm)]>;

  def MOVrsi : Pseudo<(outs CPURegs:$dst), (ins i32imm:$imm),
                      "mov $dst,$imm",
                      [(set CPURegs:$dst, simm12:$imm)]>;

  def MOVrli : Pseudo<(outs CPURegs:$dst), (ins i32imm:$imm),
                      "mov $dst,$imm",
                      [(set CPURegs:$dst, limm32:$imm)]>;
}

// ST - page 310.
//    Stores the value stored in the source operand in the destination memory
//    address. The source operand may either be a register or a long immediate
//    value. The memory address may be given as a single long immediate, or
//    as two operands whose values will be added together to get the address.
//
//    The suffixes given here refer to the whole instruction format - for
//    example, rri means store a value from a register into an address given by
//    a register + a signed 9-bit immediate.

def STrri : Pseudo<(outs), (ins MEMri:$addr, CPURegs:$src),
                  "st $src,$addr",
                  [(store CPURegs:$src, ADDRri:$addr)]>;

def STrli : Pseudo<(outs), (ins MEMli:$addr, CPURegs:$src),
                  "st $src,$addr",
                  [(store CPURegs:$src, ADDRli:$addr)]>;

def STliri : Pseudo<(outs), (ins MEMri:$addr, i32imm:$src),
                    "st $src,$addr",
                    [(store limm32:$src, ADDRri:$addr)]>;
