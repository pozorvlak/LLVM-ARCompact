//===------ ARCompactInstrFormats.td - ARCompact Instruction Formats ------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// ARCompact Special Operands
//===----------------------------------------------------------------------===//

// ARCompact Predicate operand. Default to 0 = always (AL). The second part is
// the condition code register which defaults to 0 (no register).
// TODO: Is DecodeMethod necessary if we do not have a disassembly feature?
def CondCodeOperand : AsmOperandClass { let Name = "CondCode"; }
def pred : PredicateOperand<OtherVT, (ops i32imm, i32imm),
                                     (ops (i32 0), (i32 zero_reg))> {
  let PrintMethod = "printPredicateOperand";
  let ParserMatchClass = CondCodeOperand;
  let DecoderMethod = "DecodePredicateOperand";
}

//===----------------------------------------------------------------------===//
// ARCompact Instruction Classes
//===----------------------------------------------------------------------===//

// Represents a generic ARCompact instruction.
class ARCInst<dag outs, dag ins, string asmstr, list<dag> pattern>
    : Instruction {
  field bits<32> Inst;

  let Namespace = "ARC";

  // The top five bits are the major opcode.
  bits<5> op;
  let Inst{31-27} = op;

  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = asmstr;
  let Pattern = pattern;
}

class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern>
    : ARCInst<outs, ins, asmstr, pattern> {
  let op = 0;

  let Inst{26-0} = 0;
}
