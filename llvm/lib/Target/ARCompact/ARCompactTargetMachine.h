//===---- ARCompactTargetMachine.h - Define TargetMachine for ARCompact ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the ARCompact specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef ARCOMPACTTARGETMACHINE_H
#define ARCOMPACTTARGETMACHINE_H

#include "ARCompactInstrInfo.h"
#include "ARCompactISelLowering.h"
#include "ARCompactFrameLowering.h"
#include "ARCompactSelectionDAGInfo.h"
#include "ARCompactSubtarget.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Target/TargetFrameLowering.h"

namespace llvm {

class ARCompactTargetMachine : public LLVMTargetMachine {
  ARCompactSubtarget Subtarget;
  const TargetData DataLayout;       // Calculates type size & alignment
  ARCompactTargetLowering TLInfo;
  ARCompactSelectionDAGInfo TSInfo;
  ARCompactInstrInfo InstrInfo;
  ARCompactFrameLowering FrameLowering;
public:
  ARCompactTargetMachine(const Target &T, StringRef TT,
                     StringRef CPU, StringRef FS,
                     Reloc::Model RM, CodeModel::Model CM);

  virtual const ARCompactInstrInfo *getInstrInfo() const { return &InstrInfo; }
  virtual const TargetFrameLowering  *getFrameLowering() const {
    return &FrameLowering;
  }
  virtual const ARCompactSubtarget   *getSubtargetImpl() const {
    return &Subtarget;
  }
  virtual const ARCompactRegisterInfo *getRegisterInfo() const {
    return &InstrInfo.getRegisterInfo();
  }
  virtual const ARCompactTargetLowering* getTargetLowering() const {
    return &TLInfo;
  }
  virtual const ARCompactSelectionDAGInfo* getSelectionDAGInfo() const {
    return &TSInfo;
  }
  virtual const TargetData       *getTargetData() const { return &DataLayout; }

  // Pass Pipeline Configuration
  virtual bool addInstSelector(PassManagerBase &PM, CodeGenOpt::Level OptLevel);
  virtual bool addPreEmitPass(PassManagerBase &PM, CodeGenOpt::Level OptLevel);
};

// Encore processor.
class ARCompactEncoreTargetMachine : public ARCompactTargetMachine {
public:
  ARCompactEncoreTargetMachine(const Target &T, StringRef TT,
                       StringRef CPU, StringRef FS,
                       Reloc::Model RM, CodeModel::Model CM);
};

} // end namespace llvm

#endif

