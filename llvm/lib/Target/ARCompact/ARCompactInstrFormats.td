//===------ ARCompactInstrFormats.td - ARCompact Instruction Formats ------===//
// 
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// 
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
