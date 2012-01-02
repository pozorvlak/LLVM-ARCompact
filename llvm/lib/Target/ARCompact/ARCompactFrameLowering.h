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
    // TODO: Check stack grows down, that the SP is 4-byte aligned, and that 
    // the local area offset is 0.
    : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, 4, 0), 
      STI(sti) {
  }

  /// emitProlog/emitEpilog - These methods insert prolog and epilog code into
  /// the function.
  void emitPrologue(MachineFunction &MF) const;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const;

  bool hasFP(const MachineFunction &MF) const;
};

} // End llvm namespace

#endif

