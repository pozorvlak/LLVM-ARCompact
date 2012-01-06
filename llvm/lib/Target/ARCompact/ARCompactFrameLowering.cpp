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

/**
 * From GCC. Not checked if correct.
 *
 * ARCompact stack frames look like:
 *
 *            Before call                     After call
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

/* Taken from the ARC GCC Compiler description (not ARCompact, but close.)
 *
 * Prologue and Epilogue
 * The called routine is responsible for allocating its own stack frame, making
 * sure to preserve 4-byte alignment on the stack. This action is accomplished
 * by a section of code called the prologue, which the compiler places before
 * the body of the routine. After the body of the routine, the compiler
 * generates an epilogue to restore the registers saved in the prologue code,
 * previous stack frame and to return to the caller.
 *
 * The compiler-generated prologue code does the following:
 *   • Allocates space for register arguments in case of variadic function
 *     (functions with variable argument lists).
 *   • Saves the return address register (BLINK).
 *   • Saves required non-volatile general-purpose registers into the register
 *     save area.
 *   • Saves the caller’s frame pointer (FP), if required, and sets the new
 *     frame pointer to this location.
 *   • Decrements the stack pointer (SP) to account for the new stack frame.
 *
 * At the end of the function, the compiler-generated epilogue does the
 * following:
 *   • Restores the stack pointer (SP) to the beginning of the saved register
 *     area.
 *   • Restores all non-volatile registers that were saved in the register area.
 *   • Restores caller’s frame pointer register (FP), if required.
 *   • Restores the return address register (BLINK).
 *   • Restores caller’s stack pointer register (SP).
 *   • Returns to caller through the address stored in the blink register.
 *
 * Parameter Passing
 * Parameters are passed by pushing them onto the stack. The first 8 words (32
 * bytes) of arguments are loaded into registers r0 to r7. The remaining
 * arguments are passed by storing them into the stack immediately above the
 * stack pointer register (sp).
 *
 * Return Value
 *   • In the ARCtangent runtime environment, function values are returned in
 *     register r0.
 *   • A doubleword result (double or long double) is returned in registers r0
 *     to r1.
 *   • Structures that can be contained in a single 4-byte register are returned
 *     in r0.
 *   • All other structures are returned by storing them at an address passed to
 *     the function as a hidden first argument.
 *   • Sixty-four-bit integers (long long) are returned in registers r0 to r1.
 *
 */

void ARCompactFrameLowering::emitPrologue(MachineFunction &MF) const {
  /* This code adapted from GCC and other llvm backends. */
  MachineBasicBlock &MBB = MF.front();
  MachineBasicBlock::iterator MBBI = MBB.begin();
  MachineFrameInfo  *MFI = MF.getFrameInfo();
  MachineRegisterInfo &MRI = MF.getRegInfo();
  const ARCompactInstrInfo &TII =
      *static_cast<const ARCompactInstrInfo*>(MF.getTarget().getInstrInfo());

  DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

  assert(!MFI->hasVarSizedObjects() && "Variadic functions not supported yet!");

  // Start of prologue comment.
  MDNode* start_prologue_mdnode = MDNode::get(getGlobalContext(),
      ArrayRef<Value*>(MDString::get(getGlobalContext(), "PROLOGUE START")));
  BuildMI(MBB, MBBI, dl, TII.get(ARC::COMMENT))
      .addMetadata(start_prologue_mdnode);

  // The stack size should be 4-byte aligned.
  unsigned int NumBytes = MFI->getStackSize();
  NumBytes = (NumBytes + 3) & ~3;

  // TODO: Allocate space for variadic function arguments.

  // Save the return address register, if necessary
  if (MRI.isLiveIn(ARC::BLINK) || MRI.isLiveOut(ARC::BLINK)) {
    BuildMI(MBB, MBBI, dl, TII.get(ARC::SUBrui), ARC::SP).addReg(ARC::SP)
        .addImm(UNITS_PER_WORD);
    BuildMI(MBB, MBBI, dl, TII.get(ARC::STrri)).addReg(ARC::SP)
        .addImm(-UNITS_PER_WORD).addReg(ARC::BLINK);
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
    // Check we're 4 aligned.
    assert((NumBytes & 0x3) == 0);
    BuildMI(MBB, MBBI, dl, TII.get(ARC::SUBrsi), ARC::SP).addReg(ARC::SP)
        .addImm(NumBytes);
  }

  // End of prologue comment.
  MDNode* end_prologue_mdnode = MDNode::get(getGlobalContext(),
      ArrayRef<Value*>(MDString::get(getGlobalContext(), "PROLOGUE END")));
  BuildMI(MBB, MBBI, dl, TII.get(ARC::COMMENT))
      .addMetadata(end_prologue_mdnode);
}

void ARCompactFrameLowering::emitEpilogue(MachineFunction &MF,
    MachineBasicBlock &MBB) const {
  /* This code adapted from GCC and other llvm backends. */
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  MachineFrameInfo  *MFI = MF.getFrameInfo();
  MachineRegisterInfo &MRI = MF.getRegInfo();
  const ARCompactInstrInfo &TII =
      *static_cast<const ARCompactInstrInfo*>(MF.getTarget().getInstrInfo());

  DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

  assert(!MFI->hasVarSizedObjects() && "Variadic functions not supported yet!");

  // Make sure we are putting the epilogue in the right place.
  assert(MBBI->getDesc().isReturn() &&
      "Can only put epilogue before a return instruction!");

  // Start of epilogue comment.
  MDNode* start_epilogue_mdnode = MDNode::get(getGlobalContext(),
      ArrayRef<Value*>(MDString::get(getGlobalContext(), "EPILOGUE START")));
  BuildMI(MBB, MBBI, dl, TII.get(ARC::COMMENT))
      .addMetadata(start_epilogue_mdnode);

  // The stack size should be 4-byte aligned.
  unsigned NumBytes = MFI->getStackSize();
  NumBytes = (NumBytes + 3) & ~3;

 /*
  * At the end of the function, the compiler-generated epilogue does the
  * following:
  *   • Restores the stack pointer (SP) to the beginning of the saved register
  *     area.
  *   • Restores all non-volatile registers that were saved in the register
  *     area.
  *   • Restores caller’s frame pointer register (FP), if required.
  *   • Restores the return address register (BLINK).
  *   • Restores caller’s stack pointer register (SP).
  *   • Returns to caller through the address stored in the blink register.
  */

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
  if (MRI.isLiveIn(ARC::BLINK) || MRI.isLiveOut(ARC::BLINK)) {
    BuildMI(MBB, MBBI, dl, TII.get(ARC::ADDrsi), ARC::SP).addReg(ARC::SP)
        .addImm(UNITS_PER_WORD);
    BuildMI(MBB, MBBI, dl, TII.get(ARC::LDri)).addReg(ARC::SP)
        .addReg(ARC::BLINK).addImm(-UNITS_PER_WORD);
  }

  // TODO: Is it okay to have LD/ST in the delay slot?
  // Move jump before the final non-jump isntruction, as the
  // jump has a delay slot.
  MachineBasicBlock::iterator BeforeDelayInstr = MBBI;
  MachineBasicBlock::iterator AfterDelayInstr = MBBI;
  BeforeDelayInstr--;
  MBB.splice(BeforeDelayInstr, &MBB, AfterDelayInstr);

  // End of epilogue comment.
  // For some reason, the MBBI ends up 2 instructions behind after the splice.
  // I am probably doing the splice wrong but this hack works.
  MBBI++;
  MBBI++;
  MDNode* end_epilogue_mdnode = MDNode::get(getGlobalContext(),
      ArrayRef<Value*>(MDString::get(getGlobalContext(), "EPILOGUE END")));
  BuildMI(MBB, MBBI, dl, TII.get(ARC::COMMENT))
      .addMetadata(end_epilogue_mdnode);
}

bool ARCompactFrameLowering::hasFP(const MachineFunction &MF) const {
  return (DisableFramePointerElim(MF) ||
      MF.getFrameInfo()->hasVarSizedObjects());
}
