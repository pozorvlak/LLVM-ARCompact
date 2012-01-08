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

namespace ARCCC {
  // ARC specific condition codes.
  enum CondCodes {
    COND_AL, // Always (aka 'True').
    COND_EQ, // Equal (aka COND_Z).
    COND_NE, // Not Equal (aka COND_NZ).
    COND_P, // Positive (aka COND_NN).
    COND_N, // Negative (aka COND_N).
    COND_LO, // Lower Than (aka COND_C).
    COND_HS, // Higher Than or Same (aka COND_NC).
    COND_V, // Overflow (aka COND_V).
    COND_NV, // No Overflow (aka COND_NV).
    COND_GT, // Greater Than.
    COND_GE, // Greater Than or Equal To.
    COND_LT, // Less Than.
    COND_LE, // Less Than or Equal To.
    COND_HI, // Higher Than.
    COND_LS, // Lower Than or Same.
    COND_PNZ, // Positive, non-zero. (aka COND_NN and COND_NZ).

    COND_INVALID
  };
} 

namespace llvm {
  // Forward declarations.
  class ARCompactTargetMachine;
  class FunctionPass;
  class formatted_raw_ostream;

  FunctionPass *createARCompactISelDag(ARCompactTargetMachine &TM);
} // end namespace llvm;

#endif
