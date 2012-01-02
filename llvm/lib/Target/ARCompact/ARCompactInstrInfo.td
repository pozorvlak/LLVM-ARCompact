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
// ARCompact Instructions.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Misc Instructions.
//

// The manual defines the preferred NOP asm as mov 0,0.
def NOP : Pseudo<(outs), (ins), "mov 0,0", []>;
