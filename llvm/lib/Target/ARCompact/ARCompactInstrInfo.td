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
// ARCompact Multi-Classes.
//===----------------------------------------------------------------------===//

// Captures ARCompact general purpose instructions, which have the following 
// types:
//
//   * Register1 = Register2 op Register3
//   * Register1 = Register2 op Unsigned Immediate
//   * Register1 = Register1 op Signed Immediate
//   * Register1 = Register2 op Long Immediate
//
multiclass GenPurposeInst<string opstring, SDNode OpNode> {
  def rr : Pseudo<(outs CPURegs:$dst), (ins CPURegs:$src1, CPURegs:$src2),
                      !strconcat(opstring, " $dst,$src1,$src2"),
                      [(set CPURegs:$dst,
                          (OpNode CPURegs:$src1, CPURegs:$src2))]>;

  def rui : Pseudo<(outs CPURegs:$dst), (ins CPURegs:$src1, i32imm:$src2),
                      !strconcat(opstring, " $dst,$src1,$src2"),
                      [(set CPURegs:$dst,
                          (OpNode CPURegs:$src1, uimm6:$src2))]>;

  // In the signed-immediate case, the source and destination registers must be
  // the same register, due to encoding constraints.
  let Constraints = "$src1 = $dst" in { 
    def rsi : Pseudo<(outs CPURegs:$dst), (ins CPURegs:$src1, i32imm:$src2),
                        !strconcat(opstring, " $dst,$src1,$src2"),
                        [(set CPURegs:$dst,
                            (OpNode CPURegs:$src1, simm12:$src2))]>;
  }

  def rli : Pseudo<(outs CPURegs:$dst), (ins CPURegs:$src1, i32imm:$src2),
                      !strconcat(opstring, " $dst,$src1,$src2"),
                      [(set CPURegs:$dst,
                          (OpNode CPURegs:$src1, limm32:$src2))]>;
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

// ADC - Page 179.
//    Add two source operands together, along with the carry value, and place
//    the result in the destination register.

// TODO: Does this actually use STATUS32?
let Uses = [STATUS32] in {
    defm ADC : GenPurposeInst<"adc", adde>;
}

// ADD - Page 180.
//    Add two source operands together, and place the result in the destination
//    register.

defm ADD : GenPurposeInst<"add", add>;

// AND - Page 191.
//    Takes the logical bitwise AND of two source operands, and places the
//    result into the destination register.

defm AND : GenPurposeInst<"and", and>;

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

// NEG - page 275.
//    The source value is subtracted from 0 and the result placed in the
//    destination register.
//
//    NEG only has a register-to-register version.

def NEGrr : Pseudo<(outs CPURegs:$dst), (ins CPURegs:$src),
                   "neg $dst,$src",
                   [(set CPURegs:$dst, (ineg CPURegs:$src))]>;

// NOT - page 283.
//    Takes the logical bitwise NOT of the source operand, and places the
//    result into the destination register.

def NOTrr : Pseudo<(outs CPURegs:$dst), (ins CPURegs:$src),
                   "not $dst,$src",
                   [(set CPURegs:$dst, (not CPURegs:$src))]>;

def NOTrui : Pseudo<(outs CPURegs:$dst), (ins i32imm:$src),
                    "not $dst,$src",
                    [(set CPURegs:$dst, (not uimm6:$src))]>;

def NOTrli : Pseudo<(outs CPURegs:$dst), (ins i32imm:$src),
                    "not $dst,$src",
                    [(set CPURegs:$dst, (not uimm6:$src))]>;

// OR - page 284.
//    Takes the logical bitwise OR of the source operands, and places the
//    result into the destination register.

defm OR : GenPurposeInst<"or", or>;

// SBC - page 302.
//    Subtracts the second source operand from the first, and then also
//    subtracts the carry from this value. The result is placed in the 
//    destination register.

// TODO: Does this actually use STATUS32?
let Uses = [STATUS32] in {
    defm SBC : GenPurposeInst<"sbc", sube>;
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

// SUB - Page 312.
//    Subtracts the second source operand from the first, and places the result
//    into the destination register.

defm SUB : GenPurposeInst<"sub", sub>;

// SUB also supports a "SUB a,limm,c" format.
def SUBlir : Pseudo<(outs CPURegs:$dst), (ins i32imm:$src1, CPURegs:$src2),
                      "sub $dst,$src1,$src2",
                      [(set CPURegs:$dst, (sub limm32:$src1, CPURegs:$src2))]>;

// XOR - page 332.
//    Takes the logical bitwise XOR of the source operands, and places the
//    result into the destination register.

defm XOR : GenPurposeInst<"xor", xor>;
