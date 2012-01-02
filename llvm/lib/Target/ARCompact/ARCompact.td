//===------- ARCompact.td - Describe the ARCompact Target Machine ---------===//
// 
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// 
//===----------------------------------------------------------------------===//
// This is the top level entry point for the ARCompact target.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces.
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Register File, Calling Convention, Instruction Descriptions.
//===----------------------------------------------------------------------===//

include "ARCompactRegisterInfo.td"
include "ARCompactCallingConv.td"
include "ARCompactInstrInfo.td"

def ARCompactInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// ARCOMPACT supported processors.
//===----------------------------------------------------------------------===//

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def : Proc<"encore", []>;

//===----------------------------------------------------------------------===//
// Target declaration.
//===----------------------------------------------------------------------===//

def ARCompact : Target {
  let InstructionSet = ARCompactInstrInfo;
}
