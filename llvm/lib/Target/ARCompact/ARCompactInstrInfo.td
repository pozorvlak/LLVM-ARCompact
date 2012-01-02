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
