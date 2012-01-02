//====----- ARCompactFrameLowering.cpp - ARCompact Frame Information -----====//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the ARCompact implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "ARCompactFrameLowering.h"
#include "ARCompactInstrInfo.h"
#include "llvm/Function.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Metadata.h"
#include "llvm/LLVMContext.h"
#include "llvm/ADT/ArrayRef.h"

#include <iostream>

using namespace llvm;

void ARCompactFrameLowering::emitPrologue(MachineFunction &MF) const {
  llvm_unreachable("Not yet implemented!");
}

void ARCompactFrameLowering::emitEpilogue(MachineFunction &MF,
    MachineBasicBlock &MBB) const {
  llvm_unreachable("Not yet implemented!");
}

bool ARCompactFrameLowering::hasFP(const MachineFunction &MF) const {
  return (DisableFramePointerElim(MF) ||
      MF.getFrameInfo()->hasVarSizedObjects());
}
