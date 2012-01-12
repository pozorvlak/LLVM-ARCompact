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

#include <string>

using namespace llvm;

/**
 * This diagram was taken from the GCC backend.
 *
 * ARCompact stack frames look like:
 *
 *                Before call                      After call
 *   high  +-----------------------+       +-----------------------+
 *   mem   |  reg parm save area   |       | reg parm save area    |
 *         |  only created for     |       | only created for      |
 *         |  variable arg fns     |       | variable arg fns      |
 *     AP  +-----------------------+       +-----------------------+
 *         |  return addr register |       | return addr register  |
 *         |  (if required)        |       | (if required)         |
 *         +-----------------------+       +-----------------------+
 *         |                       |       |                       |
 *         |  reg save area        |       | reg save area         |
 *         |                       |       |                       |
 *         +-----------------------+       +-----------------------+
 *         |  frame pointer        |       | frame pointer         |
 *         |  (if required)        |       | (if required)         |
 *     FP  +-----------------------+       +-----------------------+
 *         |                       |       |                       |
 *         |  local/temp variables |       | local/temp variables  |
 *         |                       |       |                       |
 *         +-----------------------+       +-----------------------+
 *         |                       |       |                       |
 *         |  arguments on stack   |       | arguments on stack    |
 *         |                       |       |                       |
 *     SP  +-----------------------+       +-----------------------+
 *                                         | reg parm save area    |
 *                                         | only created for      |
 *                                         | variable arg fns      |
 *                                     AP  +-----------------------+
 *                                         | return addr register  |
 *                                         | (if required)         |
 *                                         +-----------------------+
 *                                         |                       |
 *                                         | reg save area         |
 *                                         |                       |
 *                                         +-----------------------+
 *                                         | frame pointer         |
 *                                         | (if required)         |
 *                                     FP  +-----------------------+
 *                                         |                       |
 *                                         | local/temp variables  |
 *                                         |                       |
 *                                         +-----------------------+
 *                                         |                       |
 *                                         | arguments on stack    |
 *   low                                   |                       |
 *   mem                               SP  +-----------------------+
 */

// Emits a manual ARC::COMMENT node, allowing the insertion of comments
// into generated code.
static void EmitComment(MachineBasicBlock &MBB,
    MachineBasicBlock::iterator MBBI, DebugLoc dl,
    const ARCompactInstrInfo &TII, std::string Comment) {
  // A comment is wrapped in a MetaData node.
  MDNode* CommentMDNode = MDNode::get(getGlobalContext(),
      ArrayRef<Value*>(MDString::get(getGlobalContext(), Comment)));

  BuildMI(MBB, MBBI, dl, TII.get(ARC::COMMENT)).addMetadata(CommentMDNode);
}

void ARCompactFrameLowering::emitPrologue(MachineFunction &MF) const {
  MachineBasicBlock &MBB = MF.front();
  MachineBasicBlock::iterator MBBI = MBB.begin();
  MachineFrameInfo  *MFI = MF.getFrameInfo();
  const ARCompactInstrInfo &TII =
      *static_cast<const ARCompactInstrInfo*>(MF.getTarget().getInstrInfo());

  DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

  assert(!MFI->hasVarSizedObjects() && "Variadic functions not supported yet!");

  // Start of prologue comment.
  EmitComment(MBB, MBBI, dl, TII, "PROLOGUE START");

  // The stack size should be 4-byte aligned.
  unsigned int NumBytes = MFI->getStackSize();
  NumBytes = (NumBytes + 3) & ~3;

  // TODO: Allocate space for variadic function arguments.

  // Save the return address register, if necessary
  if (MFI->adjustsStack()) {
    BuildMI(MBB, MBBI, dl, TII.get(ARC::STrri)).addReg(ARC::SP)
        .addImm(-UNITS_PER_WORD).addReg(ARC::BLINK);
    BuildMI(MBB, MBBI, dl, TII.get(ARC::SUBrui), ARC::SP).addReg(ARC::SP)
        .addImm(UNITS_PER_WORD);
  }

  // TODO: Create the register save area, and save the required registers to it.

  // Save the caller's frame pointer (if required), and set new FP to this
  // location.
  // TODO: Work out if the frame pointer is required.
  BuildMI(MBB, MBBI, dl, TII.get(ARC::STrri)).addReg(ARC::SP)
      .addImm(-UNITS_PER_WORD).addReg(ARC::FP);
  BuildMI(MBB, MBBI, dl, TII.get(ARC::SUBrui), ARC::SP).addReg(ARC::SP)
      .addImm(UNITS_PER_WORD);
  BuildMI(MBB, MBBI, dl, TII.get(ARC::MOVrr), ARC::FP).addReg(ARC::SP);

  // Allocate space for local registers.
  if (NumBytes > 0) {
    // Check we're 4-byte aligned.
    assert((NumBytes & 0x3) == 0);
    BuildMI(MBB, MBBI, dl, TII.get(ARC::SUBrsi), ARC::SP).addReg(ARC::SP)
        .addImm(NumBytes);
  }

  // End of prologue comment.
  EmitComment(MBB, MBBI, dl, TII, "END PROLOGUE");
}

void ARCompactFrameLowering::emitEpilogue(MachineFunction &MF,
    MachineBasicBlock &MBB) const {
  /* This code adapted from GCC and other llvm backends. */
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  MachineFrameInfo  *MFI = MF.getFrameInfo();
  const ARCompactInstrInfo &TII =
      *static_cast<const ARCompactInstrInfo*>(MF.getTarget().getInstrInfo());

  DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

  assert(!MFI->hasVarSizedObjects() && "Variadic functions not supported yet!");

  // Make sure we are putting the epilogue in the right place.
  assert(MBBI->getDesc().isReturn() &&
      "Can only put epilogue before a return instruction!");

  // Start of epilogue comment.
  EmitComment(MBB, MBBI, dl, TII, "START EPILOGUE");

  // The stack size should be 4-byte aligned.
  unsigned NumBytes = MFI->getStackSize();
  NumBytes = (NumBytes + 3) & ~3;

  // Restore stack pointer to the beginning of saved register area
  if (NumBytes > 0) {
    BuildMI(MBB, MBBI, dl, TII.get(ARC::ADDrsi), ARC::SP).addReg(ARC::SP)
        .addImm(NumBytes);
  }

  // TODO: Restore any registers from the register-save area.

  // Load the frame pointer back.
  // TODO: Work out if the frame pointer was needed.
  BuildMI(MBB, MBBI, dl, TII.get(ARC::ADDrsi), ARC::SP).addReg(ARC::SP)
      .addImm(UNITS_PER_WORD);
  BuildMI(MBB, MBBI, dl, TII.get(ARC::LDri)).addReg(ARC::FP).addReg(ARC::SP)
      .addImm(-UNITS_PER_WORD);

  // Restore the return address register, if needed.
  if (MFI->adjustsStack()) {
    BuildMI(MBB, MBBI, dl, TII.get(ARC::ADDrsi), ARC::SP).addReg(ARC::SP)
        .addImm(UNITS_PER_WORD);
    BuildMI(MBB, MBBI, dl, TII.get(ARC::LDri)).addReg(ARC::SP)
        .addReg(ARC::BLINK).addImm(-UNITS_PER_WORD);
  }

  // End of epilogue comment.
  MBBI++;
  EmitComment(MBB, MBBI, dl, TII, "END EPILOGUE");
}

bool ARCompactFrameLowering::hasFP(const MachineFunction &MF) const {
  return (DisableFramePointerElim(MF) ||
      MF.getFrameInfo()->hasVarSizedObjects());
}
