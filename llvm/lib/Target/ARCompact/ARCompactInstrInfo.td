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
// Type Profiles.
//===----------------------------------------------------------------------===//

def SDT_ARCCall : SDTypeProfile<0, -1, [SDTCisVT<0, iPTR>]>;
def SDT_ARCCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i32>]>;
def SDT_ARCCallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;

//===----------------------------------------------------------------------===//
// ARCompact-specific node definitions.
//===----------------------------------------------------------------------===//

// Models the return flag of a function.
def ARCretflag : SDNode<"ARCISD::RET_FLAG", SDTNone,
                        [SDNPHasChain, SDNPOptInGlue]>;

def ARCcall : SDNode<"ARCISD::CALL", SDT_ARCCall,
                     [SDNPHasChain, SDNPOutGlue, SDNPOptInGlue, SDNPVariadic]>;
def ARCcallseq_start : 
    SDNode<"ISD::CALLSEQ_START", SDT_ARCCallSeqStart,
           [SDNPHasChain, SDNPOutGlue]>;
def ARCcallseq_end : 
    SDNode<"ISD::CALLSEQ_END", SDT_ARCCallSeqEnd,
           [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

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

// ADJCALLSTACKDOWN/UP implicitly use/def SP because they may be expanded into
// a stack adjustment and the codegen must know that they may modify the stack
// pointer before prolog-epilog rewriting occurs.
let Defs = [SP], Uses = [SP] in {
  def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i32imm:$amt),
                                "#ADJCALLSTACKDOWN",
                                [(ARCcallseq_start timm:$amt)]>;
  def ADJCALLSTACKUP   : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                                "#ADJCALLSTACKUP",
                                [(ARCcallseq_end timm:$amt1, timm:$amt2)]>;
}

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

// ASL - Page 192.
//    Arithmetically shifts the source operand left, and places the result in
//    the destination register. The shift amount can be a constant 1 (in the
//    two-operand version), or an amount given by a second source (in the
//    three-operand version). If a second source is used, only the bottom
//    5 bits of the source will be used.

def ASLr : Pseudo<(outs CPURegs:$dst), (ins CPURegs:$src),
                  "asl $dst,$src",
                  [(set CPURegs:$dst, (shl CPURegs:$src, 1))]>;

def ASLui : Pseudo<(outs CPURegs:$dst), (ins i32imm:$src),
                  "asl $dst,$src",
                  [(set CPURegs:$dst, (shl uimm6:$src, 1))]>;

def ASLli : Pseudo<(outs CPURegs:$dst), (ins i32imm:$src),
                  "asl $dst,$src",
                  [(set CPURegs:$dst, (shl limm32:$src, 1))]>;

// TODO: Model the 5-bit limit somehow?

defm ASL : GenPurposeInst<"asl", shl>;

// ASL also supports a "ASL a,limm,c" format.
def ASLlir : Pseudo<(outs CPURegs:$dst), (ins i32imm:$src1, CPURegs:$src2),
                    "asl $dst,$src1,$src2",
                    [(set CPURegs:$dst, (shl limm32:$src1, CPURegs:$src2))]>;

// ASR - Page 197.
//    Arithmetically shifts the source operand right, and places the result in
//    the destination register. The shift amount can be a constant 1 (in the
//    two-operand version), or an amount given by a second source (in the
//    three-operand version). If a second source is used, only the bottom
//    5 bits of the source will be used.

def ASRr : Pseudo<(outs CPURegs:$dst), (ins CPURegs:$src),
                  "asr $dst,$src",
                  [(set CPURegs:$dst, (sra CPURegs:$src, 1))]>;

def ASRui : Pseudo<(outs CPURegs:$dst), (ins i32imm:$src),
                  "asr $dst,$src",
                  [(set CPURegs:$dst, (sra uimm6:$src, 1))]>;

def ASRli : Pseudo<(outs CPURegs:$dst), (ins i32imm:$src),
                  "asr $dst,$src",
                  [(set CPURegs:$dst, (sra limm32:$src, 1))]>;

// TODO: Model the 5-bit limit somehow?

defm ASR : GenPurposeInst<"asr", sra>;

// ASR also supports a "ASR a,limm,c" format.
def ASRlir : Pseudo<(outs CPURegs:$dst), (ins i32imm:$src1, CPURegs:$src2),
                    "asr $dst,$src1,$src2",
                    [(set CPURegs:$dst, (sra limm32:$src1, CPURegs:$src2))]>;

// BLcc - Page 212.
//    When the specified condition code is met (cc = true in the unconditional
//    case), the program jumps to the given label and the address of the BLcc
//    instruction is stored in the link register BLINK.

// TODO: Add clobbered registers.
// TODO: Should this clobber STATUS32 or not?
// TODO: Add conditional jumps.
let isCall = 1 in {
  // All calls clobber the non-callee saved registers. SP is marked as
  // a use to prevent stack-pointer assignments that appear immediately
  // before calls from potentially appearing dead.
  let Defs = [STATUS32], Uses = [SP] in {
    // Unconditional jump.
    def BLi : Pseudo<(outs), (ins i32imm:$dst, variable_ops),
                       "bl $dst",
                       [(ARCcall imm:$dst)]>;
    def BLr : Pseudo<(outs), (ins CPURegs:$dst, variable_ops),
                       "bl $dst", 
                       [(ARCcall CPURegs:$dst)]>;
  }
}

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

// LSR - Page 254.
//    Logically shifts the source operand right, and places the result in the
//    destination register. The shift amount can be a constant 1 (in the
//    two-operand version), or an amount given by a second source (in the
//    three-operand version). If a second source is used, only the bottom
//    5 bits of the source will be used.

def LSRr : Pseudo<(outs CPURegs:$dst), (ins CPURegs:$src),
                  "lsr $dst,$src",
                  [(set CPURegs:$dst, (srl CPURegs:$src, 1))]>;

def LSRui : Pseudo<(outs CPURegs:$dst), (ins i32imm:$src),
                  "lsr $dst,$src",
                  [(set CPURegs:$dst, (srl uimm6:$src, 1))]>;

def LSRli : Pseudo<(outs CPURegs:$dst), (ins i32imm:$src),
                  "lsr $dst,$src",
                  [(set CPURegs:$dst, (srl limm32:$src, 1))]>;

// TODO: Model the 5-bit limit somehow?

defm LSR : GenPurposeInst<"lsr", srl>;

// LSR also supports a "LSR a,limm,c" format.
def LSRlir : Pseudo<(outs CPURegs:$dst), (ins i32imm:$src1, CPURegs:$src2),
                    "lsr $dst,$src1,$src2",
                    [(set CPURegs:$dst, (srl limm32:$src1, CPURegs:$src2))]>;

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
