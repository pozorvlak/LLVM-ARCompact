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
    COND_AL,  // Always (i.e. 'True').
    COND_EQ,  // Equal.
    COND_NE,  // Not Equal.
    COND_P,   // Positive.
    COND_N,   // Negative.
    COND_LO,  // Lower Than.
    COND_HS,  // Higher Than or Same.
    COND_V,   // Overflow.
    COND_NV,  // No Overflow.
    COND_GT,  // Greater Than.
    COND_GE,  // Greater Than or Equal To.
    COND_LT,  // Less Than.
    COND_LE,  // Less Than or Equal To.
    COND_HI,  // Higher Than.
    COND_LS,  // Lower Than or Same.
    COND_PNZ, // Positive, non-zero.

    COND_INVALID
  };
}

inline static const char* ARCCCToString(ARCCC::CondCodes CC) {
  switch (CC) {
    case ARCCC::COND_AL:  return "al";
    case ARCCC::COND_EQ:  return "eq";
    case ARCCC::COND_NE:  return "ne";
    case ARCCC::COND_P:   return "p";
    case ARCCC::COND_N:   return "n";
    case ARCCC::COND_LO:  return "lo";
    case ARCCC::COND_HS:  return "hs";
    case ARCCC::COND_V:   return "v";
    case ARCCC::COND_NV:  return "nv";
    case ARCCC::COND_GT:  return "gt";
    case ARCCC::COND_GE:  return "ge";
    case ARCCC::COND_LT:  return "lt";
    case ARCCC::COND_LE:  return "le";
    case ARCCC::COND_HI:  return "hi";
    case ARCCC::COND_LS:  return "ls";
    case ARCCC::COND_PNZ: return "pnz";
    default:
      llvm_unreachable("Unknown condition code");
  }
}

namespace llvm {
  // Forward declarations.
  class ARCompactTargetMachine;
  class FunctionPass;
  class formatted_raw_ostream;

  FunctionPass *createARCompactISelDag(ARCompactTargetMachine &TM);
} // end namespace llvm;

#endif
