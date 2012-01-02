//===--- ARCompactTargetMachine.cpp - Define TargetMachine for ARCompact --===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "ARCompact.h"
#include "ARCompactTargetMachine.h"
#include "llvm/PassManager.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

extern "C" void LLVMInitializeARCompactTarget() {
  // Register the target.
  RegisterTargetMachine<ARCompactEncoreTargetMachine> 
      X(TheARCompactEncoreTarget);
}

ARCompactTargetMachine::ARCompactTargetMachine(const Target &T, StringRef TT, 
                                       StringRef CPU, StringRef FS,
                                       Reloc::Model RM, CodeModel::Model CM)
  : LLVMTargetMachine(T, TT, CPU, FS, RM, CM),
    Subtarget(TT, CPU, FS),
    DataLayout(Subtarget.getDataLayout()),
    TLInfo(*this),
    TSInfo(*this),
    InstrInfo(Subtarget),
    FrameLowering(Subtarget) {
}

bool ARCompactTargetMachine::addInstSelector(PassManagerBase &PM,
                                         CodeGenOpt::Level OptLevel) {
  PM.add(createARCompactISelDag(*this));
  return false;
}

/// addPreEmitPass - This pass may be implemented by targets that want to run
/// passes immediately before machine code is emitted.  This should return
/// true if -print-machineinstrs should print out the code after the passes.
bool ARCompactTargetMachine::addPreEmitPass(PassManagerBase &PM,
                                        CodeGenOpt::Level OptLevel){
  return true;
}

ARCompactEncoreTargetMachine::ARCompactEncoreTargetMachine(const Target &T,
                                           StringRef TT, StringRef CPU,
                                           StringRef FS, Reloc::Model RM,
                                           CodeModel::Model CM)
  : ARCompactTargetMachine(T, TT, CPU, FS, RM, CM) {
}

