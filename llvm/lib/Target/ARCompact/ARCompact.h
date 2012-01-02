//===--- ARCompact.h - Top-level interface for ARCompact representation ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// ARCompact back-end.
//
//===----------------------------------------------------------------------===//

#ifndef TARGET_ARCOMPACT_H
#define TARGET_ARCOMPACT_H

#include "MCTargetDesc/ARCompactMCTargetDesc.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetMachine.h"
#include <cassert>

namespace llvm {
  // Forward declarations.
  class ARCompactTargetMachine;
  class FunctionPass;
  class formatted_raw_ostream;

  FunctionPass *createARCompactISelDag(ARCompactTargetMachine &TM);
} // end namespace llvm;

#endif
