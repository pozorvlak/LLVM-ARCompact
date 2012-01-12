//===---- ARCompactRegisterInfo.h - ARCompact Register Information Impl ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the ARCompact implementation of the TargetRegisterInfo
// class.
//
//===----------------------------------------------------------------------===//

#ifndef ARCOMPACTREGISTERINFO_H
#define ARCOMPACTREGISTERINFO_H

#include "llvm/Target/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "ARCompactGenRegisterInfo.inc"

namespace llvm {

class ARCompactSubtarget;
class TargetInstrInfo;
class Type;

struct ARCompactRegisterInfo : public ARCompactGenRegisterInfo {
  ARCompactSubtarget &Subtarget;
  const TargetInstrInfo &TII;

  ARCompactRegisterInfo(ARCompactSubtarget &st, const TargetInstrInfo &tii);

  /// Returns a null-terminated list of all of the callee saved registers.
  const unsigned *getCalleeSavedRegs(const MachineFunction *MF = 0) const;

  /// Returns a bitset indexed by physical register number indicating if a
  /// register is a special register that has particular uses and should be
  /// considered unavailable at all times, e.g. SP, BLINK.
  BitVector getReservedRegs(const MachineFunction &MF) const;

  ///  This method is called during prolog/epilog code insertion to eliminate
  /// call frame setup and destroy pseudo instructions, such as ADJCALLSTACKUP
  /// and ADJCALLSTACKDOWN.
  void eliminateCallFramePseudoInstr(MachineFunction &MF,
      MachineBasicBlock &MBB, MachineBasicBlock::iterator I) const;

  /// Eliminate abstract frame indices from instructions which may use them.
  void eliminateFrameIndex(MachineBasicBlock::iterator II, int ARCAdj,
      RegScavenger *RS = NULL) const;

  // Debug information queries.
  unsigned getRARegister() const;
  unsigned getFrameRegister(const MachineFunction &MF) const;
};

} // end namespace llvm

#endif
