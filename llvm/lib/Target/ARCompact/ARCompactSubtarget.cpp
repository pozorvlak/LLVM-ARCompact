//===------ ARCompactSubtarget.cpp - ARCOMPACT Subtarget Information ------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the ARCOMPACT specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "ARCompactSubtarget.h"
#include "ARCompact.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "ARCompactGenSubtargetInfo.inc"

using namespace llvm;

ARCompactSubtarget::ARCompactSubtarget(const std::string &TT,
    const std::string &CPU, const std::string &FS)
    : ARCompactGenSubtargetInfo(TT, CPU, FS) {
  // Determine default and user specified characteristics
  std::string CPUName = CPU;
  if (CPUName.empty()) {
    CPUName = "encore";
  }

  // Parse features string.
  ParseSubtargetFeatures(CPUName, FS);
}
