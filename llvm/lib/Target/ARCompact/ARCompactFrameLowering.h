//===--- ARCompactFrameLowering.h - Define frame lowering for ARCompact ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//
//===----------------------------------------------------------------------===//

#ifndef ARCOMPACT_FRAMEINFO_H
#define ARCOMPACT_FRAMEINFO_H

#include "ARCompact.h"
#include "ARCompactSubtarget.h"
#include "llvm/Target/TargetFrameLowering.h"

namespace llvm {
  class ARCompactSubtarget;

class ARCompactFrameLowering : public TargetFrameLowering {
  const ARCompactSubtarget &STI;
  // There are 4 bytes per word in ARCompact
  static const int UNITS_PER_WORD = 4;
public:
  explicit ARCompactFrameLowering(const ARCompactSubtarget &sti)
      : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, 4, 0),
        STI(sti) {
  }

  /// Emit prologue code into a function. According to the ARC convention,
  /// which is almost identical to the ARCompact one, the prologue must:
  ///
  ///   * Allocate space for register arguments in the case of variadic functions.
  ///   * Save the return address register (BLINK) if required.
  ///   * Save required non-volatile general-purpose registers into the
  ///     register save area.
  ///   * Save the caller’s frame pointer (FP) if required, and set the new
  ///     frame pointer to this location (before the stack frame.)
  ///   * Decrements the stack pointer (SP) to account for the new stack frame.
  void emitPrologue(MachineFunction &MF) const;

  /// Emit epilogue code into a function. According to the ARC convention,
  /// which is almost identical to the ARCompact one, the epilogue must:
  ///
  ///   * Restore the stack pointer (SP) to the beginning of the saved register
  ///     area.
  ///   * Restore all non-volatile registers that were saved in the register
  ///     area.
  ///   * Restore the caller’s frame pointer register (FP) if required.
  ///   * Restore the return address register (BLINK) if required.
  ///   * Restore the caller’s stack pointer register (SP).
  ///   * Returns to caller through the address stored in the BLINK register.
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const;

  bool hasFP(const MachineFunction &MF) const;
};

} // End llvm namespace

#endif

